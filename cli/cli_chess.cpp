//
// Apr-2023, Michael Lindner
// MIT license
//
#include "game.h"
#include "notation.h"
#include "essentutils/string_util.h"
#include <iostream>
#include <string>
#include <vector>

using namespace matt2;

///////////////////

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

void printBoard(const Game& game)
{
   std::string out;
   std::cout << printPosition(out, game.current()) << "\n";
}

///////////////////

int main()
{
   std::cout << "CHESS\n"
             << "=====\n";

   const Color playerColor = readInput("Play as [w]hite or [b]lack? ", {"w", "b"}) == "w"
                                ? Color::White
                                : Color::Black;
   
   Game game;
   Color whoseTurn = Color::White;

   printBoard(game);
   while (true)
   {
      if (whoseTurn == playerColor)
      {

      }
   }

   return EXIT_SUCCESS;
}
