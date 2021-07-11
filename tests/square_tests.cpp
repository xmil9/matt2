//
// Jun-2021, Michael Lindner
// MIT license
//
#include "square_tests.h"
#include "square.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testFileIsValid()
{
   {
      const std::string caseLabel = "isValid(File, int)";

      VERIFY(!isValid(fa, -2), caseLabel);
      VERIFY(!isValid(fa, -1), caseLabel);
      VERIFY(isValid(fa, 0), caseLabel);
      VERIFY(isValid(fa, 1), caseLabel);
      VERIFY(isValid(fa, 2), caseLabel);
      VERIFY(isValid(fa, 3), caseLabel);
      VERIFY(isValid(fa, 4), caseLabel);
      VERIFY(isValid(fa, 5), caseLabel);
      VERIFY(isValid(fa, 6), caseLabel);
      VERIFY(isValid(fa, 7), caseLabel);
      VERIFY(!isValid(fa, 8), caseLabel);
      VERIFY(!isValid(fa, 9), caseLabel);

      VERIFY(!isValid(fh, -8), caseLabel);
      VERIFY(isValid(fh, -2), caseLabel);
      VERIFY(isValid(fh, -1), caseLabel);
      VERIFY(isValid(fh, 0), caseLabel);
      VERIFY(!isValid(fh, 1), caseLabel);
      VERIFY(!isValid(fh, 2), caseLabel);
   }
}


void testFileIncrementOperator()
{
   {
      const std::string caseLabel = "operator+(File, int)";

      VERIFY(fa + 1 == fb, caseLabel);
      VERIFY(fb + 1 == fc, caseLabel);
      VERIFY(fc + 1 == fd, caseLabel);
      VERIFY(fd + 1 == fe, caseLabel);
      VERIFY(fe + 1 == ff, caseLabel);
      VERIFY(ff + 1 == fg, caseLabel);
      VERIFY(fg + 1 == fh, caseLabel);

      VERIFY(fa + 1 == fb, caseLabel);
      VERIFY(fa + 2 == fc, caseLabel);
      VERIFY(fa + 3 == fd, caseLabel);
      VERIFY(fa + 4 == fe, caseLabel);
      VERIFY(fa + 5 == ff, caseLabel);
      VERIFY(fa + 6 == fg, caseLabel);
      VERIFY(fa + 7 == fh, caseLabel);
   }
}


void testFileDecrementOperator()
{
   {
      const std::string caseLabel = "operator-(File, int)";

      VERIFY(fh - 1 == fg, caseLabel);
      VERIFY(fg - 1 == ff, caseLabel);
      VERIFY(ff - 1 == fe, caseLabel);
      VERIFY(fe - 1 == fd, caseLabel);
      VERIFY(fd - 1 == fc, caseLabel);
      VERIFY(fc - 1 == fb, caseLabel);
      VERIFY(fb - 1 == fa, caseLabel);

      VERIFY(fh - 1 == fg, caseLabel);
      VERIFY(fh - 2 == ff, caseLabel);
      VERIFY(fh - 3 == fe, caseLabel);
      VERIFY(fh - 4 == fd, caseLabel);
      VERIFY(fh - 5 == fc, caseLabel);
      VERIFY(fh - 6 == fb, caseLabel);
      VERIFY(fh - 7 == fa, caseLabel);
   }
}


///////////////////

void testRankIsValid()
{
   {
      const std::string caseLabel = "isValid(Rank, int)";

      VERIFY(!isValid(r1, -2), caseLabel);
      VERIFY(!isValid(r1, -1), caseLabel);
      VERIFY(isValid(r1, 0), caseLabel);
      VERIFY(isValid(r1, 1), caseLabel);
      VERIFY(isValid(r1, 2), caseLabel);
      VERIFY(isValid(r1, 3), caseLabel);
      VERIFY(isValid(r1, 4), caseLabel);
      VERIFY(isValid(r1, 5), caseLabel);
      VERIFY(isValid(r1, 6), caseLabel);
      VERIFY(isValid(r1, 7), caseLabel);
      VERIFY(!isValid(r1, 8), caseLabel);
      VERIFY(!isValid(r1, 9), caseLabel);

      VERIFY(!isValid(r8, -8), caseLabel);
      VERIFY(isValid(r8, -2), caseLabel);
      VERIFY(isValid(r8, -1), caseLabel);
      VERIFY(isValid(r8, 0), caseLabel);
      VERIFY(!isValid(r8, 1), caseLabel);
      VERIFY(!isValid(r8, 2), caseLabel);
   }
}


