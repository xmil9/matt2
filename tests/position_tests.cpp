//
// Jun-2021, Michael Lindner
// MIT license
//
#include "position_tests.h"
#include "position.h"
#include "test_util.h"
#include <algorithm>
#include <array>
#include <map>
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

using PieceMap = std::map<Piece, std::vector<Square>>;


bool verifyPieceLocations(Piece piece, const Position& pos, const PieceMap& expected)
{
   auto actualLocs = pos.locations(piece);

   std::vector<Square> expectedLocs;
   if (auto it = expected.find(piece); it != expected.end())
      expectedLocs = it->second;

   return std::is_permutation(std::begin(actualLocs), std::end(actualLocs),
                              std::begin(expectedLocs), std::end(expectedLocs));
}


bool verifyPositionLocations(const Position& pos, const PieceMap& expected)
{
   const std::array<Piece, 12> pieces = {Kw, Qw, Rw, Bw, Nw, Pw, Kb, Qb, Rb, Bb, Nb, Pb};
   for (auto piece : pieces)
      if (!verifyPieceLocations(piece, pos, expected))
         return false;
   return true;
}


///////////////////

void testPositionDefaultCtor()
{
   {
      const std::string caseLabel = "Position default ctor";

      Position pos;

      for (Square at = a1; true; ++at)
      {
         VERIFY(pos[at] == std::nullopt, caseLabel);
         if (at == h8)
            break;
      }

      VERIFY(verifyPositionLocations(pos, {}), caseLabel);
   }
}


