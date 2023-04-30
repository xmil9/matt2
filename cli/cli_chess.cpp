//
// Apr-2023, Michael Lindner
// MIT license
//
#include "game.h"
#include "notation.h"
#include "essentutils/string_util.h"
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#ifdef _WIN32
#include <ConsoleApi2.h>
#endif

using namespace matt2;

///////////////////

static void setupUtf8()
{
#ifdef _WIN32
   // In addition to the settings below, the VS project flag /utf-8 needs to be set
   // in the 'C/C++ > Command Line > Additional Options'.

   // Set console code page to UTF-8 so console known how to interpret string data.
   SetConsoleOutputCP(CP_UTF8);

   // Enable buffering to prevent VS from chopping up UTF-8 byte sequences
   setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif // win
}

///////////////////

static const std::string WhiteKing{"\u2654"};
static const std::string WhiteQueen{"\u2655"};
static const std::string WhiteRook{"\u2656"};
static const std::string WhiteBishop{"\u2657"};
static const std::string WhiteKnight{"\u2658"};
static const std::string WhitePawn{"\u2659"};
static const std::string BlackKing{"\u265a"};
static const std::string BlackQueen{"\u265b"};
static const std::string BlackRook{"\u265c"};
static const std::string BlackBishop{"\u265d"};
static const std::string BlackKnight{"\u265e"};
static const std::string BlackPawn{"\u265f"};
static const std::string WhiteSquare{"\u2591"};
static const std::string BlackSquare{"\u2588"};
static const std::string VertBorder{"\u2503"};
static const std::string HorzBorder{"\u2501"};
static const std::string TlCorner{"\u250f"};
static const std::string TrCorner{"\u2513"};
static const std::string BlCorner{"\u2517"};
static const std::string BrCorner{"\u251b"};
static const std::string BoardTop = TlCorner + HorzBorder + HorzBorder + HorzBorder +
                                    HorzBorder + HorzBorder + HorzBorder + HorzBorder +
                                    HorzBorder + TrCorner;
static const std::string BoardBottom = BlCorner + HorzBorder + HorzBorder + HorzBorder +
                                       HorzBorder + HorzBorder + HorzBorder + HorzBorder +
                                       HorzBorder + BrCorner;

static void printPiece(std::optional<Piece> piece)
{
   static const std::vector<std::string> rep = {
      WhiteKing, WhiteQueen, WhiteRook, WhiteBishop, WhiteKnight, WhitePawn,
      BlackKing, BlackQueen, BlackRook, BlackBishop, BlackKnight, BlackPawn};
   if (piece)
      std::cout << rep[static_cast<unsigned char>(*piece)];
}

static void printEmptySquare(Square sq)
{
   // Aliases
   static const std::string w = WhiteSquare;
   static const std::string b = BlackSquare;
   // clang-format off
   static const std::vector<std::string> rep = {
      b, w, b, w, b, w, b, w,
      w, b, w, b, w, b, w, b,
      b, w, b, w, b, w, b, w,
      w, b, w, b, w, b, w, b,
      b, w, b, w, b, w, b, w,
      w, b, w, b, w, b, w, b,
      b, w, b, w, b, w, b, w,
      w, b, w, b, w, b, w, b,
   };
   // clang-format on

   std::cout << rep[static_cast<unsigned char>(sq)];
}

static void printSquare(const Position& pos, File f, Rank r)
{
   const Square sq = makeSquare(f, r);
   const auto piece = pos[sq];
   if (piece)
      printPiece(*piece);
   else
      printEmptySquare(sq);
}

static void printRank(Rank r, const Game& g)
{
   const Position& pos = g.current();

   std::cout << toChar(r) << VertBorder;
   for (File f = fa; f <= fh; f = f + 1)
      printSquare(pos, f, r);
   std::cout << VertBorder << toChar(r) << "\n";
}

static void printBoard(const Game& game, Color perspective)
{
   const bool forWhite = perspective == White;
   const Rank start = forWhite ? r8 : r1;
   const Rank end = forWhite ? r1 : r8;
   const int step = forWhite ? -1 : 1;

   std::cout << "  abcdefgh\n";
   std::cout << " " << BoardTop << "\n";
   for (Rank r = start; r != end; r = r + step)
      printRank(r, game);
   printRank(end, game);
   std::cout << " " << BoardBottom << "\n";
   std::cout << "  abcdefgh\n";

   std::cout << std::flush;
}

