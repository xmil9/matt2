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

///////////////////

static void printPiece(std::optional<Piece> piece)
{
   static const std::vector<std::string> rep = {"\u2654", "\u2655", "\u2656", "\u2657",
                                                "\u2658", "\u2659", "\u265a", "\u265b",
                                                "\u265c", "\u265d", "\u265e", "\u265f"};
   if (piece)
      std::cout << rep[static_cast<unsigned char>(*piece)];
}

static void printEmptySquare(Square sq)
{
   static const std::string w{"_"};
   static const std::string b{"."};
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

   std::cout << toChar(r) << "|";
   for (File f = fa; f <= fh; f = f + 1)
      printSquare(pos, f, r);
   std::cout << "|" << toChar(r) << "\n";
}

static void printBoard(const Game& game, Color perspective)
{
   const bool forWhite = perspective == White;
   const Rank start = forWhite ? r8 : r1;
   const Rank end = forWhite ? r1 : r8;
   const int step = forWhite ? -1 : 1;

   std::cout << "  abcdefgh\n";
   std::cout << "  --------\n";
   for (Rank r = start; r != end; r = r + step)
      printRank(r, game);
   printRank(end, game);
   std::cout << "  --------\n";
   std::cout << "  abcdefgh\n";

   std::cout << std::flush;
}

///////////////////

int main()
{
   setupUtf8();

   std::cout << "CHESS\n"
             << "=====\n";

   const Color playerColor = readInput("Play as [w]hite or [b]lack? ", {"w", "b"}) == "w"
                                ? Color::White
                                : Color::Black;

   Game game;
   Color nextTurn = Color::White;

   printBoard(game, playerColor);
   while (true)
   {
      if (nextTurn == playerColor)
      {
      }
   }

   return EXIT_SUCCESS;
}
