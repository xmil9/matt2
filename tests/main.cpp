//
// Jun-2021, Michael Lindner
// MIT license
//
#include "move_tests.h"
#include "piece_tests.h"
#include "placement_tests.h"
#include "position_tests.h"
#include "relocation_tests.h"
#include "square_tests.h"
#include <cstdlib>
#include <iostream>


int main()
{
   testColor();
   testFile();
   testMoves();
   testPiece();
   testPlacement();
   testPosition();
   testRank();
   testRelocation();
   testSquare();

   std::cout << "matt2 tests finished.\n";
   return EXIT_SUCCESS;
}
