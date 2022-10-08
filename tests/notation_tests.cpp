//
// Dec-2021, Michael Lindner
// MIT license
//
#include "notation_tests.h"
#include "move.h"
#include "notation.h"
#include "placement.h"
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

void testLanNotateColor()
{
   {
      const std::string caseLabel = "Lan::notate color";

      std::string out;
      Lan lan;

      out.clear();
      VERIFY(lan.notate(out, Color::White) == "w", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, Color::Black) == "b", caseLabel);
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

void testLanNotateFile()
{
   {
      const std::string caseLabel = "Lan::notate file";

      std::string out;
      Lan lan;

      out.clear();
      VERIFY(lan.notate(out, fa) == "a", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, fb) == "b", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, fc) == "c", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, fd) == "d", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, fe) == "e", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, ff) == "f", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, fg) == "g", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, fh) == "h", caseLabel);

   }
}

void testLanNotateRank()
{
   {
      const std::string caseLabel = "Lan::notate rank";

      std::string out;
      Lan lan;

      out.clear();
      VERIFY(lan.notate(out, r1) == "1", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, r2) == "2", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, r3) == "3", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, r4) == "4", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, r5) == "5", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, r6) == "6", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, r7) == "7", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, r8) == "8", caseLabel);

   }
}

void testLanNotateSquare()
{
   {
      const std::string caseLabel = "Lan::notate square";

      std::string out;
      Lan lan;

      out.clear();
      VERIFY(lan.notate(out, a1) == "a1", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, b2) == "b2", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, c3) == "c3", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, d4) == "d4", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, e5) == "e5", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, f6) == "f6", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, g7) == "g7", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, h8) == "h8", caseLabel);

   }
}

void testLanNotatePlacement()
{
   {
      const std::string caseLabel = "Lan::notate placement";

      std::string out;
      Lan lan;

      out.clear();
      VERIFY((lan.notate(out, Placement(Kw, b4)) == "Kb4"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Placement(Rb, a8)) == "Ra8"), caseLabel);
      out.clear();
      VERIFY((lan.notate(out, Placement(Pb, f6)) == "f6"), caseLabel);
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

void testLanNotatePromotion()
{
   {
      const std::string caseLabel = "Lan::notate promotion move for white to queen without taking";

      std::string out;
      Lan lan;
      Promotion move{Relocation{"wc7c8"}, Qw};

      VERIFY((lan.notate(out, move) == "c7c8=Q"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate promotion move for black to knight with taking";

      std::string out;
      Lan lan;
      Promotion move{Relocation{"bb2a1"}, Nb, Bw};

      VERIFY((lan.notate(out, move) == "b2xa1=N"), caseLabel);
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

void testDnNotateColor()
{
   {
      const std::string caseLabel = "DetailedNotation::notate color";

      std::string out;
      DetailedNotation dn;

      out.clear();
      VERIFY(dn.notate(out, Color::White) == "w", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, Color::Black) == "b", caseLabel);
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

void testDnNotateFile()
{
   {
      const std::string caseLabel = "DetailedNotation::notate file";

      std::string out;
      DetailedNotation dn;

      out.clear();
      VERIFY(dn.notate(out, fa) == "a", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, fb) == "b", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, fc) == "c", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, fd) == "d", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, fe) == "e", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, ff) == "f", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, fg) == "g", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, fh) == "h", caseLabel);

   }
}

void testDnNotateRank()
{
   {
      const std::string caseLabel = "DetailedNotation::notate rank";

      std::string out;
      DetailedNotation dn;

      out.clear();
      VERIFY(dn.notate(out, r1) == "1", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, r2) == "2", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, r3) == "3", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, r4) == "4", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, r5) == "5", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, r6) == "6", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, r7) == "7", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, r8) == "8", caseLabel);

   }
}

void testDnNotateSquare()
{
   {
      const std::string caseLabel = "DetailedNotation::notate square";

      std::string out;
      DetailedNotation dn;

      out.clear();
      VERIFY(dn.notate(out, a1) == "a1", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, b2) == "b2", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, c3) == "c3", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, d4) == "d4", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, e5) == "e5", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, f6) == "f6", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, g7) == "g7", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, h8) == "h8", caseLabel);

   }
}

void testDnNotatePlacement()
{
   {
      const std::string caseLabel = "DetailedNotation::notate placement";

      std::string out;
      DetailedNotation dn;

      out.clear();
      VERIFY((dn.notate(out, Placement(Kw, b4)) == "Kwb4"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Placement(Rb, a8)) == "Rba8"), caseLabel);
      out.clear();
      VERIFY((dn.notate(out, Placement(Pb, f6)) == "bf6"), caseLabel);
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

void testDnNotatePromotion()
{
   {
      const std::string caseLabel = "DetailedNotation::notate promotion move for white to queen without taking";

      std::string out;
      DetailedNotation dn;
      Promotion move{Relocation{"wc7c8"}, Qw};

      VERIFY((dn.notate(out, move) == "wc7c8=Qw"), caseLabel);
   }
   {
      const std::string caseLabel = "DetailedNotation::notate promotion move for black to knight with taking";

      std::string out;
      DetailedNotation dn;
      Promotion move{Relocation{"bb2a1"}, Nb, Bw};

      VERIFY((dn.notate(out, move) == "bb2xa1=Nb[x:Bw]"), caseLabel);
   }
}

} // namespace


///////////////////

void testNotations()
{
   testLanScheme();
   testLanNotateColor();
   testLanNotatePiece();
   testLanNotateFile();
   testLanNotateRank();
   testLanNotateSquare();
   testLanNotatePlacement();
   testLanNotateBasicMove();
   testLanNotateCastling();
   testLanNotateEnPassant();
   testLanNotatePromotion();

   testDnScheme();
   testDnNotateColor();
   testDnNotatePiece();
   testDnNotateFile();
   testDnNotateRank();
   testDnNotateSquare();
   testDnNotatePlacement();
   testDnNotateBasicMove();
   testDnNotateCastling();
   testDnNotateEnPassant();
   testDnNotatePromotion();
}
