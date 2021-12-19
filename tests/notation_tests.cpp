//
// Dec-2021, Michael Lindner
// MIT license
//
#include "notation_tests.h"
#include "notation.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testSanScheme()
{
   {
      const std::string caseLabel = "San::scheme()";

      VERIFY((San().scheme() == NotationScheme::SAN), caseLabel);
   }
}


void testSanNotatePiece()
{
   {
      const std::string caseLabel = "San::notate piece";

      //VERIFY((San().notate() == NotationScheme::SAN), caseLabel);
   }
}


///////////////////

void testLanScheme()
{
   {
      const std::string caseLabel = "Lan::scheme()";

      VERIFY((Lan().scheme() == NotationScheme::LAN), caseLabel);
   }
}


///////////////////

void testDetailedNotationScheme()
{
   {
      const std::string caseLabel = "DetailedNotation::scheme()";

      VERIFY((DetailedNotation().scheme() == NotationScheme::Detailed), caseLabel);
   }
}

} // namespace


///////////////////

void testNotations()
{
   testSanScheme();
   testSanNotatePiece();
   //testSanNotateBasicMove();
   //testSanNotateCastling();
   //testSanNotateEnPassant();
   //testSanNotatePromotion();

   testLanScheme();

   testDetailedNotationScheme();
}
