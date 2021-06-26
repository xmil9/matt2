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

void testDefaultCtor()
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


void testNotationCtor()
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


void testLocationIndexOperator()
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


void testLocations()
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


void testAdd()
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


void testRemove()
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


void testMove()
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


void testEquality()
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

      VERIFY(Position("Kwe1 Kbe8 Bwg6") == Position("Bwg6 Kwe1 Kbe8"), caseLabel);
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
}


void testInequality()
{
   {
      const std::string caseLabel =
         "Position inequality for positions with different pieces";

      VERIFY(Position("Kwe1 Kbe8 Bwg6") != Position("Kwe1 Kbe8 Bwg6 Nbb3"), caseLabel);
      VERIFY(Position("be3") != Position("Nwg6"), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position inequality for positions same pieces on different squares";

      VERIFY(Position("Kwe1 Kbe8 Bwg6") != Position("Kwa1 Kbe8 Bwg6"), caseLabel);
   }
   {
      const std::string caseLabel =
         "Position inequality for positions that are permutations of the same pieces";

      VERIFY(!(Position("Kwe1 Kbe8 Bwg6") != Position("Bwg6 Kwe1 Kbe8")), caseLabel);
   }
   {
      const std::string caseLabel = "Position inequality for equal positions";

      Position a{"Kwe1 Kbg7 bf6 Bwe3 Nbh7 wa5"};
      VERIFY(!(a != a), caseLabel);

      Position b = a;
      VERIFY(!(a != b), caseLabel);
   }
}

} // namespace


///////////////////

void testPosition()
{
   testDefaultCtor();
   testNotationCtor();
   testLocationIndexOperator();
   testLocations();
   testAdd();
   testRemove();
   testMove();
   testEquality();
   testInequality();
}
