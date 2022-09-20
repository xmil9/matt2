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

void testLanScheme()
{
   {
      const std::string caseLabel = "Lan::scheme()";

      VERIFY((Lan().scheme() == NotationScheme::LAN), caseLabel);
   }
}

void testLanNotatePiece()
{
   {
      const std::string caseLabel = "Lan::notate piece";

      std::string out;
      Lan lan;

      out.clear();
      VERIFY((lan.notate(out, Kw) == "K"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Kb) == "K"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Qw) == "Q"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Qb) == "Q"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Rw) == "R"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Rb) == "R"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Bw) == "B"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Bb) == "B"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Nw) == "N"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Nb) == "N"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Pw) == ""), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Pb) == ""), caseLabel);
   }
}

void testLanNotateBasicMove()
{
   {
      const std::string caseLabel = "Lan::notate basic move without taking";

      std::string out;
      Lan lan;
      BasicMove move{Relocation{"Qbf5a5"}};

      VERIFY((lan.notate(out, move) == "Qf5a5"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate basic move with taking";

      std::string out;
      Lan lan;
      BasicMove move{Relocation{"Qbf5a5"}, Bw};

      VERIFY((lan.notate(out, move) == "Qf5xa5"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate basic move for pawn with taking";

      std::string out;
      Lan lan;
      BasicMove move{Relocation{"wc2d3"}, Nb};

      VERIFY((lan.notate(out, move) == "c2xd3"), caseLabel);
   }
}

void testLanNotateCastling()
{
   {
      const std::string caseLabel = "Lan::notate castling move at king-side";

      std::string out;
      Lan lan;
      Castling move{Kingside, Color::White};

      VERIFY((lan.notate(out, move) == "0-0"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate castling move at queen-side";

      std::string out;
      Lan lan;
      Castling move{Queenside, Color::Black};

      VERIFY((lan.notate(out, move) == "0-0-0"), caseLabel);
   }
}

void testLanNotateEnPassant()
{
   {
      const std::string caseLabel = "Lan::notate en-passant move for white to higher file";

      std::string out;
      Lan lan;
      EnPassant move{Relocation{"wc5d6"}};

      VERIFY((lan.notate(out, move) == "cxd6"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate en-passant move for black to lower file";

      std::string out;
      Lan lan;
      EnPassant move{Relocation{"bh4g3"}};

      VERIFY((lan.notate(out, move) == "hxg3"), caseLabel);
   }
}


///////////////////

void testDnScheme()
{
   {
      const std::string caseLabel = "DetailedNotation::scheme()";

      VERIFY((DetailedNotation().scheme() == NotationScheme::Detailed), caseLabel);
   }
}

void testDnNotatePiece()
{
   {
      const std::string caseLabel = "DetailedNotation::notate piece";

      std::string out;
      DetailedNotation dn;

      out.clear();
      VERIFY((dn.notate(out, Kw) == "Kw"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Kb) == "Kb"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Qw) == "Qw"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Qb) == "Qb"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Rw) == "Rw"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Rb) == "Rb"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Bw) == "Bw"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Bb) == "Bb"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Nw) == "Nw"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Nb) == "Nb"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Pw) == "w"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Pb) == "b"), caseLabel);
   }
}

void testDnNotateBasicMove()
{
   {
      const std::string caseLabel = "DetailedNotation::notate basic move without taking";

      std::string out;
      DetailedNotation dn;
      BasicMove move{Relocation{"Qbf5a5"}};

      VERIFY((dn.notate(out, move) == "Qbf5a5"), caseLabel);
   }
   {
      const std::string caseLabel = "DetailedNotation::notate basic move with taking";

      std::string out;
      DetailedNotation dn;
      BasicMove move{Relocation{"Qbf5a5"}, Bw};

      VERIFY((dn.notate(out, move) == "Qbf5xa5[x:Bw]"), caseLabel);
   }
   {
      const std::string caseLabel = "DetailedNotation::notate basic move for pawn with taking";

      std::string out;
      DetailedNotation dn;
      BasicMove move{Relocation{"wc2d3"}, Nb};

      VERIFY((dn.notate(out, move) == "wc2xd3[x:Nb]"), caseLabel);
   }
}

void testDnNotateCastling()
{
   {
      const std::string caseLabel = "DetailedNotation::notate castling move at king-side";

      std::string out;
      DetailedNotation dn;
      Castling move{Kingside, Color::White};

      VERIFY((dn.notate(out, move) == "w0-0"), caseLabel);
   }
   {
      const std::string caseLabel = "DetailedNotation::notate castling move at queen-side";

      std::string out;
      DetailedNotation dn;
      Castling move{Queenside, Color::Black};

      VERIFY((dn.notate(out, move) == "b0-0-0"), caseLabel);
   }
}

void testDnNotateEnPassant()
{
   {
      const std::string caseLabel = "DetailedNotation::notate en-passant move for white to higher file";

      std::string out;
      DetailedNotation dn;
      EnPassant move{Relocation{"wc5d6"}};

      VERIFY((dn.notate(out, move) == "wcxd6[x:b]"), caseLabel);
   }
   {
      const std::string caseLabel = "DetailedNotation::notate en-passant move for black to lower file";

      std::string out;
      DetailedNotation dn;
      EnPassant move{Relocation{"bh4g3"}};

      VERIFY((dn.notate(out, move) == "bhxg3[x:w]"), caseLabel);
   }
}

} // namespace


///////////////////

void testNotations()
{
   testLanScheme();
   testLanNotatePiece();
   testLanNotateBasicMove();
   testLanNotateCastling();
   testLanNotateEnPassant();
   //testLanNotatePromotion();

   testDnScheme();
   testDnNotatePiece();
   testDnNotateBasicMove();
   testDnNotateCastling();
   testDnNotateEnPassant();
}
