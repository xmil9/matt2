//
// Jun-2021, Michael Lindner
// MIT license
//
#include "move_tests.h"
#include "move.h"
#include "position.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testBasicMoveMainCtor()
{
   {
      const std::string caseLabel = "BasicMove main ctor.";

      Position pos{"Kba1 wf2"};
      BasicMove m{Relocation{"wf2f3"}};

      VERIFY(m.enPassantFile() == std::nullopt, caseLabel);
   }
}


void testBasicMoveEnPassantCtor()
{
   {
      const std::string caseLabel = "BasicMove ctor for en-passant.";

      Position pos{"Kba1 wf2"};
      BasicMove m{Relocation{"wf2f4"}, EnablesEnPassant};

      VERIFY(m.enPassantFile() == ff, caseLabel);
   }
}


void testBasicMoveMove()
{
   {
      const std::string caseLabel = "BasicMove::move without taking";

      Position pos{"Kba1 Rba3 Kwh8 wh3"};
      BasicMove m{Relocation{"Rba3c3"}};
      m.move(pos);

      VERIFY(pos[c3] == Rb, caseLabel);
      VERIFY(pos.locations(Rb).size() == 1, caseLabel);
      VERIFY(pos.locations(Rb)[0] == c3, caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
      // Verify uninvolved pieces.
      VERIFY(pos[a1] == Kb, caseLabel);
      VERIFY(pos[h8] == Kw, caseLabel);
      VERIFY(pos[h3] == Pw, caseLabel);
   }
   {
      const std::string caseLabel = "BasicMove::move with taking";

      Position pos{"Kba1 Rbc3 Kwh8 wh3"};
      BasicMove m{Relocation{"Rbc3h3"}, Pw};
      m.move(pos);

      VERIFY(pos[h3] == Rb, caseLabel);
      VERIFY(pos.locations(Rb).size() == 1, caseLabel);
      VERIFY(pos.locations(Rb)[0] == h3, caseLabel);
      VERIFY(pos.locations(Pw).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
      // Verify uninvolved pieces.
      VERIFY(pos[a1] == Kb, caseLabel);
      VERIFY(pos[h8] == Kw, caseLabel);
   }
   {
      const std::string caseLabel = "BasicMove::move with en-passant";

      Position pos{"Kba1 Kwh8 wg2"};
      BasicMove m{Relocation{"wg2g4"}, EnablesEnPassant};
      m.move(pos);

      VERIFY(pos[g4] == Pw, caseLabel);
      VERIFY(pos.locations(Pw).size() == 1, caseLabel);
      VERIFY(pos.locations(Pw)[0] == g4, caseLabel);
      VERIFY(pos.enPassantFile() == fg, caseLabel);
      // Verify uninvolved pieces.
      VERIFY(pos[a1] == Kb, caseLabel);
      VERIFY(pos[h8] == Kw, caseLabel);
   }
}


void testBasicMoveReverse()
{
   {
      const std::string caseLabel = "BasicMove::reverse without taking";

      const Position originalPos{"Kba1 Rba3 Kwh8 wh3"};
      Position pos = originalPos;
      BasicMove m{Relocation{"Rba3c3"}};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "BasicMove::reverse with taking";

      const Position originalPos{"Kba1 Rba3 Kwh8 wh3"};
      Position pos = originalPos;
      BasicMove m{Relocation{"Rba3h3"}, Pw};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "BasicMove::reverse with en-passant";

      Position originalPos{"Kba1 Kwh8 wg2"};
      Position pos = originalPos;
      BasicMove m{Relocation{"wg2g4"}, EnablesEnPassant};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
}


void testBasicMoveEquality()
{
   {
      const std::string caseLabel = "BasicMove equality for equal moves";

      VERIFY(BasicMove(Relocation(Qw, d7, d3)) == BasicMove(Relocation(Qw, d7, d3)),
             caseLabel);
      VERIFY(BasicMove(Relocation(Qw, d7, d3), Rb) ==
                BasicMove(Relocation(Qw, d7, d3), Rb),
             caseLabel);
      VERIFY(BasicMove(Relocation(Pb, e7, e5), EnablesEnPassant) ==
                BasicMove(Relocation(Pb, e7, e5), EnablesEnPassant),
             caseLabel);
   }
   {
      const std::string caseLabel = "BasicMove equality for unequal moves";

      VERIFY(!(BasicMove(Relocation(Qw, d7, d3)) == BasicMove(Relocation(Qw, d7, d4))),
             caseLabel);
      VERIFY(!(BasicMove(Relocation(Qw, d7, d3), Rb) ==
               BasicMove(Relocation(Qw, d7, d3), Nb)),
             caseLabel);
      VERIFY(!(BasicMove(Relocation(Pb, e7, e5), EnablesEnPassant) ==
               BasicMove(Relocation(Pb, e7, e5))),
             caseLabel);
   }
}


void testBasicMoveInequality()
{
   {
      const std::string caseLabel = "BasicMove inequality for equal moves";

      VERIFY(!(BasicMove(Relocation(Qw, d7, d3)) != BasicMove(Relocation(Qw, d7, d3))),
             caseLabel);
      VERIFY(!(BasicMove(Relocation(Qw, d7, d3), Rb) !=
               BasicMove(Relocation(Qw, d7, d3), Rb)),
             caseLabel);
      VERIFY(!(BasicMove(Relocation(Pb, e7, e5), EnablesEnPassant) !=
               BasicMove(Relocation(Pb, e7, e5), EnablesEnPassant)),
             caseLabel);
   }
   {
      const std::string caseLabel = "BasicMove inequality for unequal moves";

      VERIFY(BasicMove(Relocation(Qw, d7, d3)) != BasicMove(Relocation(Qw, d7, d4)),
             caseLabel);
      VERIFY(BasicMove(Relocation(Qw, d7, d3), Rb) !=
                BasicMove(Relocation(Qw, d7, d3), Nb),
             caseLabel);
      VERIFY(BasicMove(Relocation(Pb, e7, e5), EnablesEnPassant) !=
                BasicMove(Relocation(Pb, e7, e5)),
             caseLabel);
   }
}


///////////////////

void testCastlingMove()
{
   {
      const std::string caseLabel = "Castling::move for castling black king-side";

      Position pos{"Kbe8 Rbh8"};
      Castling m{Kingside, Color::Black};
      m.move(pos);

      VERIFY(pos[g8] == Kb, caseLabel);
      VERIFY(pos.locations(Kb).size() == 1, caseLabel);
      VERIFY(pos.locations(Kb)[0] == g8, caseLabel);
      VERIFY(pos[f8] == Rb, caseLabel);
      VERIFY(pos.locations(Rb).size() == 1, caseLabel);
      VERIFY(pos.locations(Rb)[0] == f8, caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "Castling::move for castling white king-side";

      Position pos{"Kwe1 Rwh1"};
      Castling m{Kingside, Color::White};
      m.move(pos);

      VERIFY(pos[g1] == Kw, caseLabel);
      VERIFY(pos.locations(Kw).size() == 1, caseLabel);
      VERIFY(pos.locations(Kw)[0] == g1, caseLabel);
      VERIFY(pos[f1] == Rw, caseLabel);
      VERIFY(pos.locations(Rw).size() == 1, caseLabel);
      VERIFY(pos.locations(Rw)[0] == f1, caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "Castling::move for castling black queen-side";

      Position pos{"Kbe8 Rba8"};
      Castling m{Queenside, Color::Black};
      m.move(pos);

      VERIFY(pos[c8] == Kb, caseLabel);
      VERIFY(pos.locations(Kb).size() == 1, caseLabel);
      VERIFY(pos.locations(Kb)[0] == c8, caseLabel);
      VERIFY(pos[d8] == Rb, caseLabel);
      VERIFY(pos.locations(Rb).size() == 1, caseLabel);
      VERIFY(pos.locations(Rb)[0] == d8, caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "Castling::move for castling white queen-side";

      Position pos{"Kwe1 Rwa1"};
      Castling m{Queenside, Color::White};
      m.move(pos);

      VERIFY(pos[c1] == Kw, caseLabel);
      VERIFY(pos.locations(Kw).size() == 1, caseLabel);
      VERIFY(pos.locations(Kw)[0] == c1, caseLabel);
      VERIFY(pos[d1] == Rw, caseLabel);
      VERIFY(pos.locations(Rw).size() == 1, caseLabel);
      VERIFY(pos.locations(Rw)[0] == d1, caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
}


void testCastlingReverse()
{
   {
      const std::string caseLabel = "Castling::reverse for castling black king-side";

      const Position originalPos{"Kbe8 Rbh8"};
      Position pos = originalPos;
      Castling m{Kingside, Color::Black};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "Castling::move for castling white king-side";

      const Position originalPos{"Kwe1 Rwh1"};
      Position pos = originalPos;
      Castling m{Kingside, Color::White};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "Castling::move for castling black queen-side";

      const Position originalPos{"Kbe8 Rba8"};
      Position pos = originalPos;
      Castling m{Queenside, Color::Black};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "Castling::move for castling white queen-side";

      const Position originalPos{"Kwe1 Rwa1"};
      Position pos = originalPos;
      Castling m{Queenside, Color::White};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
}


void testCastlingEquality()
{
   {
      const std::string caseLabel = "Castling equality for equal moves";

      VERIFY(Castling(Kingside, Color::Black) == Castling(Kingside, Color::Black),
             caseLabel);
      VERIFY(Castling(Queenside, Color::White) == Castling(Queenside, Color::White),
             caseLabel);
   }
   {
      const std::string caseLabel = "Castling equality for unequal moves";

      VERIFY(!(Castling(Kingside, Color::Black) == Castling(Queenside, Color::Black)),
             caseLabel);
      VERIFY(!(Castling(Queenside, Color::White) == Castling(Queenside, Color::Black)),
             caseLabel);
   }
}


void testCastlingInequality()
{
   {
      const std::string caseLabel = "Castling inequality for equal moves";

      VERIFY(!(Castling(Kingside, Color::Black) != Castling(Kingside, Color::Black)),
             caseLabel);
      VERIFY(!(Castling(Queenside, Color::White) != Castling(Queenside, Color::White)),
             caseLabel);
   }
   {
      const std::string caseLabel = "Castling inequality for unequal moves";

      VERIFY(Castling(Kingside, Color::Black) != Castling(Queenside, Color::Black),
             caseLabel);
      VERIFY(Castling(Queenside, Color::White) != Castling(Queenside, Color::Black),
             caseLabel);
   }
}


///////////////////

void testEnPassantMove()
{
   {
      const std::string caseLabel = "EnPassant::move white pawn to higher file";

      // Position: Black pawn just moved from e7 to e5 preventing white from taking it.
      Position pos{"wd5 be5"};
      EnPassant m{Relocation{Pw, d5, e6}};
      m.move(pos);

      VERIFY(pos[d5] == std::nullopt, caseLabel);
      VERIFY(pos[e6] == Pw, caseLabel);
      VERIFY(pos.locations(Pw).size() == 1, caseLabel);
      VERIFY(pos.locations(Pw)[0] == e6, caseLabel);
      VERIFY(pos[e5] == std::nullopt, caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant::move white pawn to lower file";

      // Position: Black pawn just moved from g7 to g5 preventing white from taking it.
      Position pos{"wh5 bg5"};
      EnPassant m{Relocation{Pw, h5, g6}};
      m.move(pos);

      VERIFY(pos[h5] == std::nullopt, caseLabel);
      VERIFY(pos[g6] == Pw, caseLabel);
      VERIFY(pos.locations(Pw).size() == 1, caseLabel);
      VERIFY(pos.locations(Pw)[0] == g6, caseLabel);
      VERIFY(pos[g5] == std::nullopt, caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant::move black pawn to higher file";

      // Position: White pawn just moved from b2 to b4 preventing black from taking it.
      Position pos{"ba4 wb4"};
      EnPassant m{Relocation{Pb, a4, b3}};
      m.move(pos);

      VERIFY(pos[a4] == std::nullopt, caseLabel);
      VERIFY(pos[b3] == Pb, caseLabel);
      VERIFY(pos.locations(Pb).size() == 1, caseLabel);
      VERIFY(pos.locations(Pb)[0] == b3, caseLabel);
      VERIFY(pos[b4] == std::nullopt, caseLabel);
      VERIFY(pos.locations(Pw).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant::move black pawn to lower file";

      // Position: White pawn just moved from e2 to e4 preventing black from taking it.
      Position pos{"bf4 we4"};
      EnPassant m{Relocation{Pb, f4, e3}};
      m.move(pos);

      VERIFY(pos[f4] == std::nullopt, caseLabel);
      VERIFY(pos[e3] == Pb, caseLabel);
      VERIFY(pos.locations(Pb).size() == 1, caseLabel);
      VERIFY(pos.locations(Pb)[0] == e3, caseLabel);
      VERIFY(pos[e4] == std::nullopt, caseLabel);
      VERIFY(pos.locations(Pw).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
}


void testEnPassantReverse()
{
   {
      const std::string caseLabel = "EnPassant::reverse white pawn to higher file";

      // Position: Black pawn just moved from e7 to e5 preventing white from taking it.
      const Position originalPos{"wd5 be5"};
      Position pos = originalPos;
      EnPassant m{Relocation{Pw, d5, e6}};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant::reverse white pawn to lower file";

      // Position: Black pawn just moved from g7 to g5 preventing white from taking it.
      const Position originalPos{"wh5 bg5"};
      Position pos = originalPos;
      EnPassant m{Relocation{Pw, h5, g6}};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant::reverse black pawn to higher file";

      // Position: White pawn just moved from b2 to b4 preventing black from taking it.
      const Position originalPos{"ba4 wb4"};
      Position pos = originalPos;
      EnPassant m{Relocation{Pb, a4, b3}};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant::reverse black pawn to lower file";

      // Position: White pawn just moved from e2 to e4 preventing black from taking it.
      const Position originalPos{"bf4 we4"};
      Position pos = originalPos;
      EnPassant m{Relocation{Pb, f4, e3}};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
}


void testEnPassantEquality()
{
   {
      const std::string caseLabel = "EnPassant equality for equal moves";

      VERIFY(EnPassant(Relocation(Pb, c4, d3)) == EnPassant(Relocation(Pb, c4, d3)),
             caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant equality for unequal moves";

      VERIFY(!(EnPassant(Relocation(Pb, c4, b3)) == EnPassant(Relocation(Pb, c4, d3))),
             caseLabel);
   }
}


void testEnPassantInequality()
{
   {
      const std::string caseLabel = "EnPassant inequality for equal moves";

      VERIFY(!(EnPassant(Relocation(Pb, c4, d3)) != EnPassant(Relocation(Pb, c4, d3))),
             caseLabel);
   }
   {
      const std::string caseLabel = "EnPassant inequality for unequal moves";

      VERIFY(EnPassant(Relocation(Pb, c4, b3)) != EnPassant(Relocation(Pb, c4, d3)),
             caseLabel);
   }
}


///////////////////

void testPromotionMove()
{
   {
      const std::string caseLabel = "Promotion::move promote white pawn to queen";

      Position pos{"wc7"};
      Promotion m{Relocation{"wc7c8"}, Qw};
      m.move(pos);

      VERIFY(pos[c7] == std::nullopt, caseLabel);
      VERIFY(pos[c8] == Qw, caseLabel);
      VERIFY(pos.locations(Qw).size() == 1, caseLabel);
      VERIFY(pos.locations(Qw)[0] == c8, caseLabel);
      VERIFY(pos.locations(Pw).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "Promotion::move promote black pawn to rook";

      Position pos{"bh2"};
      Promotion m{Relocation{"bh2h1"}, Rb};
      m.move(pos);

      VERIFY(pos[h2] == std::nullopt, caseLabel);
      VERIFY(pos[h1] == Rb, caseLabel);
      VERIFY(pos.locations(Rb).size() == 1, caseLabel);
      VERIFY(pos.locations(Rb)[0] == h1, caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel =
         "Promotion::move promote black pawn to queen with taking";

      Position pos{"bh2 Bwg1"};
      Promotion m{Relocation{"bh2g1"}, Qb, Bw};
      m.move(pos);

      VERIFY(pos[h2] == std::nullopt, caseLabel);
      VERIFY(pos[g1] == Qb, caseLabel);
      VERIFY(pos.locations(Qb).size() == 1, caseLabel);
      VERIFY(pos.locations(Qb)[0] == g1, caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
      VERIFY(pos.locations(Bw).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "Promotion::move promote with multiple queens";

      Position pos{"Qbd7 Qba4 bf2"};
      Promotion m{Relocation{"bf2f1"}, Qb};
      m.move(pos);

      VERIFY(pos[f2] == std::nullopt, caseLabel);
      VERIFY(pos[f1] == Qb, caseLabel);
      VERIFY(pos.locations(Qb).size() == 3, caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
   {
      const std::string caseLabel = "Promotion::move promote with max rooks";

      Position pos{"Rwa4 Rwb4 Rwc4 Rwd4 Rwe4 Rwf4 Rwg4 Rwh4 Rwb2 wh7"};
      Promotion m{Relocation{"wh7h8"}, Rw};
      m.move(pos);

      VERIFY(pos[h7] == std::nullopt, caseLabel);
      VERIFY(pos[h8] == Rw, caseLabel);
      VERIFY(pos.locations(Rw).size() == 10, caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
      VERIFY(pos.enPassantFile() == std::nullopt, caseLabel);
   }
}


void testPromotionReverse()
{
   {
      const std::string caseLabel = "Promotion::reverse promote white pawn to queen";

      const Position originalPos{"wc7"};
      Position pos = originalPos;
      Promotion m{Relocation{"wc7c8"}, Qw};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "Promotion::reverse promote black pawn to rook";

      const Position originalPos{"bh2"};
      Position pos = originalPos;
      Promotion m{Relocation{"bh2h1"}, Rb};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel =
         "Promotion::reverse promote black pawn to queen with taking";

      const Position originalPos{"bh2 Bwg1"};
      Position pos = originalPos;
      Promotion m{Relocation{"bh2g1"}, Qb, Bw};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel =
         "Promotion::reverse for promotion with multiple queens";

      const Position originalPos{"Qbd7 Qba4 bf2"};
      Position pos = originalPos;
      Promotion m{Relocation{"bf2f1"}, Qb};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
   {
      const std::string caseLabel = "Promotion::reverse for promotion with max rooks";

      const Position originalPos{"Rwa4 Rwb4 Rwc4 Rwd4 Rwe4 Rwf4 Rwg4 Rwh4 Rwb2 wh7"};
      Position pos = originalPos;
      Promotion m{Relocation{"wh7h8"}, Rw};
      m.move(pos);
      m.reverse(pos);

      VERIFY(pos == originalPos, caseLabel);
   }
}


void testPromotionEquality()
{
   {
      const std::string caseLabel = "Promotion equality for equal moves";

      VERIFY(Promotion(Relocation(Pb, c2, c1), Qb) ==
                Promotion(Relocation(Pb, c2, c1), Qb),
             caseLabel);
      VERIFY(Promotion(Relocation(Pb, c2, d1), Qb, Bw) ==
                Promotion(Relocation(Pb, c2, d1), Qb, Bw),
             caseLabel);
   }
   {
      const std::string caseLabel = "Promotion equality for unequal moves";

      VERIFY(!(Promotion(Relocation(Pb, d2, d1), Qb) ==
               Promotion(Relocation(Pb, c2, c1), Qb)),
             caseLabel);
      VERIFY(!(Promotion(Relocation(Pb, c2, c1), Qb) ==
               Promotion(Relocation(Pb, c2, c1), Rb)),
             caseLabel);
      VERIFY(!(Promotion(Relocation(Pb, c2, d1), Qb, Bw) ==
               Promotion(Relocation(Pb, c2, d1), Qb, Nw)),
             caseLabel);
   }
}


void testPromotionInequality()
{
   {
      const std::string caseLabel = "Promotion inequality for equal moves";

      VERIFY(!(Promotion(Relocation(Pb, c2, c1), Qb) !=
               Promotion(Relocation(Pb, c2, c1), Qb)),
             caseLabel);
      VERIFY(!(Promotion(Relocation(Pb, c2, d1), Qb, Bw) !=
               Promotion(Relocation(Pb, c2, d1), Qb, Bw)),
             caseLabel);
   }
   {
      const std::string caseLabel = "Promotion inequality for unequal moves";

      VERIFY(Promotion(Relocation(Pb, d2, d1), Qb) !=
                Promotion(Relocation(Pb, c2, c1), Qb),
             caseLabel);
      VERIFY(Promotion(Relocation(Pb, c2, c1), Qb) !=
                Promotion(Relocation(Pb, c2, c1), Rb),
             caseLabel);
      VERIFY(Promotion(Relocation(Pb, c2, d1), Qb, Bw) !=
                Promotion(Relocation(Pb, c2, d1), Qb, Nw),
             caseLabel);
   }
}


///////////////////

void testMakeMove()
{
   {
      const std::string caseLabel = "makeMove for basic move";

      Position pos{"Bbe6 wc4"};
      Move m = BasicMove{{Bb, e6, c4}, Pw};
      makeMove(pos, m);

      const Position expected{"Bbc4"};
      VERIFY(pos == expected, caseLabel);
   }
   {
      const std::string caseLabel = "makeMove for castling move";

      Position pos{"Kwe1 Rwh1"};
      Move m = Castling{Kingside, Color::White};
      makeMove(pos, m);

      const Position expected{"Kwg1 Rwf1"};
      VERIFY(pos == expected, caseLabel);
   }
   {
      const std::string caseLabel = "makeMove for en passant move";

      Position pos{"bf4 wg4"};
      Move m = EnPassant{{Pb, f4, g3}};
      makeMove(pos, m);

      const Position expected{"bg3"};
      VERIFY(pos == expected, caseLabel);
   }
   {
      const std::string caseLabel = "makeMove for promotion move";

      Position pos{"wa7"};
      Move m = Promotion{{Pw, a7, a8}, Nw};
      makeMove(pos, m);

      const Position expected{"Nwa8"};
      VERIFY(pos == expected, caseLabel);
   }
}


void testReverseMove()
{
   {
      const std::string caseLabel = "reverseMove for basic move";

      const Position original{"Bbe6 wc4"};
      Position pos = original;
      Move m = BasicMove{{Bb, e6, c4}, Pw};
      makeMove(pos, m);
      reverseMove(pos, m);

      VERIFY(pos == original, caseLabel);
   }
   {
      const std::string caseLabel = "reverseMove for castling move";

      const Position original{"Kwe1 Rwh1"};
      Position pos = original;
      Move m = Castling{Kingside, Color::White};
      makeMove(pos, m);
      reverseMove(pos, m);

      VERIFY(pos == original, caseLabel);
   }
   {
      const std::string caseLabel = "reverseMove for en passant move";

      const Position original{"bf4 wg4"};
      Position pos = original;
      Move m = EnPassant{{Pb, f4, g3}};
      makeMove(pos, m);
      reverseMove(pos, m);

      VERIFY(pos == original, caseLabel);
   }
   {
      const std::string caseLabel = "reverseMove for promotion move";

      const Position original{"wa7"};
      Position pos = original;
      Move m = Promotion{{Pw, a7, a8}, Nw};
      makeMove(pos, m);
      reverseMove(pos, m);

      VERIFY(pos == original, caseLabel);
   }
}

} // namespace


///////////////////

void testMoves()
{
   testBasicMoveMainCtor();
   testBasicMoveEnPassantCtor();
   testBasicMoveMove();
   testBasicMoveReverse();
   testBasicMoveEquality();
   testBasicMoveInequality();
   testCastlingMove();
   testCastlingReverse();
   testCastlingEquality();
   testCastlingInequality();
   testEnPassantMove();
   testEnPassantReverse();
   testEnPassantEquality();
   testEnPassantInequality();
   testPromotionMove();
   testPromotionReverse();
   testPromotionEquality();
   testPromotionInequality();
   testMakeMove();
   testReverseMove();
}
