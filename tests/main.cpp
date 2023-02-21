//
// Jun-2021, Michael Lindner
// MIT license
//
#include "daily_chess_scoring_tests.h"
#include "game_tests.h"
#include "move_tests.h"
#include "notation_tests.h"
#include "piece_tests.h"
#include "piece_value_scoring_tests.h"
#include "placement_tests.h"
#include "position_tests.h"
#include "relocation_tests.h"
#include "rules_tests.h"
#include "scoring_tests.h"
#include "square_tests.h"
#include <cstdlib>
#include <iostream>


int main()
{
   testColor();
   testDailyChessScoring();
   testDiagonal();
   testFile();
   testGame();
   testMoves();
   testNotations();
   testOffset();
   testPiece();
   testPieceIterator();
   testPieceValueScoring();
   testPlacement();
   testPlacementIterator();
   testPosition();
   testQuadrant();
   testRank();
   testRelocation();
   testRules();
   testScoring();
   testSquare();

   std::cout << "matt2 tests finished.\n";
   return EXIT_SUCCESS;
}