static void printWelcome()
{
   std::cout << "Matt2 Chess\n"
             << "===========\n";
}

static void printHelp()
{
   std::cout << "\n";
   std::cout << "[q] - quit\n";
   std::cout << "[h] - help\n";
   std::cout << "Enter moves in Pure Algebraic Coordinate Notation:\n";
   std::cout << " General move:\n";
   std::cout << "  <from square><to square>[<promoted to>]\n";
   std::cout << "  with optional <promoted to> = q, r, b, n\n";
   std::cout << " Castling:\n";
   std::cout << "  <king-side> = O-O\n";
   std::cout << "  <queen-side> = O-O-O\n";
   std::cout << "Examples: d2d4, f7f8q, O-O-O\n";
   std::cout << "\n";
}

///////////////////

static const std::string Quit{"q"};
static const std::string Help{"h"};

static std::string capWord(std::string word)
{
   if (!word.empty())
      word[0] = static_cast<char>(std::toupper(word[0]));
   return word;
}

static bool isValidChoice(const std::string& choice,
                          const std::vector<std::string>& valid)
{
   return std::find(valid.begin(), valid.end(), choice) != valid.end();
}

static std::string readInput(std::string_view prompt,
                             const std::vector<std::string>& choices)
{
   std::string choice;

   while (!isValidChoice(choice, choices))
   {
      std::cout << prompt;
      std::cin >> choice;
      choice = esl::lowercase(choice);
   }

   return choice;
}

static std::string readInput(std::string_view prompt)
{
   std::cout << prompt;

   std::string choice;
   std::cin >> choice;
   choice = esl::lowercase(choice);

   return choice;
}

static std::optional<Color> readPlayerColor()
{
   while (true)
   {
      const std::string input =
         readInput("Play as [w]hite or [b]lack? ", {"w", "b", Quit, Help});

      if (input == "w")
         return White;
      else if (input == "b")
         return Black;
      else if (input == Quit)
         return std::nullopt;
      else if (input == Help)
         printHelp();
   }

   return std::nullopt;
}

static std::optional<size_t> readDifficulty()
{
   while (true)
   {
      const std::string input =
         readInput("Difficulty 1-3? ", {"1", "2", "3", Quit, Help});

      if (input == Quit)
         return std::nullopt;
      else if (input == Help)
         printHelp();
      else
         return std::stoi(input);
   }

   return std::nullopt;
}

///////////////////

static bool playersTurn(Game& g)
{
   bool quit = false;
   bool validMove = false;
   std::string moveDescr;

   while (!quit && !validMove)
   {
      const std::string input = readInput("Your move? ");

      if (esl::lowercase(input) == Quit)
      {
         quit = true;
      }
      else
      {
         std::tie(validMove, moveDescr) = g.enterNextMove(input);

         if (!validMove)
            std::cout << moveDescr << " Try again.\n";
      }
   }

   return quit;
}

static bool enginesTurn(Game& g, size_t turnDepth, Color engineColor)
{
   const auto [validMove, moveDescr] = g.calcNextMove(turnDepth);
   const bool gameOver = !validMove;

   if (validMove)
      std::cout << capWord(toString(engineColor)) << " move: " << moveDescr << "\n";
   else
      std::cout << moveDescr << "\n";

   return gameOver;
}

///////////////////

int main()
{
   setupUtf8();

   printWelcome();
   printHelp();

   const auto playerColor = readPlayerColor();
   if (!playerColor)
      return EXIT_SUCCESS;

   const auto difficulty = readDifficulty();
   if (!difficulty)
      return EXIT_SUCCESS;

   Game game;
   Color nextTurn = White;
   bool gameOver = false;

   printBoard(game, *playerColor);

   while (!gameOver)
   {
      if (nextTurn == playerColor)
         gameOver = playersTurn(game);
      else
         gameOver = enginesTurn(game, *difficulty, !*playerColor);

      if (!gameOver)
      {
         nextTurn = !nextTurn;
         printBoard(game, *playerColor);
      }
   }

   return EXIT_SUCCESS;
}
