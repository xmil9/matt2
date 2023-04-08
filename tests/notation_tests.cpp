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
      VERIFY(lan.notate(out, White) == "w", caseLabel);
      out.clear();
      VERIFY(lan.notate(out, Black) == "b", caseLabel);
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
      Castling move{Kingside, White};

      VERIFY((lan.notate(out, move) == "0-0"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate castling move at queen-side";

      std::string out;
      Lan lan;
      Castling move{Queenside, Black};

      VERIFY((lan.notate(out, move) == "0-0-0"), caseLabel);
   }
}

void testLanNotateEnPassant()
{
   {
      const std::string caseLabel =
         "Lan::notate en-passant move for white to higher file";

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
      const std::string caseLabel =
         "Lan::notate promotion move for white to queen without taking";

      std::string out;
      Lan lan;
      Promotion move{Relocation{"wc7c8"}, Qw};

      VERIFY((lan.notate(out, move) == "c7c8=Q"), caseLabel);
   }
   {
      const std::string caseLabel =
         "Lan::notate promotion move for black to knight with taking";

      std::string out;
      Lan lan;
      Promotion move{Relocation{"bb2a1"}, Nb, Bw};

      VERIFY((lan.notate(out, move) == "b2xa1=N"), caseLabel);
   }
}

void testLanNotatePosition()
{
   {
      const std::string caseLabel = "Lan::notate position without pieces";

      std::string out;
      Lan lan;
      Position pos;

      VERIFY((lan.notate(out, pos) == ""), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate position with one piece";

      std::string out;
      Lan lan;
      Position pos{"Kba8"};

      VERIFY((lan.notate(out, pos) == "Kba8"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate position two pieces";

      std::string out;
      Lan lan;
      Position pos{"Kwf3 Kbb5"};

      VERIFY((lan.notate(out, pos) == "Kwf3 Kbb5"), caseLabel);
   }
   {
      const std::string caseLabel = "Lan::notate position with all pieces";

      std::string out;
      Lan lan;
      Position pos = StartPos;

      const std::string expected{
         "Rwa1 Rwh1 Bwc1 Bwf1 Nwb1 Nwg1 Qwd1 wa2 wb2 wc2 wd2 we2 wf2 wg2 wh2 Kwe1 Rba8 "
         "Rbh8 Bbc8 Bbf8 Nbb8 Nbg8 Qbd8 ba7 bb7 bc7 bd7 be7 bf7 bg7 bh7 Kbe8"};
      VERIFY((lan.notate(out, pos) == expected), caseLabel);
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
      VERIFY(dn.notate(out, White) == "w", caseLabel);
      out.clear();
      VERIFY(dn.notate(out, Black) == "b", caseLabel);
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
      const std::string caseLabel =
         "DetailedNotation::notate basic move for pawn with taking";

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
      Castling move{Kingside, White};

      VERIFY((dn.notate(out, move) == "w0-0"), caseLabel);
   }
   {
      const std::string caseLabel =
         "DetailedNotation::notate castling move at queen-side";

      std::string out;
      DetailedNotation dn;
      Castling move{Queenside, Black};

      VERIFY((dn.notate(out, move) == "b0-0-0"), caseLabel);
   }
}

void testDnNotateEnPassant()
{
   {
      const std::string caseLabel =
         "DetailedNotation::notate en-passant move for white to higher file";

      std::string out;
      DetailedNotation dn;
      EnPassant move{Relocation{"wc5d6"}};

      VERIFY((dn.notate(out, move) == "wcxd6[x:b]"), caseLabel);
   }
   {
      const std::string caseLabel =
         "DetailedNotation::notate en-passant move for black to lower file";

      std::string out;
      DetailedNotation dn;
      EnPassant move{Relocation{"bh4g3"}};

      VERIFY((dn.notate(out, move) == "bhxg3[x:w]"), caseLabel);
   }
}

void testDnNotatePromotion()
{
   {
      const std::string caseLabel =
         "DetailedNotation::notate promotion move for white to queen without taking";

      std::string out;
      DetailedNotation dn;
      Promotion move{Relocation{"wc7c8"}, Qw};

      VERIFY((dn.notate(out, move) == "wc7c8=Qw"), caseLabel);
   }
   {
      const std::string caseLabel =
         "DetailedNotation::notate promotion move for black to knight with taking";

      std::string out;
      DetailedNotation dn;
      Promotion move{Relocation{"bb2a1"}, Nb, Bw};

      VERIFY((dn.notate(out, move) == "bb2xa1=Nb[x:Bw]"), caseLabel);
   }
}

void testDnNotatePosition()
{
   // For now same as:
   // testLanNotatePosition()
}

///////////////////

void testPrintPosition()
{
   {
      const std::string caseLabel = "printPosition for empty position";

      std::string out;
      Position pos;

      const std::string expected{"  abcdefgh\n"
                                 "  --------\n"
                                 "8|........|8\n"
                                 "7|........|7\n"
                                 "6|........|6\n"
                                 "5|........|5\n"
                                 "4|........|4\n"
                                 "3|........|3\n"
                                 "2|........|2\n"
                                 "1|........|1\n"
                                 "  --------\n"
                                 "  abcdefgh\n"};
      VERIFY((printPosition(out, pos) == expected), caseLabel);
   }
   {
      const std::string caseLabel = "printPosition for one piece";

      std::string out;
      Position pos{"Kbf5"};

      const std::string expected{"  abcdefgh\n"
                                 "  --------\n"
                                 "8|........|8\n"
                                 "7|........|7\n"
                                 "6|........|6\n"
                                 "5|.....k..|5\n"
                                 "4|........|4\n"
                                 "3|........|3\n"
                                 "2|........|2\n"
                                 "1|........|1\n"
                                 "  --------\n"
                                 "  abcdefgh\n"};
      VERIFY((printPosition(out, pos) == expected), caseLabel);
   }
   {
      const std::string caseLabel = "printPosition for all pieces";

      std::string out;
      Position pos = StartPos;

      const std::string expected{"  abcdefgh\n"
                                 "  --------\n"
                                 "8|rnbqkbnr|8\n"
                                 "7|pppppppp|7\n"
                                 "6|........|6\n"
                                 "5|........|5\n"
                                 "4|........|4\n"
                                 "3|........|3\n"
                                 "2|PPPPPPPP|2\n"
                                 "1|RNBQKBNR|1\n"
                                 "  --------\n"
                                 "  abcdefgh\n"};
      VERIFY((printPosition(out, pos) == expected), caseLabel);
   }
}

void testReadMovePacn()
{
   {
      const std::string caseLabel = "readMovePacn for valid descriptions";

      VERIFY(readMovePacn("a1a3") == std::optional(MoveDescription(a1, a3, std::nullopt)),
             caseLabel);
      VERIFY(readMovePacn("b2c2") == std::optional(MoveDescription(b2, c2, std::nullopt)),
             caseLabel);
      VERIFY(readMovePacn("d3e4") == std::optional(MoveDescription(d3, e4, std::nullopt)),
             caseLabel);
      VERIFY(readMovePacn("f5g6") == std::optional(MoveDescription(f5, g6, std::nullopt)),
             caseLabel);
      VERIFY(readMovePacn("h7h8") == std::optional(MoveDescription(h7, h8, std::nullopt)),
             caseLabel);
      VERIFY(readMovePacn("g7g8q") ==
                std::optional(MoveDescription(g7, g8, MoveDescription::Promotion::Queen)),
             caseLabel);
      VERIFY(readMovePacn("g7g8r") ==
                std::optional(MoveDescription(g7, g8, MoveDescription::Promotion::Rook)),
             caseLabel);
      VERIFY(readMovePacn("g7g8b") == std::optional(MoveDescription(
                                         g7, g8, MoveDescription::Promotion::Bishop)),
             caseLabel);
      VERIFY(readMovePacn("g7g8n") == std::optional(MoveDescription(
                                         g7, g8, MoveDescription::Promotion::Knight)),
             caseLabel);
      VERIFY(readMovePacn("g7g8qignored") ==
                std::optional(MoveDescription(g7, g8, MoveDescription::Promotion::Queen)),
             caseLabel);
      VERIFY(readMovePacn("g7g8ignored") ==
                std::optional(MoveDescription(g7, g8, std::nullopt)),
             caseLabel);
   }
   {
      const std::string caseLabel = "readMovePacn for invalid descriptions";

      VERIFY(readMovePacn("s1a3") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("b9c2") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("d3x4") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("f5g0") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("----") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("g") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("g7") == std::nullopt, caseLabel);
      VERIFY(readMovePacn("g7g") == std::nullopt, caseLabel);
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
   testLanNotatePosition();

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
   testDnNotatePosition();

   testPrintPosition();
   testReadMovePacn();
}