void testPositionNotationCtor()
{
   {
      const std::string caseLabel = "Position notation ctor for single placement";

      Position pos{"Kbe8"};
      VERIFY(pos[e8] == Kb, caseLabel);

      PieceMap expectedLocs = {{Kb, {e8}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position notation ctor for single placement with separator at beginning";

      Position pos{" Kbe8"};
      VERIFY(pos[e8] == Kb, caseLabel);

      PieceMap expectedLocs = {{Kb, {e8}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel = "Position notation ctor for single placement with "
                                    "multiple separators at beginning";

      Position pos{"    Kbe8"};
      VERIFY(pos[e8] == Kb, caseLabel);

      PieceMap expectedLocs = {{Kb, {e8}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position notation ctor for single placement with separator at end";

      Position pos{"Kbe8 "};
      VERIFY(pos[e8] == Kb, caseLabel);

      PieceMap expectedLocs = {{Kb, {e8}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position notation ctor for single placement with multiple separators at end";

      Position pos{"Kbe8   "};
      VERIFY(pos[e8] == Kb, caseLabel);

      PieceMap expectedLocs = {{Kb, {e8}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel = "Position notation ctor for multiple placements";

      Position pos{"Kbe8 Kwe1 wa2 bf7"};
      VERIFY(pos[e8] == Kb, caseLabel);
      VERIFY(pos[e1] == Kw, caseLabel);
      VERIFY(pos[a2] == Pw, caseLabel);
      VERIFY(pos[f7] == Pb, caseLabel);

      PieceMap expectedLocs = {{Kb, {e8}}, {Kw, {e1}}, {Pw, {a2}}, {Pb, {f7}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position notation ctor for multiple placements with multiple separators";

      Position pos{"  Kbe8  Kwe1     "};
      VERIFY(pos[e8] == Kb, caseLabel);
      VERIFY(pos[e1] == Kw, caseLabel);

      PieceMap expectedLocs = {{Kb, {e8}}, {Kw, {e1}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel = "Position notation ctor for multiple placements with "
                                    "multiple pieces of same type";

      Position pos{"Kbe8 Kwe1 wa2 bf7 Nwd5 Nwg3 bg7"};
      VERIFY(pos[e8] == Kb, caseLabel);
      VERIFY(pos[e1] == Kw, caseLabel);
      VERIFY(pos[a2] == Pw, caseLabel);
      VERIFY(pos[f7] == Pb, caseLabel);
      VERIFY(pos[d5] == Nw, caseLabel);
      VERIFY(pos[g3] == Nw, caseLabel);
      VERIFY(pos[g7] == Pb, caseLabel);

      PieceMap expectedLocs = {
         {Kb, {e8}}, {Kw, {e1}}, {Pw, {a2}}, {Pb, {f7, g7}}, {Nw, {d5, g3}}};
      VERIFY(verifyPositionLocations(pos, expectedLocs), caseLabel);
   }
   {
      const std::string caseLabel = "Position notation ctor for invalid notation";

      try
      {
         Position pos{"ba7 Gwa1"};
         FAIL("Exception expected.", caseLabel);
      }
      catch (std::runtime_error&)
      {
         // Expected
      }
      catch (...)
      {
         FAIL("Other exception type expected.", caseLabel);
      }
   }
}


void testPositionLocationIndexOperator()
{
   {
      const std::string caseLabel = "Position::operator[](Square)";

      Position pos = StartPos;

      VERIFY(pos[a1] == Rw, caseLabel);
      VERIFY(pos[b1] == Nw, caseLabel);
      VERIFY(pos[c1] == Bw, caseLabel);
      VERIFY(pos[d1] == Qw, caseLabel);
      VERIFY(pos[e1] == Kw, caseLabel);
      VERIFY(pos[f1] == Bw, caseLabel);
      VERIFY(pos[g1] == Nw, caseLabel);
      VERIFY(pos[h1] == Rw, caseLabel);
      VERIFY(pos[a2] == Pw, caseLabel);
      VERIFY(pos[b2] == Pw, caseLabel);
      VERIFY(pos[c2] == Pw, caseLabel);
      VERIFY(pos[d2] == Pw, caseLabel);
      VERIFY(pos[e2] == Pw, caseLabel);
      VERIFY(pos[f2] == Pw, caseLabel);
      VERIFY(pos[g2] == Pw, caseLabel);
      VERIFY(pos[h2] == Pw, caseLabel);
      VERIFY(pos[a3] == std::nullopt, caseLabel);
      VERIFY(pos[b3] == std::nullopt, caseLabel);
      VERIFY(pos[c3] == std::nullopt, caseLabel);
      VERIFY(pos[d3] == std::nullopt, caseLabel);
      VERIFY(pos[e3] == std::nullopt, caseLabel);
      VERIFY(pos[f3] == std::nullopt, caseLabel);
      VERIFY(pos[g3] == std::nullopt, caseLabel);
      VERIFY(pos[h3] == std::nullopt, caseLabel);
      VERIFY(pos[a4] == std::nullopt, caseLabel);
      VERIFY(pos[b4] == std::nullopt, caseLabel);
      VERIFY(pos[c4] == std::nullopt, caseLabel);
      VERIFY(pos[d4] == std::nullopt, caseLabel);
      VERIFY(pos[e4] == std::nullopt, caseLabel);
      VERIFY(pos[f4] == std::nullopt, caseLabel);
      VERIFY(pos[g4] == std::nullopt, caseLabel);
      VERIFY(pos[h4] == std::nullopt, caseLabel);
      VERIFY(pos[a5] == std::nullopt, caseLabel);
      VERIFY(pos[b5] == std::nullopt, caseLabel);
      VERIFY(pos[c5] == std::nullopt, caseLabel);
      VERIFY(pos[d5] == std::nullopt, caseLabel);
      VERIFY(pos[e5] == std::nullopt, caseLabel);
      VERIFY(pos[f5] == std::nullopt, caseLabel);
      VERIFY(pos[g5] == std::nullopt, caseLabel);
      VERIFY(pos[h5] == std::nullopt, caseLabel);
      VERIFY(pos[a6] == std::nullopt, caseLabel);
      VERIFY(pos[b6] == std::nullopt, caseLabel);
      VERIFY(pos[c6] == std::nullopt, caseLabel);
      VERIFY(pos[d6] == std::nullopt, caseLabel);
      VERIFY(pos[e6] == std::nullopt, caseLabel);
      VERIFY(pos[f6] == std::nullopt, caseLabel);
      VERIFY(pos[g6] == std::nullopt, caseLabel);
      VERIFY(pos[h6] == std::nullopt, caseLabel);
      VERIFY(pos[a7] == Pb, caseLabel);
      VERIFY(pos[b7] == Pb, caseLabel);
      VERIFY(pos[c7] == Pb, caseLabel);
      VERIFY(pos[d7] == Pb, caseLabel);
      VERIFY(pos[e7] == Pb, caseLabel);
      VERIFY(pos[f7] == Pb, caseLabel);
      VERIFY(pos[g7] == Pb, caseLabel);
      VERIFY(pos[h7] == Pb, caseLabel);
      VERIFY(pos[a8] == Rb, caseLabel);
      VERIFY(pos[b8] == Nb, caseLabel);
      VERIFY(pos[c8] == Bb, caseLabel);
      VERIFY(pos[d8] == Qb, caseLabel);
      VERIFY(pos[e8] == Kb, caseLabel);
      VERIFY(pos[f8] == Bb, caseLabel);
      VERIFY(pos[g8] == Nb, caseLabel);
      VERIFY(pos[h8] == Rb, caseLabel);
   }
}


void testPositionLocations()
{
   {
      const std::string caseLabel = "Position::locations";

      const PieceMap expected = {
         {Rw, {a1, h1}}, {Nw, {b1, g1}}, {Bw, {c1, f1}},
         {Qw, {d1}},     {Kw, {e1}},     {Pw, {a2, b2, c2, d2, e2, f2, g2, h2}},
         {Rb, {a8, h8}}, {Nb, {b8, g8}}, {Bb, {c8, f8}},
         {Qb, {d8}},     {Kb, {e8}},     {Pb, {a7, b7, c7, d7, e7, f7, g7, h7}}};
      VERIFY(verifyPositionLocations(StartPos, expected), caseLabel);
   }
}


void testPositionAdd()
{
   {
      const std::string caseLabel = "Position::add to empty position";

      Position pos;
      pos.add("Bbe6");
      VERIFY(pos[e6] == Bb, caseLabel);
   }
   {
      const std::string caseLabel = "Position::add to populated position";

      Position pos{"Kwe1 Kbg7 bf6"};
      pos.add("Bbe6");
      VERIFY(pos[e6] == Bb, caseLabel);
   }
   {
      const std::string caseLabel = "Position::add multiple pieces";

      Position pos{"Kwe1 Kbg7 bf6"};
      pos.add("Bbe6");
      pos.add("ba4");
      pos.add("Qwb2");
      VERIFY(pos[e6] == Bb, caseLabel);
      VERIFY(pos[a4] == Pb, caseLabel);
      VERIFY(pos[b2] == Qw, caseLabel);
   }
}


void testPositionRemove()
{
   {
      const std::string caseLabel = "Position::remove from populated position";

      Position pos{"Kwe1 Kbg7 bf6"};
      pos.remove("bf6");
      VERIFY(pos[f6] == std::nullopt, caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
   }
   {
      const std::string caseLabel = "Position::remove last piece";

      Position pos{"Kwe1"};
      pos.remove("Kwe1");
      VERIFY(pos[e1] == std::nullopt, caseLabel);
      VERIFY(pos.locations(Kw).empty(), caseLabel);
   }
   {
      const std::string caseLabel = "Position::remove one of multiple pieces of one type";

      Position pos{"Kwe1 wa2 wb2 wc2"};
      pos.remove("wb2");
      VERIFY(pos[b2] == std::nullopt, caseLabel);
      VERIFY(pos.locations(Pw).size() == 2, caseLabel);
   }
   {
      const std::string caseLabel = "Position::remove multiple pieces";

      Position pos{"Kwe1 Kbg7 bf6 Bwe3 Nbh7 wa5"};
      pos.remove("Kbg7");
      pos.remove("Bwe3");
      pos.remove("bf6");
      pos.remove("wa5");
      VERIFY(pos.locations(Kw).size() == 1, caseLabel);
      VERIFY(pos.locations(Kb).empty(), caseLabel);
      VERIFY(pos.locations(Pb).empty(), caseLabel);
      VERIFY(pos.locations(Bw).empty(), caseLabel);
      VERIFY(pos.locations(Nb).size() == 1, caseLabel);
      VERIFY(pos.locations(Pw).empty(), caseLabel);
   }
}


void testPositionMove()
{
   {
      const std::string caseLabel = "Position::move in populated position";

      Position pos{"Kwe1 Kbg7 bf6"};
      pos.move(Relocation{"bf6f5"});
      VERIFY(pos[f6] == std::nullopt, caseLabel);
      VERIFY(pos[f5] == Pb, caseLabel);
      VERIFY(pos.locations(Pb).size() == 1, caseLabel);
   }
   {
      const std::string caseLabel = "Position::move only piece";

      Position pos{"Kwe1"};
      pos.move(Relocation{"Kwe1d2"});
      VERIFY(pos[e1] == std::nullopt, caseLabel);
      VERIFY(pos[d2] == Kw, caseLabel);
      VERIFY(pos.locations(Kw).size() == 1, caseLabel);
   }
   {
      const std::string caseLabel = "Position::move one of multiple pieces of one type";

      Position pos{"Kwe1 be7 bf7 bg7"};
      pos.move(Relocation{"bf7f5"});
      VERIFY(pos[f7] == std::nullopt, caseLabel);
      VERIFY(pos[f5] == Pb, caseLabel);
      VERIFY(pos.locations(Pb).size() == 3, caseLabel);
   }
   {
      const std::string caseLabel = "Position::move multiple pieces";

      Position pos{"Kwe1 be7 bf7 bg7"};
      pos.move(Relocation{"bf7f5"});
      pos.move(Relocation{"Kwe1f1"});
      VERIFY(pos[f7] == std::nullopt, caseLabel);
      VERIFY(pos[f5] == Pb, caseLabel);
      VERIFY(pos.locations(Pb).size() == 3, caseLabel);
      VERIFY(pos[e1] == std::nullopt, caseLabel);
      VERIFY(pos[f1] == Kw, caseLabel);
      VERIFY(pos.locations(Kw).size() == 1, caseLabel);
   }
}


void testPositionEquality()
{
   {
      const std::string caseLabel =
         "Position equality for positions with different pieces";

      VERIFY(!(Position("Kwe1 Kbe8 Bwg6") == Position("Kwe1 Kbe8 Bwg6 Nbb3")), caseLabel);
      VERIFY(!(Position("be3") == Position("Nwg6")), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position equality for positions same pieces on different squares";

      VERIFY(!(Position("Kwe1 Kbe8 Bwg6") == Position("Kwa1 Kbe8 Bwg6")), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position equality for positions that are permutations of the same pieces";

      VERIFY(Position("Kwe1 Kbe8 Bwg6 wa2 wa3 wa4") ==
                Position("Bwg6 Kwe1 Kbe8 wa4 wa3 wa2"),
             caseLabel);
   }
   {
      const std::string caseLabel = "Position equality for equal positions";

      Position a{"Kwe1 Kbg7 bf6 Bwe3 Nbh7 wa5"};
      VERIFY(a == a, caseLabel);

      Position b = a;
      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel = "Position equality for default position";

      Position a;
      VERIFY(a == a, caseLabel);
   }
   {
      const std::string caseLabel =
         "Position equality for king moved away and back (eliminating castling)";

      const Position unmoved{"Kbe8 Kwe1"};
      Position moved = unmoved;
      moved.move(Relocation{"Kwe1f1"});
      moved.move(Relocation{"Kwf1e1"});

      // We are not considering the game state that is stored with the position when
      // comparing positions, so the positions are equal.
      VERIFY(moved == unmoved, caseLabel);
   }
   {
      const std::string caseLabel =
         "Position equality for kingside rook moved away and back (eliminating castling)";

      const Position unmoved{"Kbe8 Rbh8 Kwe1"};
      Position moved = unmoved;
      moved.move(Relocation{"Rbh8f8"});
      moved.move(Relocation{"Rbf8h8"});

      // We are not considering the game state that is stored with the position when
      // comparing positions, so the positions are equal.
      VERIFY(moved == unmoved, caseLabel);
   }
   {
      const std::string caseLabel = "Position equality for queenside rook moved away and "
                                    "back (eliminating castling)";

      const Position unmoved{"Kbe8 Kwe1 Rwa1"};
      Position moved = unmoved;
      moved.move(Relocation{"Rwa1a6"});
      moved.move(Relocation{"Rwa6a1"});

      // We are not considering the game state that is stored with the position when
      // comparing positions, so the positions are equal.
      VERIFY(moved == unmoved, caseLabel);
   }
}


void testPositionInequality()
{
   {
      const std::string caseLabel = "Position inequality for unequal positions";

      VERIFY(Position("Kwe1 Kbe8 Bwg6") != Position("Kwe1 Kbe8 Bwg6 Nbb3"), caseLabel);
      VERIFY(Position("be3") != Position("Nwg6"), caseLabel);
   }
   {
      const std::string caseLabel = "Position inequality for equal positions";

      Position a{"Kwe1 Kbg7 bf6 Bwe3 Nbh7 wa5"};
      VERIFY(!(a != a), caseLabel);

      Position b = a;
      VERIFY(!(a != b), caseLabel);
   }
}

void testPositionCount()
{
   {
      const std::string caseLabel = "Position::count for white";

      Position pos{"Kwe1 Kbe8 Bwg6"};
      VERIFY(pos.count(Color::White) == 2, caseLabel);
   }
   {
      const std::string caseLabel = "Position::count for black";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      VERIFY(pos.count(Color::Black) == 3, caseLabel);
   }
   {
      const std::string caseLabel = "Position::count for no pieces";

      Position pos{"Kbe8"};
      VERIFY(pos.count(Color::White) == 0, caseLabel);
   }
}

void testPositionBeginForPlacementIterator()
{
   {
      const std::string caseLabel = "Position::begin for white";

      Position pos{"Kwe1 Kbe8 Bwg6"};
      const auto iter = pos.begin(Color::White);
      const auto piece = iter.piece();
      const auto sq = iter.at();

      VERIFY(isWhite(piece), caseLabel);
      VERIFY(isKing(piece) || isBishop(piece), caseLabel);
      VERIFY(sq == e1 || sq == g6, caseLabel);
   }
   {
      const std::string caseLabel = "Position::begin(Side) for black";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      const auto iter = pos.begin(Color::Black);
      const auto piece = iter.piece();
      const auto sq = iter.at();

      VERIFY(isBlack(piece), caseLabel);
      VERIFY(isKing(piece) || isPawn(piece) || isRook(piece), caseLabel);
      VERIFY(sq == e8 || sq == g7 || sq == a2, caseLabel);
   }
   {
      const std::string caseLabel = "Position::begin(Side) for no pieces";

      Position pos{"Kbe8"};
      const auto iter = pos.begin(Color::White);

      VERIFY(iter == pos.end(Color::White), caseLabel);
   }
}

void testPositionEndForPlacementIterator()
{
   {
      const std::string caseLabel = "Position::end(Side) for white";

      Position pos{"Kwe1 Kbe8 Bwg6"};
      const auto end = pos.end(Color::White);

      auto begin = pos.begin(Color::White);
      for (size_t i = 0; i < pos.count(Color::White); ++i)
         ++begin;

      VERIFY(begin == end, caseLabel);
   }
   {
      const std::string caseLabel = "Position::end(Side) for black";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      const auto end = pos.end(Color::Black);

      auto begin = pos.begin(Color::Black);
      for (size_t i = 0; i < pos.count(Color::Black); ++i)
         ++begin;

      VERIFY(begin == end, caseLabel);
   }
}

void testPositionCountForPieces()
{
   {
      const std::string caseLabel = "Position::count(Side, Piece) for white";

      VERIFY(Position("wa2 wg3 wf7").count(Pw) == 3, caseLabel);
      VERIFY(Position("Nwb4").count(Nw) == 1, caseLabel);
      VERIFY(Position("Bwa6 Bwb6").count(Bw) == 2, caseLabel);
      VERIFY(Position("Rwe5 Rwa5").count(Rw) == 2, caseLabel);
      VERIFY(Position("Qwf4").count(Qw) == 1, caseLabel);
      VERIFY(Position("Kwe1").count(Kw) == 1, caseLabel);
   }
   {
      const std::string caseLabel = "Position::count(Side, Piece) for black";

      VERIFY(Position("bb2 bg3 bf7").count(Pb) == 3, caseLabel);
      VERIFY(Position("Nbb4").count(Nb) == 1, caseLabel);
      VERIFY(Position("Bba6 Bbb6").count(Bb) == 2, caseLabel);
      VERIFY(Position("Rbe5 Rba5").count(Rb) == 2, caseLabel);
      VERIFY(Position("Qbf4").count(Qb) == 1, caseLabel);
      VERIFY(Position("Kbe1").count(Kb) == 1, caseLabel);
   }
   {
      const std::string caseLabel = "Position::count(Side, Piece) for no pieces";

      Position pos{"Kbe8 Kwe1"};
      VERIFY(pos.count(Pw) == 0, caseLabel);
      VERIFY(pos.count(Bb) == 0, caseLabel);
   }
}

void testPositionBeginForPieceIterator()
{
   {
      const std::string caseLabel = "Position::begin(side, Piece) for white";

      Position pos{"Kwe1 Kbe8 Bwg6"};
      const auto iter = pos.begin(Bw);

      VERIFY(*iter == g6, caseLabel);
   }
   {
      const std::string caseLabel = "Position::begin(Side, Piece) for black";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      const auto iter = pos.begin(Rb);

      VERIFY(*iter == a2, caseLabel);
   }
   {
      const std::string caseLabel = "Position::begin(Side, Piece) for no pieces";

      Position pos{"Kbe8"};
      const auto iter = pos.begin(Nw);

      VERIFY(iter == pos.end(Nw), caseLabel);
   }
}

void testPositionEndForPieceIterator()
{
   {
      const std::string caseLabel = "Position::end(Side, Piece) for white";

      Position pos{"Kwe1 Kbe8 Bwg6, Bwf6"};
      const auto end = pos.end(Bw);

      auto begin = pos.begin(Bw);
      for (size_t i = 0; i < pos.count(Bw); ++i)
         ++begin;

      VERIFY(begin == end, caseLabel);
   }
   {
      const std::string caseLabel = "Position::end(Side, Piece) for black";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2 bh3 ba4"};
      const auto end = pos.end(Pb);

      auto begin = pos.begin(Pb);
      for (size_t i = 0; i < pos.count(Pb); ++i)
         ++begin;

      VERIFY(begin == end, caseLabel);
   }
}

void testPositionScore()
{
   {
      const std::string caseLabel = "Position::score before score is calculated";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      VERIFY(!pos.score(), caseLabel);
   }
   {
      const std::string caseLabel = "Position::score after score is calculated";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      pos.updateScore();
      VERIFY(pos.score().has_value(), caseLabel);
   }
   {
      const std::string caseLabel = "Position::score for starting position";

      Position pos = StartPos;
      pos.updateScore();
      VERIFY(pos.score().has_value(), caseLabel);
      VERIFY(pos.score() == 0., caseLabel);
   }
}

void testPositionUpdateScore()
{
   {
      const std::string caseLabel = "Position::updateScore for starting position";

      Position pos = StartPos;
      VERIFY(pos.updateScore() == 0., caseLabel);
   }
}

void testPositionEnPassantSquare()
{
   {
      const std::string caseLabel = "Position::enPassantFile not set initially";

      Position pos = StartPos;
      VERIFY(!pos.enPassantSquare(), caseLabel);
   }
   {
      const std::string caseLabel = "Position::enPassantFile";

      Position pos = StartPos;
      pos.setEnPassantSquare(e4);
      VERIFY(pos.enPassantSquare() == e4, caseLabel);
   }
}

void testPositionSetEnPassantFile()
{
   {
      const std::string caseLabel = "Position::setEnPassantFile";

      Position pos = StartPos;
      pos.setEnPassantSquare(a5);
      VERIFY(pos.enPassantSquare() == a5, caseLabel);
   }
}

void testPositionHasKingMoved()
{
   {
      const std::string caseLabel = "Position::hasKingMoved is 'false' initially";

      Position pos = StartPos;
      VERIFY(!pos.hasKingMoved(Color::White), caseLabel);
      VERIFY(!pos.hasKingMoved(Color::Black), caseLabel);
   }
   {
      const std::string caseLabel = "Position::hasKingMoved for white king";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      pos.move(Relocation{"Kwe1e2"});
      VERIFY(pos.hasKingMoved(Color::White), caseLabel);
      VERIFY(!pos.hasKingMoved(Color::Black), caseLabel);
   }
   {
      const std::string caseLabel = "Position::hasKingMoved for black king";

      Position pos{"Kwe1 Kbe8 Bwg6 bg7 Rba2"};
      pos.move(Relocation{"Kbe8f8"});
      VERIFY(!pos.hasKingMoved(Color::White), caseLabel);
      VERIFY(pos.hasKingMoved(Color::Black), caseLabel);
   }
}

void testPositionHasRookMoved()
{
   {
      const std::string caseLabel = "Position::hasRookMoved is 'false' initially";

      Position pos = StartPos;
      VERIFY(!pos.hasRookMoved(Color::White, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::White, false), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::hasRookMoved is 'true' after white king-side rook moved";

      Position pos{"Kwe1 Rwa1 Rwh1 Kbe8 Rba8 Rbh8"};
      pos.move(Relocation{"Rwh1h5"});

      VERIFY(pos.hasRookMoved(Color::White, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::White, false), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::hasRookMoved is 'true' after white queen-side rook moved";

      Position pos{"Kwe1 Rwa1 Rwh1 Kbe8 Rba8 Rbh8"};
      pos.move(Relocation{"Rwa1b1"});

      VERIFY(!pos.hasRookMoved(Color::White, true), caseLabel);
      VERIFY(pos.hasRookMoved(Color::White, false), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::hasRookMoved is 'true' after black king-side rook moved";

      Position pos{"Kwe1 Rwa1 Rwh1 Kbe8 Rba8 Rbh8"};
      pos.move(Relocation{"Rbh8f8"});

      VERIFY(!pos.hasRookMoved(Color::White, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::White, false), caseLabel);
      VERIFY(pos.hasRookMoved(Color::Black, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::hasRookMoved is 'true' after black queen-side rook moved";

      Position pos{"Kwe1 Rwa1 Rwh1 Kbe8 Rba8 Rbh8"};
      pos.move(Relocation{"Rba8a4"});

      VERIFY(!pos.hasRookMoved(Color::White, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::White, false), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::Black, true), caseLabel);
      VERIFY(pos.hasRookMoved(Color::Black, false), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::hasRookMoved is 'true' after multiple rooks moved";

      Position pos{"Kwe1 Rwa1 Rwh1 Kbe8 Rba8 Rbh8"};
      pos.move(Relocation{"Rwh1h5"});
      pos.move(Relocation{"Rbh8f8"});
      pos.move(Relocation{"Rba8a4"});

      VERIFY(pos.hasRookMoved(Color::White, true), caseLabel);
      VERIFY(!pos.hasRookMoved(Color::White, false), caseLabel);
      VERIFY(pos.hasRookMoved(Color::Black, true), caseLabel);
      VERIFY(pos.hasRookMoved(Color::Black, false), caseLabel);
   }
}

void testPositionCanAttackForPlacement()
{
   // King
   {
      const std::string caseLabel = "Position::canAttack with king for empty square";

      Position pos{"Kwe4"};
      VERIFY(pos.canAttack(e5, Placement{"Kwe4"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with king for empty square that cannot be reached";

      Position pos{"Kwe4"};
      VERIFY(!pos.canAttack(e1, Placement{"Kwe4"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with king for square occupied by opponent";

      Position pos{"Kwe4 be5"};
      VERIFY(pos.canAttack(e5, Placement{"Kwe4"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with king for square occupied by own piece";

      Position pos{"Kwe4 we5"};
      VERIFY(!pos.canAttack(e5, Placement{"Kwe4"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with king for square occupied by self";

      Position pos{"Kwe4"};
      VERIFY(!pos.canAttack(e4, Placement{"Kwe4"}), caseLabel);
   }

   // Queen
   {
      const std::string caseLabel = "Position::canAttack with queen for empty square";

      Position pos{"Qbd5"};
      VERIFY(pos.canAttack(d1, Placement{"Qbd5"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with queen for empty square that cannot be reached";

      Position pos{"Qbd5"};
      VERIFY(!pos.canAttack(e1, Placement{"Qbd5"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with queen for square occupied by opponent";

      Position pos{"Qbd5 wd1"};
      VERIFY(pos.canAttack(d1, Placement{"Qbd5"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with queen for square occupied by own piece";

      Position pos{"Qbd5 bd1"};
      VERIFY(!pos.canAttack(d1, Placement{"Qbd5"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with queen for square occupied by self";

      Position pos{"Qbd5"};
      VERIFY(!pos.canAttack(d5, Placement{"Qbd5"}), caseLabel);
   }

   // Rook
   {
      const std::string caseLabel = "Position::canAttack with rook for empty square";

      Position pos{"Rbb3"};
      VERIFY(pos.canAttack(h3, Placement{"Rbb3"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with rook for empty square that cannot be reached";

      Position pos{"Rbb3"};
      VERIFY(!pos.canAttack(h4, Placement{"Rbb3"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with rook for square occupied by opponent";

      Position pos{"Rbb3 wh3"};
      VERIFY(pos.canAttack(h3, Placement{"Rbb3"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with rook for square occupied by own piece";

      Position pos{"Rbb3 bh3"};
      VERIFY(!pos.canAttack(h3, Placement{"Rbb3"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with rook for square occupied by self";

      Position pos{"Rbb3"};
      VERIFY(!pos.canAttack(b3, Placement{"Rbb3"}), caseLabel);
   }

   // Bishop
   {
      const std::string caseLabel = "Position::canAttack with bishop for empty square";

      Position pos{"Bwa1"};
      VERIFY(pos.canAttack(h8, Placement{"Bwa1"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with bishop for empty square that cannot be reached";

      Position pos{"Bwa1"};
      VERIFY(!pos.canAttack(h7, Placement{"Bwa1"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with bishop for square occupied by opponent";

      Position pos{"Bwa1 bh8"};
      VERIFY(pos.canAttack(h8, Placement{"Bwa1"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with bishop for square occupied by own piece";

      Position pos{"Bwa1 wh8"};
      VERIFY(!pos.canAttack(h8, Placement{"Bwa1"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with bishop for square occupied by self";

      Position pos{"Bwa1"};
      VERIFY(!pos.canAttack(a1, Placement{"Bwa1"}), caseLabel);
   }

   // Knight
   {
      const std::string caseLabel = "Position::canAttack with knight for empty square";

      Position pos{"Nbg6"};
      VERIFY(pos.canAttack(e5, Placement{"Nbg6"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with knight for empty square that cannot be reached";

      Position pos{"Nbg6"};
      VERIFY(!pos.canAttack(a1, Placement{"Nbg6"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with knight for square occupied by opponent";

      Position pos{"Nbg6 we5"};
      VERIFY(pos.canAttack(e5, Placement{"Nbg6"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with knight for square occupied by own piece";

      Position pos{"Nbg6 be5"};
      VERIFY(!pos.canAttack(e5, Placement{"Nbg6"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with knight for square occupied by self";

      Position pos{"Nbg6"};
      VERIFY(!pos.canAttack(g6, Placement{"Nbg6"}), caseLabel);
   }

   // Pawn
   {
      const std::string caseLabel = "Position::canAttack with pawn for empty square";

      Position pos{"bh7"};
      VERIFY(pos.canAttack(g6, Placement{"bh7"}), caseLabel);
   }
   {
      const std::string caseLabel = "Position::canAttack with pawn for empty square that "
                                    "be moved to but not attacked";

      Position pos{"bh7"};
      VERIFY(!pos.canAttack(h6, Placement{"Nbg6"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with pawn for empty square that cannot be reached";

      Position pos{"bh7"};
      VERIFY(!pos.canAttack(b2, Placement{"bh7"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with pawn for square occupied by opponent";

      Position pos{"bh7 wg6"};
      VERIFY(pos.canAttack(g6, Placement{"bh7"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with pawn for square occupied by own piece";

      Position pos{"bh7 bg6"};
      VERIFY(!pos.canAttack(g6, Placement{"bh7"}), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position::canAttack with pawn for square occupied by self";

      Position pos{"bh7"};
      VERIFY(!pos.canAttack(h7, Placement{"bh7"}), caseLabel);
   }
   {
      const std::string caseLabel = "Position::canAttack with pawn through en-passant";

      Position pos{"be4 wd2"};
      pos.move(Relocation{"wd2d4"});
      pos.setEnPassantSquare(d4);
      VERIFY(pos.canAttack(d4, Placement{"be4"}), caseLabel);
   }
}

void testPositionCanAttackForColor()
{
   {
      const std::string caseLabel = "Position::canAttack for color with one piece";

      Position pos{"Qwd4"};
      VERIFY(pos.canAttack(d1, Color::White), caseLabel);
      VERIFY(!pos.canAttack(e1, Color::White), caseLabel);
   }
   {
      const std::string caseLabel = "Position::canAttack for color with multiple pieces";

      Position pos{"Qwd4 wf3"};
      // One can attack
      VERIFY(pos.canAttack(d1, Color::White), caseLabel);
      VERIFY(pos.canAttack(g4, Color::White), caseLabel);
      // None can attack
      VERIFY(!pos.canAttack(h5, Color::White), caseLabel);
      // Both can attack
      VERIFY(pos.canAttack(e4, Color::White), caseLabel);
   }
}

///////////////////

void testPlacementIterCopyCtor()
{
   {
      const std::string caseLabel = "PlacementIterator copy ctor";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);

      auto copy{it};

      VERIFY(copy == it, caseLabel);
   }
}

void testPlacementIterMoveCtor()
{
   {
      const std::string caseLabel = "PlacementIterator move ctor";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);
      Placement placement = *it;

      auto movedTo{std::move(it)};

      VERIFY(*movedTo == placement, caseLabel);
   }
}

void testPlacementIterCopyAssignment()
{
   {
      const std::string caseLabel = "PlacementIterator copy assignment";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);

      PlacementIterator copy;
      copy = it;

      VERIFY(copy == it, caseLabel);
   }
}

void testPlacementIterMoveAssignment()
{
   {
      const std::string caseLabel = "PlacementIterator move assignment";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);
      Placement placement = *it;

      PlacementIterator movedTo;
      movedTo = std::move(it);

      VERIFY(*movedTo == placement, caseLabel);
   }
}

void testPlacementIterDereference()
{
   {
      const std::string caseLabel = "PlacementIterator::operator*()";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);

      Placement placement = *it;
      VERIFY(placement == Placement("Qwd4") || placement == Placement("wf3"), caseLabel);
   }
}

void testPlacementIterPrefixInc()
{
   {
      const std::string caseLabel = "PlacementIterator::operator++()";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);

      Placement prev = *it;
      ++it;
      Placement next = *it;

      VERIFY(next != prev && (next == Placement("Qwd4") || next == Placement("wf3")),
             caseLabel);
   }
}

void testPlacementIterPostfixInc()
{
   {
      const std::string caseLabel = "PlacementIterator::operator++(int)";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);

      Placement prev = *it;
      it++;
      Placement next = *it;

      VERIFY(next != prev && (next == Placement("Qwd4") || next == Placement("wf3")),
             caseLabel);
   }
}

void testPlacementIterPrefixDec()
{
   {
      const std::string caseLabel = "PlacementIterator::operator--()";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);
      ++it;

      Placement next = *it;
      --it;
      Placement prev = *it;

      VERIFY(next != prev && (prev == Placement("Qwd4") || prev == Placement("wf3")),
             caseLabel);
   }
}

void testPlacementIterPostfixDec()
{
   {
      const std::string caseLabel = "PlacementIterator::operator--(int)";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);
      ++it;

      Placement next = *it;
      it--;
      Placement prev = *it;

      VERIFY(next != prev && (prev == Placement("Qwd4") || prev == Placement("wf3")),
             caseLabel);
   }
}

void testPlacementIterPieceAccessor()
{
   {
      const std::string caseLabel = "PlacementIterator::piece()";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);
      Placement placement = *it;

      VERIFY(it.piece() == placement.piece(), caseLabel);
   }
}

void testPlacementIterLocationAccessor()
{
   {
      const std::string caseLabel = "PlacementIterator::at()";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Color::White);
      Placement placement = *it;

      VERIFY(it.at() == placement.at(), caseLabel);
   }
}

void testPlacementIterEquality()
{
   {
      const std::string caseLabel = "Placement iterator equality for equal placements";

      Position pos{"Qwd4 wf3"};
      auto a = pos.begin(Color::White);
      auto b = pos.begin(Color::White);

      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Placement iterator equality for placements from different positions";

      Position posA{"Qwd4 wf3"};
      Position posB{"Qwd4 wf3"};
      auto a = posA.begin(Color::White);
      auto b = posB.begin(Color::White);

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Placement iterator equality for placements of different sides";

      Position pos{"Qwd4 bf3"};
      auto a = pos.begin(Color::White);
      auto b = pos.begin(Color::Black);

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Placement iterator equality for placements of different pieces";

      Position pos{"Qwd4 wf3"};
      auto a = pos.begin(Color::White);
      auto b = a;
      ++b;

      VERIFY(!(a == b), caseLabel);
   }
}

void testPlacementIterInequality()
{
   {
      const std::string caseLabel = "Placement iterator inequality for equal placements";

      Position pos{"Qwd4 wf3"};
      auto a = pos.begin(Color::White);
      auto b = pos.begin(Color::White);

      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "Placement iterator inequality for placements from different positions";

      Position posA{"Qwd4 wf3"};
      Position posB{"Qwd4 wf3"};
      auto a = posA.begin(Color::White);
      auto b = posB.begin(Color::White);

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Placement iterator inequality for placements of different sides";

      Position pos{"Qwd4 bf3"};
      auto a = pos.begin(Color::White);
      auto b = pos.begin(Color::Black);

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "Placement iterator inequality for placements of different pieces";

      Position pos{"Qwd4 wf3"};
      auto a = pos.begin(Color::White);
      auto b = a;
      ++b;

      VERIFY(a != b, caseLabel);
   }
}

void testPlacementIterLtComparision()
{
   {
      const std::string caseLabel = "PlacementIterator less-than comparision";

      Position pos{"Qwd4 wf3 Rwa1"};
      auto c = pos.begin(Color::White);
      auto a = c++;
      auto b = c++;

      VERIFY(a < b, caseLabel);
      VERIFY(a < c, caseLabel);
      VERIFY(b < c, caseLabel);
      VERIFY(!(b < a), caseLabel);
      VERIFY(!(c < a), caseLabel);
      VERIFY(!(c < b), caseLabel);
   }
}

void testPlacementIterGtComparision()
{
   {
      const std::string caseLabel = "PlacementIterator greater-than comparision";

      Position pos{"Qwd4 wf3 Rwa1"};
      auto c = pos.begin(Color::White);
      auto a = c++;
      auto b = c++;

      VERIFY(b > a, caseLabel);
      VERIFY(c > a, caseLabel);
      VERIFY(c > b, caseLabel);
      VERIFY(!(a > b), caseLabel);
      VERIFY(!(a > c), caseLabel);
      VERIFY(!(b > c), caseLabel);
   }
}

void testPlacementIterLeComparision()
{
   {
      const std::string caseLabel = "PlacementIterator less-than-or-equal comparision";

      Position pos{"Qwd4 wf3 Rwa1"};
      auto c = pos.begin(Color::White);
      auto a = c++;
      auto b = c++;

      VERIFY(a <= b, caseLabel);
      VERIFY(a <= c, caseLabel);
      VERIFY(b <= c, caseLabel);
      VERIFY(a <= a, caseLabel);
      VERIFY(b <= b, caseLabel);
      VERIFY(c <= c, caseLabel);
      VERIFY(!(b <= a), caseLabel);
      VERIFY(!(c <= a), caseLabel);
      VERIFY(!(c <= b), caseLabel);
   }
}

void testPlacementIterGeComparision()
{
   {
      const std::string caseLabel = "PlacementIterator greater-than-or-equal comparision";

      Position pos{"Kbe2 Bbd7 bb3"};
      auto c = pos.begin(Color::Black);
      auto a = c++;
      auto b = c++;

      VERIFY(b >= a, caseLabel);
      VERIFY(c >= a, caseLabel);
      VERIFY(c >= b, caseLabel);
      VERIFY(a >= a, caseLabel);
      VERIFY(b >= b, caseLabel);
      VERIFY(c >= c, caseLabel);
      VERIFY(!(a >= b), caseLabel);
      VERIFY(!(a >= c), caseLabel);
      VERIFY(!(b >= c), caseLabel);
   }
}

void testPlacementIterSwap()
{
   {
      const std::string caseLabel = "PlacementIterator swap";

      Position posA{"Qwd4 wf3 Rwa1"};
      auto a = posA.begin(Color::White);
      const auto origA = a;
      Position posB{"Kbe2 Bbd7 bb3"};
      auto b = posB.begin(Color::Black);
      const auto origB = b;

      swap(a, b);

      VERIFY(a == origB, caseLabel);
      VERIFY(b == origA, caseLabel);
   }
}

void testPlacementIterIterateAllPlacements()
{
   const std::string caseLabel = "PlacementIterator iterate all placements";

   Position pos{"Qwd4 wf3 Bbh4 Rwa1"};

   auto it = pos.begin(Color::White);
   auto e = pos.end(Color::White);

   std::vector<Placement> iterated;
   for (; it != e; ++it)
      iterated.push_back(*it);

   VERIFY(iterated.size() == pos.count(Color::White), caseLabel);
   VERIFY(std::find(iterated.begin(), iterated.end(), Placement("Qwd4")) !=
             iterated.end(),
          caseLabel);
   VERIFY(std::find(iterated.begin(), iterated.end(), Placement("wf3")) != iterated.end(),
          caseLabel);
   VERIFY(std::find(iterated.begin(), iterated.end(), Placement("Rwa1")) !=
             iterated.end(),
          caseLabel);
}

///////////////////

void testPieceIterCopyCtor()
{
   {
      const std::string caseLabel = "PieceIterator copy ctor";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Pw);

      auto copy{it};

      VERIFY(copy == it, caseLabel);
   }
}

void testPieceIterMoveCtor()
{
   {
      const std::string caseLabel = "PieceIterator move ctor";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Pw);
      Square sq = *it;

      auto movedTo{std::move(it)};

      VERIFY(*movedTo == sq, caseLabel);
   }
}

void testPieceIterCopyAssignment()
{
   {
      const std::string caseLabel = "PieceIterator copy assignment";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Pw);

      PieceIterator copy;
      copy = it;

      VERIFY(copy == it, caseLabel);
   }
}

void testPieceIterMoveAssignment()
{
   {
      const std::string caseLabel = "PieceIterator move assignment";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Pw);
      Square sq = *it;

      PieceIterator movedTo;
      movedTo = std::move(it);

      VERIFY(*movedTo == sq, caseLabel);
   }
}

void testPieceIterDereference()
{
   {
      const std::string caseLabel = "PieceIterator::operator*()";

      Position pos{"Qwd4 wf3"};
      auto it = pos.begin(Pw);

      Square sq = *it;
      VERIFY(sq == f3, caseLabel);
   }
}

void testPieceIterPrefixInc()
{
   {
      const std::string caseLabel = "PieceIterator::operator++()";

      Position pos{"Qwd4 wf3 wb5"};
      auto it = pos.begin(Pw);

      Square prev = *it;
      ++it;
      Square next = *it;

      VERIFY(next != prev && (next == f3 || next == b5), caseLabel);
   }
}

void testPieceIterPostfixInc()
{
   {
      const std::string caseLabel = "PieceIterator::operator++(int)";

      Position pos{"Qwd4 wf3 wb5"};
      auto it = pos.begin(Pw);

      Square prev = *it;
      it++;
      Square next = *it;

      VERIFY(next != prev && (next == f3 || next == b5), caseLabel);
   }
}

void testPieceIterPrefixDec()
{
   {
      const std::string caseLabel = "PieceIterator::operator--()";

      Position pos{"Qwd4 wf3 wb5"};
      auto it = pos.begin(Pw);
      ++it;

      Square next = *it;
      --it;
      Square prev = *it;

      VERIFY(next != prev && (next == f3 || next == b5), caseLabel);
   }
}

void testPieceIterPostfixDec()
{
   {
      const std::string caseLabel = "PieceIterator::operator--(int)";

      Position pos{"Qwd4 wf3 wb5"};
      auto it = pos.begin(Pw);
      ++it;

      Square next = *it;
      it--;
      Square prev = *it;

      VERIFY(next != prev && (next == f3 || next == b5), caseLabel);
   }
}

void testPieceIterEquality()
{
   {
      const std::string caseLabel = "PieceIterator equality for equal pieces";

      Position pos{"Qwd4 wf3"};
      auto a = pos.begin(Pw);
      auto b = pos.begin(Pw);

      VERIFY(a == b, caseLabel);
   }
   {
      const std::string caseLabel =
         "PieceIterator equality for pieces from different positions";

      Position posA{"Qwd4 wf3"};
      Position posB{"Qwd4 wf3"};
      auto a = posA.begin(Pw);
      auto b = posB.begin(Pw);

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel =
         "PieceIterator equality for pieces of different sides";

      Position pos{"Qwd4 wg6 bf3"};
      auto a = pos.begin(Pw);
      auto b = pos.begin(Pb);

      VERIFY(!(a == b), caseLabel);
   }
   {
      const std::string caseLabel = "PieceIterator equality for different pieces";

      Position pos{"Qwd4 wf3 wg6"};
      auto a = pos.begin(Pw);
      auto b = a;
      ++b;

      VERIFY(!(a == b), caseLabel);
   }
}

void testPieceIterInequality()
{
   {
      const std::string caseLabel = "PieceIterator inequality for equal pieces";

      Position pos{"Qwd4 wf3"};
      auto a = pos.begin(Pw);
      auto b = pos.begin(Pw);

      VERIFY(!(a != b), caseLabel);
   }
   {
      const std::string caseLabel =
         "PieceIterator inequality for pieces from different positions";

      Position posA{"Qwd4 wf3"};
      Position posB{"Qwd4 wf3"};
      auto a = posA.begin(Pw);
      auto b = posB.begin(Pw);

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel =
         "PieceIterator inequality for pieces of different sides";

      Position pos{"Qwd4 wg6 bf3"};
      auto a = pos.begin(Pw);
      auto b = pos.begin(Pb);

      VERIFY(a != b, caseLabel);
   }
   {
      const std::string caseLabel = "PieceIterator inequality for different pieces";

      Position pos{"Qwd4 wf3 wg6"};
      auto a = pos.begin(Pw);
      auto b = a;
      ++b;

      VERIFY(a != b, caseLabel);
   }
}

void testPieceIterLtComparision()
{
   {
      const std::string caseLabel = "PieceIterator less-than comparision";

      Position pos{"Qwd4 wf3 wg6 wb2"};
      auto c = pos.begin(Pw);
      auto a = c++;
      auto b = c++;

      VERIFY(a < b, caseLabel);
      VERIFY(a < c, caseLabel);
      VERIFY(b < c, caseLabel);
      VERIFY(!(b < a), caseLabel);
      VERIFY(!(c < a), caseLabel);
      VERIFY(!(c < b), caseLabel);
   }
}

void testPieceIterGtComparision()
{
   {
      const std::string caseLabel = "PieceIterator greater-than comparision";

      Position pos{"Qwd4 wf3 wg6 wb2"};
      auto c = pos.begin(Pw);
      auto a = c++;
      auto b = c++;

      VERIFY(b > a, caseLabel);
      VERIFY(c > a, caseLabel);
      VERIFY(c > b, caseLabel);
      VERIFY(!(a > b), caseLabel);
      VERIFY(!(a > c), caseLabel);
      VERIFY(!(b > c), caseLabel);
   }
}

void testPieceIterLeComparision()
{
   {
      const std::string caseLabel = "PieceIterator less-than-or-equal comparision";

      Position pos{"Qwd4 wf3 wg6 wb2"};
      auto c = pos.begin(Pw);
      auto a = c++;
      auto b = c++;

      VERIFY(a <= b, caseLabel);
      VERIFY(a <= c, caseLabel);
      VERIFY(b <= c, caseLabel);
      VERIFY(a <= a, caseLabel);
      VERIFY(b <= b, caseLabel);
      VERIFY(c <= c, caseLabel);
      VERIFY(!(b <= a), caseLabel);
      VERIFY(!(c <= a), caseLabel);
      VERIFY(!(c <= b), caseLabel);
   }
}

void testPieceIterGeComparision()
{
   {
      const std::string caseLabel = "PieceIterator greater-than-or-equal comparision";

      Position pos{"Kbe2 bf3 bg6 bb2"};
      auto c = pos.begin(Pb);
      auto a = c++;
      auto b = c++;

      VERIFY(b >= a, caseLabel);
      VERIFY(c >= a, caseLabel);
      VERIFY(c >= b, caseLabel);
      VERIFY(a >= a, caseLabel);
      VERIFY(b >= b, caseLabel);
      VERIFY(c >= c, caseLabel);
      VERIFY(!(a >= b), caseLabel);
      VERIFY(!(a >= c), caseLabel);
      VERIFY(!(b >= c), caseLabel);
   }
}

void testPieceIterSwap()
{
   {
      const std::string caseLabel = "PieceIterator swap";

      Position posA{"Qwd4 wf3 Rwa1"};
      auto a = posA.begin(Rw);
      const auto origA = a;
      Position posB{"Kbe2 Bbd7 bb3"};
      auto b = posB.begin(Bb);
      const auto origB = b;

      swap(a, b);

      VERIFY(a == origB, caseLabel);
      VERIFY(b == origA, caseLabel);
   }
}

void testPieceIterIterateAllPieces()
{
   const std::string caseLabel = "PieceIterator iterate all pieces";

   Position pos{"Qwd4 wf3 wg2 wh2 wb7 bh4 Rwa1"};

   auto it = pos.begin(Pw);
   auto e = pos.end(Pw);

   std::vector<Square> iterated;
   for (; it != e; ++it)
      iterated.push_back(*it);

   VERIFY(iterated.size() == 4, caseLabel);
   VERIFY(std::find(iterated.begin(), iterated.end(), f3) != iterated.end(), caseLabel);
   VERIFY(std::find(iterated.begin(), iterated.end(), g2) != iterated.end(), caseLabel);
   VERIFY(std::find(iterated.begin(), iterated.end(), h2) != iterated.end(), caseLabel);
   VERIFY(std::find(iterated.begin(), iterated.end(), b7) != iterated.end(), caseLabel);
}

} // namespace


///////////////////

void testPosition()
{
   testPositionDefaultCtor();
   testPositionNotationCtor();
   testPositionLocationIndexOperator();
   testPositionLocations();
   testPositionAdd();
   testPositionRemove();
   testPositionMove();
   testPositionEquality();
   testPositionInequality();
   testPositionCount();
   testPositionBeginForPlacementIterator();
   testPositionEndForPlacementIterator();
   testPositionCountForPieces();
   testPositionBeginForPieceIterator();
   testPositionEndForPieceIterator();
   testPositionScore();
   testPositionUpdateScore();
   testPositionEnPassantSquare();
   testPositionSetEnPassantFile();
   testPositionHasKingMoved();
   testPositionHasRookMoved();
   testPositionCanAttackForPlacement();
   testPositionCanAttackForColor();
}

void testPlacementIterator()
{
   testPlacementIterCopyCtor();
   testPlacementIterMoveCtor();
   testPlacementIterCopyAssignment();
   testPlacementIterMoveAssignment();
   testPlacementIterDereference();
   testPlacementIterPrefixInc();
   testPlacementIterPostfixInc();
   testPlacementIterPrefixDec();
   testPlacementIterPostfixDec();
   testPlacementIterPieceAccessor();
   testPlacementIterLocationAccessor();
   testPlacementIterEquality();
   testPlacementIterInequality();
   testPlacementIterLtComparision();
   testPlacementIterGtComparision();
   testPlacementIterLeComparision();
   testPlacementIterGeComparision();
   testPlacementIterSwap();
   testPlacementIterIterateAllPlacements();
}

void testPieceIterator()
{
   testPieceIterCopyCtor();
   testPieceIterMoveCtor();
   testPieceIterCopyAssignment();
   testPieceIterMoveAssignment();
   testPieceIterDereference();
   testPieceIterPrefixInc();
   testPieceIterPostfixInc();
   testPieceIterPrefixDec();
   testPieceIterPostfixDec();
   testPieceIterEquality();
   testPieceIterInequality();
   testPieceIterLtComparision();
   testPieceIterGtComparision();
   testPieceIterLeComparision();
   testPieceIterGeComparision();
   testPieceIterSwap();
   testPieceIterIterateAllPieces();
}
