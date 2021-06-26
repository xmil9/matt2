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
}


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
   testBasicMoveMove();
   testBasicMoveReverse();
   testCastlingMove();
   testCastlingReverse();
   testEnPassantMove();
   testEnPassantReverse();
   testPromotionMove();
   testPromotionReverse();
   testMakeMove();
   testReverseMove();
}