void testRankIncrementOperator()
{
   {
      const std::string caseLabel = "operator+(Rank, int)";

      VERIFY(r1 + 1 == r2, caseLabel);
      VERIFY(r2 + 1 == r3, caseLabel);
      VERIFY(r3 + 1 == r4, caseLabel);
      VERIFY(r4 + 1 == r5, caseLabel);
      VERIFY(r5 + 1 == r6, caseLabel);
      VERIFY(r6 + 1 == r7, caseLabel);
      VERIFY(r7 + 1 == r8, caseLabel);

      VERIFY(r1 + 1 == r2, caseLabel);
      VERIFY(r1 + 2 == r3, caseLabel);
      VERIFY(r1 + 3 == r4, caseLabel);
      VERIFY(r1 + 4 == r5, caseLabel);
      VERIFY(r1 + 5 == r6, caseLabel);
      VERIFY(r1 + 6 == r7, caseLabel);
      VERIFY(r1 + 7 == r8, caseLabel);
   }
}


void testRankDecrementOperator()
{
   {
      const std::string caseLabel = "operator-(Rank, int)";

      VERIFY(r2 - 1 == r1, caseLabel);
      VERIFY(r3 - 1 == r2, caseLabel);
      VERIFY(r4 - 1 == r3, caseLabel);
      VERIFY(r5 - 1 == r4, caseLabel);
      VERIFY(r6 - 1 == r5, caseLabel);
      VERIFY(r7 - 1 == r6, caseLabel);
      VERIFY(r8 - 1 == r7, caseLabel);

      VERIFY(r8 - 1 == r7, caseLabel);
      VERIFY(r8 - 2 == r6, caseLabel);
      VERIFY(r8 - 3 == r5, caseLabel);
      VERIFY(r8 - 4 == r4, caseLabel);
      VERIFY(r8 - 5 == r3, caseLabel);
      VERIFY(r8 - 6 == r2, caseLabel);
      VERIFY(r8 - 7 == r1, caseLabel);
   }
}


///////////////////

void testMakeSquareFromNotation()
{
   {
      const std::string caseLabel = "makeSquare from notation for valid squares";

      VERIFY(makeSquare("a1") == a1, caseLabel);
      VERIFY(makeSquare("a2") == a2, caseLabel);
      VERIFY(makeSquare("a3") == a3, caseLabel);
      VERIFY(makeSquare("a4") == a4, caseLabel);
      VERIFY(makeSquare("a5") == a5, caseLabel);
      VERIFY(makeSquare("a6") == a6, caseLabel);
      VERIFY(makeSquare("a7") == a7, caseLabel);
      VERIFY(makeSquare("a8") == a8, caseLabel);
      VERIFY(makeSquare("b1") == b1, caseLabel);
      VERIFY(makeSquare("b2") == b2, caseLabel);
      VERIFY(makeSquare("b3") == b3, caseLabel);
      VERIFY(makeSquare("b4") == b4, caseLabel);
      VERIFY(makeSquare("b5") == b5, caseLabel);
      VERIFY(makeSquare("b6") == b6, caseLabel);
      VERIFY(makeSquare("b7") == b7, caseLabel);
      VERIFY(makeSquare("b8") == b8, caseLabel);
      VERIFY(makeSquare("c1") == c1, caseLabel);
      VERIFY(makeSquare("c2") == c2, caseLabel);
      VERIFY(makeSquare("c3") == c3, caseLabel);
      VERIFY(makeSquare("c4") == c4, caseLabel);
      VERIFY(makeSquare("c5") == c5, caseLabel);
      VERIFY(makeSquare("c6") == c6, caseLabel);
      VERIFY(makeSquare("c7") == c7, caseLabel);
      VERIFY(makeSquare("c8") == c8, caseLabel);
      VERIFY(makeSquare("d1") == d1, caseLabel);
      VERIFY(makeSquare("d2") == d2, caseLabel);
      VERIFY(makeSquare("d3") == d3, caseLabel);
      VERIFY(makeSquare("d4") == d4, caseLabel);
      VERIFY(makeSquare("d5") == d5, caseLabel);
      VERIFY(makeSquare("d6") == d6, caseLabel);
      VERIFY(makeSquare("d7") == d7, caseLabel);
      VERIFY(makeSquare("d8") == d8, caseLabel);
      VERIFY(makeSquare("e1") == e1, caseLabel);
      VERIFY(makeSquare("e2") == e2, caseLabel);
      VERIFY(makeSquare("e3") == e3, caseLabel);
      VERIFY(makeSquare("e4") == e4, caseLabel);
      VERIFY(makeSquare("e5") == e5, caseLabel);
      VERIFY(makeSquare("e6") == e6, caseLabel);
      VERIFY(makeSquare("e7") == e7, caseLabel);
      VERIFY(makeSquare("e8") == e8, caseLabel);
      VERIFY(makeSquare("f1") == f1, caseLabel);
      VERIFY(makeSquare("f2") == f2, caseLabel);
      VERIFY(makeSquare("f3") == f3, caseLabel);
      VERIFY(makeSquare("f4") == f4, caseLabel);
      VERIFY(makeSquare("f5") == f5, caseLabel);
      VERIFY(makeSquare("f6") == f6, caseLabel);
      VERIFY(makeSquare("f7") == f7, caseLabel);
      VERIFY(makeSquare("f8") == f8, caseLabel);
      VERIFY(makeSquare("g1") == g1, caseLabel);
      VERIFY(makeSquare("g2") == g2, caseLabel);
      VERIFY(makeSquare("g3") == g3, caseLabel);
      VERIFY(makeSquare("g4") == g4, caseLabel);
      VERIFY(makeSquare("g5") == g5, caseLabel);
      VERIFY(makeSquare("g6") == g6, caseLabel);
      VERIFY(makeSquare("g7") == g7, caseLabel);
      VERIFY(makeSquare("g8") == g8, caseLabel);
      VERIFY(makeSquare("h1") == h1, caseLabel);
      VERIFY(makeSquare("h2") == h2, caseLabel);
      VERIFY(makeSquare("h3") == h3, caseLabel);
      VERIFY(makeSquare("h4") == h4, caseLabel);
      VERIFY(makeSquare("h5") == h5, caseLabel);
      VERIFY(makeSquare("h6") == h6, caseLabel);
      VERIFY(makeSquare("h7") == h7, caseLabel);
      VERIFY(makeSquare("h8") == h8, caseLabel);
   }
   {
      const std::string caseLabel = "makeSquare from notation for empty notation";

      try
      {
         makeSquare("");
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
   {
      const std::string caseLabel = "makeSquare from notation for notation with only a file";

      try
      {
         makeSquare("b");
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
   {
      const std::string caseLabel = "makeSquare from notation for notation with only a rank";

      try
      {
         makeSquare("4");
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
   {
      const std::string caseLabel = "makeSquare from notation for invalid file letter";

      try
      {
         makeSquare("k4");
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
   {
      const std::string caseLabel = "makeSquare from notation for capitalized file letter";

      try
      {
         makeSquare("C4");
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
   {
      const std::string caseLabel = "makeSquare from notation for invalid rank notation";

      try
      {
         makeSquare("c9");
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


void testMakeSquareFromFileRank()
{
   {
      const std::string caseLabel = "makeSquare from file/rank";

      VERIFY(makeSquare(fa, r1) == a1, caseLabel);
      VERIFY(makeSquare(fa, r2) == a2, caseLabel);
      VERIFY(makeSquare(fa, r3) == a3, caseLabel);
      VERIFY(makeSquare(fa, r4) == a4, caseLabel);
      VERIFY(makeSquare(fa, r5) == a5, caseLabel);
      VERIFY(makeSquare(fa, r6) == a6, caseLabel);
      VERIFY(makeSquare(fa, r7) == a7, caseLabel);
      VERIFY(makeSquare(fa, r8) == a8, caseLabel);
      VERIFY(makeSquare(fb, r1) == b1, caseLabel);
      VERIFY(makeSquare(fb, r2) == b2, caseLabel);
      VERIFY(makeSquare(fb, r3) == b3, caseLabel);
      VERIFY(makeSquare(fb, r4) == b4, caseLabel);
      VERIFY(makeSquare(fb, r5) == b5, caseLabel);
      VERIFY(makeSquare(fb, r6) == b6, caseLabel);
      VERIFY(makeSquare(fb, r7) == b7, caseLabel);
      VERIFY(makeSquare(fb, r8) == b8, caseLabel);
      VERIFY(makeSquare(fc, r1) == c1, caseLabel);
      VERIFY(makeSquare(fc, r2) == c2, caseLabel);
      VERIFY(makeSquare(fc, r3) == c3, caseLabel);
      VERIFY(makeSquare(fc, r4) == c4, caseLabel);
      VERIFY(makeSquare(fc, r5) == c5, caseLabel);
      VERIFY(makeSquare(fc, r6) == c6, caseLabel);
      VERIFY(makeSquare(fc, r7) == c7, caseLabel);
      VERIFY(makeSquare(fc, r8) == c8, caseLabel);
      VERIFY(makeSquare(fd, r1) == d1, caseLabel);
      VERIFY(makeSquare(fd, r2) == d2, caseLabel);
      VERIFY(makeSquare(fd, r3) == d3, caseLabel);
      VERIFY(makeSquare(fd, r4) == d4, caseLabel);
      VERIFY(makeSquare(fd, r5) == d5, caseLabel);
      VERIFY(makeSquare(fd, r6) == d6, caseLabel);
      VERIFY(makeSquare(fd, r7) == d7, caseLabel);
      VERIFY(makeSquare(fd, r8) == d8, caseLabel);
      VERIFY(makeSquare(fe, r1) == e1, caseLabel);
      VERIFY(makeSquare(fe, r2) == e2, caseLabel);
      VERIFY(makeSquare(fe, r3) == e3, caseLabel);
      VERIFY(makeSquare(fe, r4) == e4, caseLabel);
      VERIFY(makeSquare(fe, r5) == e5, caseLabel);
      VERIFY(makeSquare(fe, r6) == e6, caseLabel);
      VERIFY(makeSquare(fe, r7) == e7, caseLabel);
      VERIFY(makeSquare(fe, r8) == e8, caseLabel);
      VERIFY(makeSquare(ff, r1) == f1, caseLabel);
      VERIFY(makeSquare(ff, r2) == f2, caseLabel);
      VERIFY(makeSquare(ff, r3) == f3, caseLabel);
      VERIFY(makeSquare(ff, r4) == f4, caseLabel);
      VERIFY(makeSquare(ff, r5) == f5, caseLabel);
      VERIFY(makeSquare(ff, r6) == f6, caseLabel);
      VERIFY(makeSquare(ff, r7) == f7, caseLabel);
      VERIFY(makeSquare(ff, r8) == f8, caseLabel);
      VERIFY(makeSquare(fg, r1) == g1, caseLabel);
      VERIFY(makeSquare(fg, r2) == g2, caseLabel);
      VERIFY(makeSquare(fg, r3) == g3, caseLabel);
      VERIFY(makeSquare(fg, r4) == g4, caseLabel);
      VERIFY(makeSquare(fg, r5) == g5, caseLabel);
      VERIFY(makeSquare(fg, r6) == g6, caseLabel);
      VERIFY(makeSquare(fg, r7) == g7, caseLabel);
      VERIFY(makeSquare(fg, r8) == g8, caseLabel);
      VERIFY(makeSquare(fh, r1) == h1, caseLabel);
      VERIFY(makeSquare(fh, r2) == h2, caseLabel);
      VERIFY(makeSquare(fh, r3) == h3, caseLabel);
      VERIFY(makeSquare(fh, r4) == h4, caseLabel);
      VERIFY(makeSquare(fh, r5) == h5, caseLabel);
      VERIFY(makeSquare(fh, r6) == h6, caseLabel);
      VERIFY(makeSquare(fh, r7) == h7, caseLabel);
      VERIFY(makeSquare(fh, r8) == h8, caseLabel);
   }
}


void testGettingFileOfSquare()
{
   {
      const std::string caseLabel = "file(Square)";

      VERIFY(file(a1) == fa, caseLabel);
      VERIFY(file(b2) == fb, caseLabel);
      VERIFY(file(c3) == fc, caseLabel);
      VERIFY(file(d4) == fd, caseLabel);
      VERIFY(file(e5) == fe, caseLabel);
      VERIFY(file(f6) == ff, caseLabel);
      VERIFY(file(g7) == fg, caseLabel);
      VERIFY(file(h8) == fh, caseLabel);
   }
}


void testGettingRankOfSquare()
{
   {
      const std::string caseLabel = "rank(Square)";

      VERIFY(rank(a1) == r1, caseLabel);
      VERIFY(rank(b2) == r2, caseLabel);
      VERIFY(rank(c3) == r3, caseLabel);
      VERIFY(rank(d4) == r4, caseLabel);
      VERIFY(rank(e5) == r5, caseLabel);
      VERIFY(rank(f6) == r6, caseLabel);
      VERIFY(rank(g7) == r7, caseLabel);
      VERIFY(rank(h8) == r8, caseLabel);
   }
}


void testSquareIncrementOperator()
{
   {
      const std::string caseLabel = "operator++(Square&) increases value";

      Square sq = a1;
      VERIFY(++sq == a2 && sq == a2, caseLabel);
      VERIFY(++sq == a3 && sq == a3, caseLabel);
   }
   {
      const std::string caseLabel = "operator++(Square&) increases to next file";

      Square sq = a8;
      VERIFY(++sq == b1 && sq == b1, caseLabel);
   }
   {
      const std::string caseLabel = "operator++(Square&) for h8";

      Square sq = h8;
      VERIFY(++sq == a1 && sq == a1, caseLabel);
   }
}


///////////////////

void testOffsetIsOnBoard()
{
   {
      const std::string caseLabel = "isOnBoard(Square, Offset)";

      VERIFY(!isOnBoard(a1, {-1, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {1, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {2, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {3, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {4, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {5, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {6, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {7, 0}), caseLabel);
      VERIFY(!isOnBoard(a1, {8, 0}), caseLabel);

      VERIFY(!isOnBoard(h8, {-8, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {-7, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {-6, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {-5, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {-4, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {-3, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {-2, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {-1, 0}), caseLabel);
      VERIFY(isOnBoard(h8, {0, 0}), caseLabel);
      VERIFY(!isOnBoard(h8, {1, 0}), caseLabel);

      VERIFY(!isOnBoard(a1, {0, -1}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 0}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 1}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 2}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 3}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 4}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 5}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 6}), caseLabel);
      VERIFY(isOnBoard(a1, {0, 7}), caseLabel);
      VERIFY(!isOnBoard(a1, {0, 8}), caseLabel);

      VERIFY(!isOnBoard(h8, {0, -8}), caseLabel);
      VERIFY(isOnBoard(h8, {0, -7}), caseLabel);
      VERIFY(isOnBoard(h8, {0, -6}), caseLabel);
      VERIFY(isOnBoard(h8, {0, -5}), caseLabel);
      VERIFY(isOnBoard(h8, {0, -4}), caseLabel);
      VERIFY(isOnBoard(h8, {0, -3}), caseLabel);
      VERIFY(isOnBoard(h8, {0, -2}), caseLabel);
      VERIFY(isOnBoard(h8, {0, -1}), caseLabel);
      VERIFY(isOnBoard(h8, {0, 0}), caseLabel);
      VERIFY(!isOnBoard(h8, {0, 1}), caseLabel);


      VERIFY(!isOnBoard(d4, {-3, -3}), caseLabel);
      VERIFY(!isOnBoard(d4, {3, 3}), caseLabel);
      VERIFY(!isOnBoard(d4, {-4, 1}), caseLabel);
      VERIFY(!isOnBoard(d4, {5, 2}), caseLabel);
      VERIFY(!isOnBoard(d4, {2, -4}), caseLabel);
      VERIFY(!isOnBoard(d4, {1, 5}), caseLabel);
   }
}


void testOffsetAdditionToSquare()
{
   {
      const std::string caseLabel = "operator+(Square, Offset)";

      VERIFY((b2 + Offset{1, 3}) == c5, caseLabel);
      VERIFY((c4 + Offset{-2, -3}) == a1, caseLabel);
      VERIFY((c4 + Offset{0, 0}) == c4, caseLabel);
   }
}

} // namespace


///////////////////

void testFile()
{
   testFileIsValid();
   testFileIncrementOperator();
   testFileDecrementOperator();
}


void testRank()
{
   testRankIsValid();
   testRankIncrementOperator();
   testRankDecrementOperator();
}


void testSquare()
{
   testMakeSquareFromNotation();
   testMakeSquareFromFileRank();
   testGettingFileOfSquare();
   testGettingRankOfSquare();
   testSquareIncrementOperator();
}


void testOffset()
{
   testOffsetIsOnBoard();
   testOffsetAdditionToSquare();
}
