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
      const std::string caseLabel = "isValid(File)";

      VERIFY(!isValid(static_cast<File>(-2)), caseLabel);
      VERIFY(!isValid(static_cast<File>(-1)), caseLabel);
      VERIFY(isValid(static_cast<File>(0)), caseLabel);
      VERIFY(isValid(static_cast<File>(1)), caseLabel);
      VERIFY(isValid(static_cast<File>(2)), caseLabel);
      VERIFY(isValid(static_cast<File>(3)), caseLabel);
      VERIFY(isValid(static_cast<File>(4)), caseLabel);
      VERIFY(isValid(static_cast<File>(5)), caseLabel);
      VERIFY(isValid(static_cast<File>(6)), caseLabel);
      VERIFY(isValid(static_cast<File>(7)), caseLabel);
      VERIFY(!isValid(static_cast<File>(8)), caseLabel);
      VERIFY(!isValid(static_cast<File>(9)), caseLabel);
   }
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


void testFileToLowercaseChar()
{
   {
      const std::string caseLabel = "toLowercaseChar(File)";

      VERIFY(toLowercaseChar(fa) == 'a', caseLabel);
      VERIFY(toLowercaseChar(fb) == 'b', caseLabel);
      VERIFY(toLowercaseChar(fc) == 'c', caseLabel);
      VERIFY(toLowercaseChar(fd) == 'd', caseLabel);
      VERIFY(toLowercaseChar(fe) == 'e', caseLabel);
      VERIFY(toLowercaseChar(ff) == 'f', caseLabel);
      VERIFY(toLowercaseChar(fg) == 'g', caseLabel);
      VERIFY(toLowercaseChar(fh) == 'h', caseLabel);
   }
}

void testFileIsLowerAdjacent()
{
   {
      const std::string caseLabel = "isLowerAdjacent(File, File)";

      VERIFY(!isLowerAdjacent(fa, fa), caseLabel);
      VERIFY(isLowerAdjacent(fa, fb), caseLabel);
      VERIFY(!isLowerAdjacent(fa, fc), caseLabel);
      VERIFY(!isLowerAdjacent(fa, fd), caseLabel);
      VERIFY(!isLowerAdjacent(fa, fe), caseLabel);
      VERIFY(!isLowerAdjacent(fa, ff), caseLabel);
      VERIFY(!isLowerAdjacent(fa, fg), caseLabel);
      VERIFY(!isLowerAdjacent(fa, fh), caseLabel);
      VERIFY(!isLowerAdjacent(fb, fa), caseLabel);
      VERIFY(!isLowerAdjacent(fb, fb), caseLabel);
      VERIFY(isLowerAdjacent(fb, fc), caseLabel);
      VERIFY(!isLowerAdjacent(fb, fd), caseLabel);
      VERIFY(!isLowerAdjacent(fb, fe), caseLabel);
      VERIFY(!isLowerAdjacent(fb, ff), caseLabel);
      VERIFY(!isLowerAdjacent(fb, fg), caseLabel);
      VERIFY(!isLowerAdjacent(fb, fh), caseLabel);
      VERIFY(!isLowerAdjacent(fc, fa), caseLabel);
      VERIFY(!isLowerAdjacent(fc, fb), caseLabel);
      VERIFY(!isLowerAdjacent(fc, fc), caseLabel);
      VERIFY(isLowerAdjacent(fc, fd), caseLabel);
      VERIFY(!isLowerAdjacent(fc, fe), caseLabel);
      VERIFY(!isLowerAdjacent(fc, ff), caseLabel);
      VERIFY(!isLowerAdjacent(fc, fg), caseLabel);
      VERIFY(!isLowerAdjacent(fc, fh), caseLabel);
      VERIFY(!isLowerAdjacent(fd, fa), caseLabel);
      VERIFY(!isLowerAdjacent(fd, fb), caseLabel);
      VERIFY(!isLowerAdjacent(fd, fc), caseLabel);
      VERIFY(!isLowerAdjacent(fd, fd), caseLabel);
      VERIFY(isLowerAdjacent(fd, fe), caseLabel);
      VERIFY(!isLowerAdjacent(fd, ff), caseLabel);
      VERIFY(!isLowerAdjacent(fd, fg), caseLabel);
      VERIFY(!isLowerAdjacent(fd, fh), caseLabel);
      VERIFY(!isLowerAdjacent(fe, fa), caseLabel);
      VERIFY(!isLowerAdjacent(fe, fb), caseLabel);
      VERIFY(!isLowerAdjacent(fe, fc), caseLabel);
      VERIFY(!isLowerAdjacent(fe, fd), caseLabel);
      VERIFY(!isLowerAdjacent(fe, fe), caseLabel);
      VERIFY(isLowerAdjacent(fe, ff), caseLabel);
      VERIFY(!isLowerAdjacent(fe, fg), caseLabel);
      VERIFY(!isLowerAdjacent(fe, fh), caseLabel);
      VERIFY(!isLowerAdjacent(ff, fa), caseLabel);
      VERIFY(!isLowerAdjacent(ff, fb), caseLabel);
      VERIFY(!isLowerAdjacent(ff, fc), caseLabel);
      VERIFY(!isLowerAdjacent(ff, fd), caseLabel);
      VERIFY(!isLowerAdjacent(ff, fe), caseLabel);
      VERIFY(!isLowerAdjacent(ff, ff), caseLabel);
      VERIFY(isLowerAdjacent(ff, fg), caseLabel);
      VERIFY(!isLowerAdjacent(ff, fh), caseLabel);
      VERIFY(!isLowerAdjacent(fg, fa), caseLabel);
      VERIFY(!isLowerAdjacent(fg, fb), caseLabel);
      VERIFY(!isLowerAdjacent(fg, fc), caseLabel);
      VERIFY(!isLowerAdjacent(fg, fd), caseLabel);
      VERIFY(!isLowerAdjacent(fg, fe), caseLabel);
      VERIFY(!isLowerAdjacent(fg, ff), caseLabel);
      VERIFY(!isLowerAdjacent(fg, fg), caseLabel);
      VERIFY(isLowerAdjacent(fg, fh), caseLabel);
      VERIFY(!isLowerAdjacent(fh, fa), caseLabel);
      VERIFY(!isLowerAdjacent(fh, fb), caseLabel);
      VERIFY(!isLowerAdjacent(fh, fc), caseLabel);
      VERIFY(!isLowerAdjacent(fh, fd), caseLabel);
      VERIFY(!isLowerAdjacent(fh, fe), caseLabel);
      VERIFY(!isLowerAdjacent(fh, ff), caseLabel);
      VERIFY(!isLowerAdjacent(fh, fg), caseLabel);
      VERIFY(!isLowerAdjacent(fh, fh), caseLabel);
   }
}

void testFileIsHigherAdjacent()
{
   {
      const std::string caseLabel = "isHigherAdjacent(File, File)";

      VERIFY(!isHigherAdjacent(fa, fa), caseLabel);
      VERIFY(!isHigherAdjacent(fa, fb), caseLabel);
      VERIFY(!isHigherAdjacent(fa, fc), caseLabel);
      VERIFY(!isHigherAdjacent(fa, fd), caseLabel);
      VERIFY(!isHigherAdjacent(fa, fe), caseLabel);
      VERIFY(!isHigherAdjacent(fa, ff), caseLabel);
      VERIFY(!isHigherAdjacent(fa, fg), caseLabel);
      VERIFY(!isHigherAdjacent(fa, fh), caseLabel);
      VERIFY(isHigherAdjacent(fb, fa), caseLabel);
      VERIFY(!isHigherAdjacent(fb, fb), caseLabel);
      VERIFY(!isHigherAdjacent(fb, fc), caseLabel);
      VERIFY(!isHigherAdjacent(fb, fd), caseLabel);
      VERIFY(!isHigherAdjacent(fb, fe), caseLabel);
      VERIFY(!isHigherAdjacent(fb, ff), caseLabel);
      VERIFY(!isHigherAdjacent(fb, fg), caseLabel);
      VERIFY(!isHigherAdjacent(fb, fh), caseLabel);
      VERIFY(!isHigherAdjacent(fc, fa), caseLabel);
      VERIFY(isHigherAdjacent(fc, fb), caseLabel);
      VERIFY(!isHigherAdjacent(fc, fc), caseLabel);
      VERIFY(!isHigherAdjacent(fc, fd), caseLabel);
      VERIFY(!isHigherAdjacent(fc, fe), caseLabel);
      VERIFY(!isHigherAdjacent(fc, ff), caseLabel);
      VERIFY(!isHigherAdjacent(fc, fg), caseLabel);
      VERIFY(!isHigherAdjacent(fc, fh), caseLabel);
      VERIFY(!isHigherAdjacent(fd, fa), caseLabel);
      VERIFY(!isHigherAdjacent(fd, fb), caseLabel);
      VERIFY(isHigherAdjacent(fd, fc), caseLabel);
      VERIFY(!isHigherAdjacent(fd, fd), caseLabel);
      VERIFY(!isHigherAdjacent(fd, fe), caseLabel);
      VERIFY(!isHigherAdjacent(fd, ff), caseLabel);
      VERIFY(!isHigherAdjacent(fd, fg), caseLabel);
      VERIFY(!isHigherAdjacent(fd, fh), caseLabel);
      VERIFY(!isHigherAdjacent(fe, fa), caseLabel);
      VERIFY(!isHigherAdjacent(fe, fb), caseLabel);
      VERIFY(!isHigherAdjacent(fe, fc), caseLabel);
      VERIFY(isHigherAdjacent(fe, fd), caseLabel);
      VERIFY(!isHigherAdjacent(fe, fe), caseLabel);
      VERIFY(!isHigherAdjacent(fe, ff), caseLabel);
      VERIFY(!isHigherAdjacent(fe, fg), caseLabel);
      VERIFY(!isHigherAdjacent(fe, fh), caseLabel);
      VERIFY(!isHigherAdjacent(ff, fa), caseLabel);
      VERIFY(!isHigherAdjacent(ff, fb), caseLabel);
      VERIFY(!isHigherAdjacent(ff, fc), caseLabel);
      VERIFY(!isHigherAdjacent(ff, fd), caseLabel);
      VERIFY(isHigherAdjacent(ff, fe), caseLabel);
      VERIFY(!isHigherAdjacent(ff, ff), caseLabel);
      VERIFY(!isHigherAdjacent(ff, fg), caseLabel);
      VERIFY(!isHigherAdjacent(ff, fh), caseLabel);
      VERIFY(!isHigherAdjacent(fg, fa), caseLabel);
      VERIFY(!isHigherAdjacent(fg, fb), caseLabel);
      VERIFY(!isHigherAdjacent(fg, fc), caseLabel);
      VERIFY(!isHigherAdjacent(fg, fd), caseLabel);
      VERIFY(!isHigherAdjacent(fg, fe), caseLabel);
      VERIFY(isHigherAdjacent(fg, ff), caseLabel);
      VERIFY(!isHigherAdjacent(fg, fg), caseLabel);
      VERIFY(!isHigherAdjacent(fg, fh), caseLabel);
      VERIFY(!isHigherAdjacent(fh, fa), caseLabel);
      VERIFY(!isHigherAdjacent(fh, fb), caseLabel);
      VERIFY(!isHigherAdjacent(fh, fc), caseLabel);
      VERIFY(!isHigherAdjacent(fh, fd), caseLabel);
      VERIFY(!isHigherAdjacent(fh, fe), caseLabel);
      VERIFY(!isHigherAdjacent(fh, ff), caseLabel);
      VERIFY(isHigherAdjacent(fh, fg), caseLabel);
      VERIFY(!isHigherAdjacent(fh, fh), caseLabel);
   }
}

void testFileIsAdjacent()
{
   {
      const std::string caseLabel = "isAdjacent(File, File)";

      VERIFY(!isAdjacent(fa, fa), caseLabel);
      VERIFY(isAdjacent(fa, fb), caseLabel);
      VERIFY(!isAdjacent(fa, fc), caseLabel);
      VERIFY(!isAdjacent(fa, fd), caseLabel);
      VERIFY(!isAdjacent(fa, fe), caseLabel);
      VERIFY(!isAdjacent(fa, ff), caseLabel);
      VERIFY(!isAdjacent(fa, fg), caseLabel);
      VERIFY(!isAdjacent(fa, fh), caseLabel);
      VERIFY(isAdjacent(fb, fa), caseLabel);
      VERIFY(!isAdjacent(fb, fb), caseLabel);
      VERIFY(isAdjacent(fb, fc), caseLabel);
      VERIFY(!isAdjacent(fb, fd), caseLabel);
      VERIFY(!isAdjacent(fb, fe), caseLabel);
      VERIFY(!isAdjacent(fb, ff), caseLabel);
      VERIFY(!isAdjacent(fb, fg), caseLabel);
      VERIFY(!isAdjacent(fb, fh), caseLabel);
      VERIFY(!isAdjacent(fc, fa), caseLabel);
      VERIFY(isAdjacent(fc, fb), caseLabel);
      VERIFY(!isAdjacent(fc, fc), caseLabel);
      VERIFY(isAdjacent(fc, fd), caseLabel);
      VERIFY(!isAdjacent(fc, fe), caseLabel);
      VERIFY(!isAdjacent(fc, ff), caseLabel);
      VERIFY(!isAdjacent(fc, fg), caseLabel);
      VERIFY(!isAdjacent(fc, fh), caseLabel);
      VERIFY(!isAdjacent(fd, fa), caseLabel);
      VERIFY(!isAdjacent(fd, fb), caseLabel);
      VERIFY(isAdjacent(fd, fc), caseLabel);
      VERIFY(!isAdjacent(fd, fd), caseLabel);
      VERIFY(isAdjacent(fd, fe), caseLabel);
      VERIFY(!isAdjacent(fd, ff), caseLabel);
      VERIFY(!isAdjacent(fd, fg), caseLabel);
      VERIFY(!isAdjacent(fd, fh), caseLabel);
      VERIFY(!isAdjacent(fe, fa), caseLabel);
      VERIFY(!isAdjacent(fe, fb), caseLabel);
      VERIFY(!isAdjacent(fe, fc), caseLabel);
      VERIFY(isAdjacent(fe, fd), caseLabel);
      VERIFY(!isAdjacent(fe, fe), caseLabel);
      VERIFY(isAdjacent(fe, ff), caseLabel);
      VERIFY(!isAdjacent(fe, fg), caseLabel);
      VERIFY(!isAdjacent(fe, fh), caseLabel);
      VERIFY(!isAdjacent(ff, fa), caseLabel);
      VERIFY(!isAdjacent(ff, fb), caseLabel);
      VERIFY(!isAdjacent(ff, fc), caseLabel);
      VERIFY(!isAdjacent(ff, fd), caseLabel);
      VERIFY(isAdjacent(ff, fe), caseLabel);
      VERIFY(!isAdjacent(ff, ff), caseLabel);
      VERIFY(isAdjacent(ff, fg), caseLabel);
      VERIFY(!isAdjacent(ff, fh), caseLabel);
      VERIFY(!isAdjacent(fg, fa), caseLabel);
      VERIFY(!isAdjacent(fg, fb), caseLabel);
      VERIFY(!isAdjacent(fg, fc), caseLabel);
      VERIFY(!isAdjacent(fg, fd), caseLabel);
      VERIFY(!isAdjacent(fg, fe), caseLabel);
      VERIFY(isAdjacent(fg, ff), caseLabel);
      VERIFY(!isAdjacent(fg, fg), caseLabel);
      VERIFY(isAdjacent(fg, fh), caseLabel);
      VERIFY(!isAdjacent(fh, fa), caseLabel);
      VERIFY(!isAdjacent(fh, fb), caseLabel);
      VERIFY(!isAdjacent(fh, fc), caseLabel);
      VERIFY(!isAdjacent(fh, fd), caseLabel);
      VERIFY(!isAdjacent(fh, fe), caseLabel);
      VERIFY(!isAdjacent(fh, ff), caseLabel);
      VERIFY(isAdjacent(fh, fg), caseLabel);
      VERIFY(!isAdjacent(fh, fh), caseLabel);
   }
}

void testFileDistance()
{
   {
      const std::string caseLabel = "distance(File, File)";

      VERIFY(distance(fa, fa) == 0, caseLabel);
      VERIFY(distance(fa, fb) == -1, caseLabel);
      VERIFY(distance(fa, fc) == -2, caseLabel);
      VERIFY(distance(fa, fd) == -3, caseLabel);
      VERIFY(distance(fa, fe) == -4, caseLabel);
      VERIFY(distance(fa, ff) == -5, caseLabel);
      VERIFY(distance(fa, fg) == -6, caseLabel);
      VERIFY(distance(fa, fh) == -7, caseLabel);
      VERIFY(distance(fb, fa) == 1, caseLabel);
      VERIFY(distance(fc, fa) == 2, caseLabel);
      VERIFY(distance(fd, fa) == 3, caseLabel);
      VERIFY(distance(fe, fa) == 4, caseLabel);
      VERIFY(distance(ff, fa) == 5, caseLabel);
      VERIFY(distance(fg, fa) == 6, caseLabel);
      VERIFY(distance(fh, fa) == 7, caseLabel);
   }
}

///////////////////

void testRankIsValid()
{
   {
      const std::string caseLabel = "isValid(Rank)";

      VERIFY(!isValid(static_cast<Rank>(-2)), caseLabel);
      VERIFY(!isValid(static_cast<Rank>(-1)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(0)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(1)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(2)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(3)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(4)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(5)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(6)), caseLabel);
      VERIFY(isValid(static_cast<Rank>(7)), caseLabel);
      VERIFY(!isValid(static_cast<Rank>(8)), caseLabel);
      VERIFY(!isValid(static_cast<Rank>(9)), caseLabel);
   }
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

void testRankToLowercaseChar()
{
   {
      const std::string caseLabel = "toLowercaseChar(rank)";

      VERIFY(toLowercaseChar(r1) == '1', caseLabel);
      VERIFY(toLowercaseChar(r2) == '2', caseLabel);
      VERIFY(toLowercaseChar(r3) == '3', caseLabel);
      VERIFY(toLowercaseChar(r4) == '4', caseLabel);
      VERIFY(toLowercaseChar(r5) == '5', caseLabel);
      VERIFY(toLowercaseChar(r6) == '6', caseLabel);
      VERIFY(toLowercaseChar(r7) == '7', caseLabel);
      VERIFY(toLowercaseChar(r8) == '8', caseLabel);
   }
}

void testRankIsLowerAdjacent()
{
   {
      const std::string caseLabel = "isLowerAdjacent(Rank, Rank)";

      VERIFY(!isLowerAdjacent(r1, r1), caseLabel);
      VERIFY(isLowerAdjacent(r1, r2), caseLabel);
      VERIFY(!isLowerAdjacent(r1, r3), caseLabel);
      VERIFY(!isLowerAdjacent(r1, r4), caseLabel);
      VERIFY(!isLowerAdjacent(r1, r5), caseLabel);
      VERIFY(!isLowerAdjacent(r1, r6), caseLabel);
      VERIFY(!isLowerAdjacent(r1, r7), caseLabel);
      VERIFY(!isLowerAdjacent(r1, r8), caseLabel);
      VERIFY(!isLowerAdjacent(r2, r1), caseLabel);
      VERIFY(!isLowerAdjacent(r2, r2), caseLabel);
      VERIFY(isLowerAdjacent(r2, r3), caseLabel);
      VERIFY(!isLowerAdjacent(r2, r4), caseLabel);
      VERIFY(!isLowerAdjacent(r2, r5), caseLabel);
      VERIFY(!isLowerAdjacent(r2, r6), caseLabel);
      VERIFY(!isLowerAdjacent(r2, r7), caseLabel);
      VERIFY(!isLowerAdjacent(r2, r8), caseLabel);
      VERIFY(!isLowerAdjacent(r3, r1), caseLabel);
      VERIFY(!isLowerAdjacent(r3, r2), caseLabel);
      VERIFY(!isLowerAdjacent(r3, r3), caseLabel);
      VERIFY(isLowerAdjacent(r3, r4), caseLabel);
      VERIFY(!isLowerAdjacent(r3, r5), caseLabel);
      VERIFY(!isLowerAdjacent(r3, r6), caseLabel);
      VERIFY(!isLowerAdjacent(r3, r7), caseLabel);
      VERIFY(!isLowerAdjacent(r3, r8), caseLabel);
      VERIFY(!isLowerAdjacent(r4, r1), caseLabel);
      VERIFY(!isLowerAdjacent(r4, r2), caseLabel);
      VERIFY(!isLowerAdjacent(r4, r3), caseLabel);
      VERIFY(!isLowerAdjacent(r4, r4), caseLabel);
      VERIFY(isLowerAdjacent(r4, r5), caseLabel);
      VERIFY(!isLowerAdjacent(r4, r6), caseLabel);
      VERIFY(!isLowerAdjacent(r4, r7), caseLabel);
      VERIFY(!isLowerAdjacent(r4, r8), caseLabel);
      VERIFY(!isLowerAdjacent(r5, r1), caseLabel);
      VERIFY(!isLowerAdjacent(r5, r2), caseLabel);
      VERIFY(!isLowerAdjacent(r5, r3), caseLabel);
      VERIFY(!isLowerAdjacent(r5, r4), caseLabel);
      VERIFY(!isLowerAdjacent(r5, r5), caseLabel);
      VERIFY(isLowerAdjacent(r5, r6), caseLabel);
      VERIFY(!isLowerAdjacent(r5, r7), caseLabel);
      VERIFY(!isLowerAdjacent(r5, r8), caseLabel);
      VERIFY(!isLowerAdjacent(r6, r1), caseLabel);
      VERIFY(!isLowerAdjacent(r6, r2), caseLabel);
      VERIFY(!isLowerAdjacent(r6, r3), caseLabel);
      VERIFY(!isLowerAdjacent(r6, r4), caseLabel);
      VERIFY(!isLowerAdjacent(r6, r5), caseLabel);
      VERIFY(!isLowerAdjacent(r6, r6), caseLabel);
      VERIFY(isLowerAdjacent(r6, r7), caseLabel);
      VERIFY(!isLowerAdjacent(r6, r8), caseLabel);
      VERIFY(!isLowerAdjacent(r7, r1), caseLabel);
      VERIFY(!isLowerAdjacent(r7, r2), caseLabel);
      VERIFY(!isLowerAdjacent(r7, r3), caseLabel);
      VERIFY(!isLowerAdjacent(r7, r4), caseLabel);
      VERIFY(!isLowerAdjacent(r7, r5), caseLabel);
      VERIFY(!isLowerAdjacent(r7, r6), caseLabel);
      VERIFY(!isLowerAdjacent(r7, r7), caseLabel);
      VERIFY(isLowerAdjacent(r7, r8), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r1), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r2), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r3), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r4), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r5), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r6), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r7), caseLabel);
      VERIFY(!isLowerAdjacent(r8, r8), caseLabel);
   }
}

void testRankIsHigherAdjacent()
{
   {
      const std::string caseLabel = "isHigherAdjacent(Rank, Rank)";

      VERIFY(!isHigherAdjacent(r1, r1), caseLabel);
      VERIFY(!isHigherAdjacent(r1, r2), caseLabel);
      VERIFY(!isHigherAdjacent(r1, r3), caseLabel);
      VERIFY(!isHigherAdjacent(r1, r4), caseLabel);
      VERIFY(!isHigherAdjacent(r1, r5), caseLabel);
      VERIFY(!isHigherAdjacent(r1, r6), caseLabel);
      VERIFY(!isHigherAdjacent(r1, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r1, r8), caseLabel);
      VERIFY(isHigherAdjacent(r2, r1), caseLabel);
      VERIFY(!isHigherAdjacent(r2, r2), caseLabel);
      VERIFY(!isHigherAdjacent(r2, r3), caseLabel);
      VERIFY(!isHigherAdjacent(r2, r4), caseLabel);
      VERIFY(!isHigherAdjacent(r2, r5), caseLabel);
      VERIFY(!isHigherAdjacent(r2, r6), caseLabel);
      VERIFY(!isHigherAdjacent(r2, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r2, r8), caseLabel);
      VERIFY(!isHigherAdjacent(r3, r1), caseLabel);
      VERIFY(isHigherAdjacent(r3, r2), caseLabel);
      VERIFY(!isHigherAdjacent(r3, r3), caseLabel);
      VERIFY(!isHigherAdjacent(r3, r4), caseLabel);
      VERIFY(!isHigherAdjacent(r3, r5), caseLabel);
      VERIFY(!isHigherAdjacent(r3, r6), caseLabel);
      VERIFY(!isHigherAdjacent(r3, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r3, r8), caseLabel);
      VERIFY(!isHigherAdjacent(r4, r1), caseLabel);
      VERIFY(!isHigherAdjacent(r4, r2), caseLabel);
      VERIFY(isHigherAdjacent(r4, r3), caseLabel);
      VERIFY(!isHigherAdjacent(r4, r4), caseLabel);
      VERIFY(!isHigherAdjacent(r4, r5), caseLabel);
      VERIFY(!isHigherAdjacent(r4, r6), caseLabel);
      VERIFY(!isHigherAdjacent(r4, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r4, r8), caseLabel);
      VERIFY(!isHigherAdjacent(r5, r1), caseLabel);
      VERIFY(!isHigherAdjacent(r5, r2), caseLabel);
      VERIFY(!isHigherAdjacent(r5, r3), caseLabel);
      VERIFY(isHigherAdjacent(r5, r4), caseLabel);
      VERIFY(!isHigherAdjacent(r5, r5), caseLabel);
      VERIFY(!isHigherAdjacent(r5, r6), caseLabel);
      VERIFY(!isHigherAdjacent(r5, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r5, r8), caseLabel);
      VERIFY(!isHigherAdjacent(r6, r1), caseLabel);
      VERIFY(!isHigherAdjacent(r6, r2), caseLabel);
      VERIFY(!isHigherAdjacent(r6, r3), caseLabel);
      VERIFY(!isHigherAdjacent(r6, r4), caseLabel);
      VERIFY(isHigherAdjacent(r6, r5), caseLabel);
      VERIFY(!isHigherAdjacent(r6, r6), caseLabel);
      VERIFY(!isHigherAdjacent(r6, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r6, r8), caseLabel);
      VERIFY(!isHigherAdjacent(r7, r1), caseLabel);
      VERIFY(!isHigherAdjacent(r7, r2), caseLabel);
      VERIFY(!isHigherAdjacent(r7, r3), caseLabel);
      VERIFY(!isHigherAdjacent(r7, r4), caseLabel);
      VERIFY(!isHigherAdjacent(r7, r5), caseLabel);
      VERIFY(isHigherAdjacent(r7, r6), caseLabel);
      VERIFY(!isHigherAdjacent(r7, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r7, r8), caseLabel);
      VERIFY(!isHigherAdjacent(r8, r1), caseLabel);
      VERIFY(!isHigherAdjacent(r8, r2), caseLabel);
      VERIFY(!isHigherAdjacent(r8, r3), caseLabel);
      VERIFY(!isHigherAdjacent(r8, r4), caseLabel);
      VERIFY(!isHigherAdjacent(r8, r5), caseLabel);
      VERIFY(!isHigherAdjacent(r8, r6), caseLabel);
      VERIFY(isHigherAdjacent(r8, r7), caseLabel);
      VERIFY(!isHigherAdjacent(r8, r8), caseLabel);
   }
}

void testRankIsAdjacent()
{
   {
      const std::string caseLabel = "isAdjacent(Rank, Rank)";

      VERIFY(!isAdjacent(r1, r1), caseLabel);
      VERIFY(isAdjacent(r1, r2), caseLabel);
      VERIFY(!isAdjacent(r1, r3), caseLabel);
      VERIFY(!isAdjacent(r1, r4), caseLabel);
      VERIFY(!isAdjacent(r1, r5), caseLabel);
      VERIFY(!isAdjacent(r1, r6), caseLabel);
      VERIFY(!isAdjacent(r1, r7), caseLabel);
      VERIFY(!isAdjacent(r1, r8), caseLabel);
      VERIFY(isAdjacent(r2, r1), caseLabel);
      VERIFY(!isAdjacent(r2, r2), caseLabel);
      VERIFY(isAdjacent(r2, r3), caseLabel);
      VERIFY(!isAdjacent(r2, r4), caseLabel);
      VERIFY(!isAdjacent(r2, r5), caseLabel);
      VERIFY(!isAdjacent(r2, r6), caseLabel);
      VERIFY(!isAdjacent(r2, r7), caseLabel);
      VERIFY(!isAdjacent(r2, r8), caseLabel);
      VERIFY(!isAdjacent(r3, r1), caseLabel);
      VERIFY(isAdjacent(r3, r2), caseLabel);
      VERIFY(!isAdjacent(r3, r3), caseLabel);
      VERIFY(isAdjacent(r3, r4), caseLabel);
      VERIFY(!isAdjacent(r3, r5), caseLabel);
      VERIFY(!isAdjacent(r3, r6), caseLabel);
      VERIFY(!isAdjacent(r3, r7), caseLabel);
      VERIFY(!isAdjacent(r3, r8), caseLabel);
      VERIFY(!isAdjacent(r4, r1), caseLabel);
      VERIFY(!isAdjacent(r4, r2), caseLabel);
      VERIFY(isAdjacent(r4, r3), caseLabel);
      VERIFY(!isAdjacent(r4, r4), caseLabel);
      VERIFY(isAdjacent(r4, r5), caseLabel);
      VERIFY(!isAdjacent(r4, r6), caseLabel);
      VERIFY(!isAdjacent(r4, r7), caseLabel);
      VERIFY(!isAdjacent(r4, r8), caseLabel);
      VERIFY(!isAdjacent(r5, r1), caseLabel);
      VERIFY(!isAdjacent(r5, r2), caseLabel);
      VERIFY(!isAdjacent(r5, r3), caseLabel);
      VERIFY(isAdjacent(r5, r4), caseLabel);
      VERIFY(!isAdjacent(r5, r5), caseLabel);
      VERIFY(isAdjacent(r5, r6), caseLabel);
      VERIFY(!isAdjacent(r5, r7), caseLabel);
      VERIFY(!isAdjacent(r5, r8), caseLabel);
      VERIFY(!isAdjacent(r6, r1), caseLabel);
      VERIFY(!isAdjacent(r6, r2), caseLabel);
      VERIFY(!isAdjacent(r6, r3), caseLabel);
      VERIFY(!isAdjacent(r6, r4), caseLabel);
      VERIFY(isAdjacent(r6, r5), caseLabel);
      VERIFY(!isAdjacent(r6, r6), caseLabel);
      VERIFY(isAdjacent(r6, r7), caseLabel);
      VERIFY(!isAdjacent(r6, r8), caseLabel);
      VERIFY(!isAdjacent(r7, r1), caseLabel);
      VERIFY(!isAdjacent(r7, r2), caseLabel);
      VERIFY(!isAdjacent(r7, r3), caseLabel);
      VERIFY(!isAdjacent(r7, r4), caseLabel);
      VERIFY(!isAdjacent(r7, r5), caseLabel);
      VERIFY(isAdjacent(r7, r6), caseLabel);
      VERIFY(!isAdjacent(r7, r7), caseLabel);
      VERIFY(isAdjacent(r7, r8), caseLabel);
      VERIFY(!isAdjacent(r8, r1), caseLabel);
      VERIFY(!isAdjacent(r8, r2), caseLabel);
      VERIFY(!isAdjacent(r8, r3), caseLabel);
      VERIFY(!isAdjacent(r8, r4), caseLabel);
      VERIFY(!isAdjacent(r8, r5), caseLabel);
      VERIFY(!isAdjacent(r8, r6), caseLabel);
      VERIFY(isAdjacent(r8, r7), caseLabel);
      VERIFY(!isAdjacent(r8, r8), caseLabel);
   }
}

void testRankDistance()
{
   {
      const std::string caseLabel = "distance(Rank, Rank)";

      VERIFY(distance(r1, r1) == 0, caseLabel);
      VERIFY(distance(r1, r2) == -1, caseLabel);
      VERIFY(distance(r1, r3) == -2, caseLabel);
      VERIFY(distance(r1, r4) == -3, caseLabel);
      VERIFY(distance(r1, r5) == -4, caseLabel);
      VERIFY(distance(r1, r6) == -5, caseLabel);
      VERIFY(distance(r1, r7) == -6, caseLabel);
      VERIFY(distance(r1, r8) == -7, caseLabel);
      VERIFY(distance(r2, r1) == 1, caseLabel);
      VERIFY(distance(r3, r1) == 2, caseLabel);
      VERIFY(distance(r4, r1) == 3, caseLabel);
      VERIFY(distance(r5, r1) == 4, caseLabel);
      VERIFY(distance(r6, r1) == 5, caseLabel);
      VERIFY(distance(r7, r1) == 6, caseLabel);
      VERIFY(distance(r8, r1) == 7, caseLabel);
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
      const std::string caseLabel =
         "makeSquare from notation for notation with only a file";

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
      const std::string caseLabel =
         "makeSquare from notation for notation with only a rank";

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
      const std::string caseLabel =
         "makeSquare from notation for capitalized file letter";

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

void testSquareIsValid()
{
   {
      const std::string caseLabel = "isValid(Square)";

      VERIFY(!isValid(static_cast<Square>(-1)), caseLabel);
      for (int i = 0; i < 64; ++i)
         VERIFY(isValid(static_cast<Square>(i)), caseLabel);
      VERIFY(!isValid(static_cast<Square>(65)), caseLabel);
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

void testUpDiagonal()
{
   {
      const std::string caseLabel = "upDiagonal(Square)";

      VERIFY(upDiagonal(a1) == upDiagonal(b2), caseLabel);
      VERIFY(upDiagonal(a1) == upDiagonal(c3), caseLabel);
      VERIFY(upDiagonal(a1) == upDiagonal(d4), caseLabel);
      VERIFY(upDiagonal(a1) == upDiagonal(e5), caseLabel);
      VERIFY(upDiagonal(a1) == upDiagonal(f6), caseLabel);
      VERIFY(upDiagonal(a1) == upDiagonal(g7), caseLabel);
      VERIFY(upDiagonal(a1) == upDiagonal(h8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(b1), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(c1), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(d1), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(e1), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(f1), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(g1), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(h1), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(a8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(b8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(c8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(d8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(e8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(f8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(g8), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(b7), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(d6), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(f5), caseLabel);
      VERIFY(upDiagonal(a1) != upDiagonal(h3), caseLabel);

      VERIFY(upDiagonal(c7) == upDiagonal(a5), caseLabel);
      VERIFY(upDiagonal(c7) == upDiagonal(b6), caseLabel);
      VERIFY(upDiagonal(c7) == upDiagonal(d8), caseLabel);
      VERIFY(upDiagonal(c7) != upDiagonal(b5), caseLabel);
      VERIFY(upDiagonal(c7) != upDiagonal(c6), caseLabel);
      VERIFY(upDiagonal(c7) != upDiagonal(d7), caseLabel);
      VERIFY(upDiagonal(c7) != upDiagonal(e4), caseLabel);
      VERIFY(upDiagonal(c7) != upDiagonal(g1), caseLabel);
      VERIFY(upDiagonal(c7) != upDiagonal(h2), caseLabel);
   }
}

void testDownDiagonal()
{
   {
      const std::string caseLabel = "downDiagonal(Square)";

      VERIFY(downDiagonal(a8) == downDiagonal(b7), caseLabel);
      VERIFY(downDiagonal(a8) == downDiagonal(c6), caseLabel);
      VERIFY(downDiagonal(a8) == downDiagonal(d5), caseLabel);
      VERIFY(downDiagonal(a8) == downDiagonal(e4), caseLabel);
      VERIFY(downDiagonal(a8) == downDiagonal(f3), caseLabel);
      VERIFY(downDiagonal(a8) == downDiagonal(g2), caseLabel);
      VERIFY(downDiagonal(a8) == downDiagonal(h1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(b8), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(c8), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(d8), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(e8), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(f8), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(g8), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(h8), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(a1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(b1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(c1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(d1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(e1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(f1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(g1), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(b3), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(d2), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(f4), caseLabel);
      VERIFY(downDiagonal(a8) != downDiagonal(h5), caseLabel);

      VERIFY(downDiagonal(d2) == downDiagonal(a5), caseLabel);
      VERIFY(downDiagonal(d2) == downDiagonal(b4), caseLabel);
      VERIFY(downDiagonal(d2) == downDiagonal(c3), caseLabel);
      VERIFY(downDiagonal(d2) == downDiagonal(e1), caseLabel);
      VERIFY(downDiagonal(d2) != downDiagonal(b5), caseLabel);
      VERIFY(downDiagonal(d2) != downDiagonal(c6), caseLabel);
      VERIFY(downDiagonal(d2) != downDiagonal(d7), caseLabel);
      VERIFY(downDiagonal(d2) != downDiagonal(e4), caseLabel);
      VERIFY(downDiagonal(d2) != downDiagonal(g1), caseLabel);
      VERIFY(downDiagonal(d2) != downDiagonal(h2), caseLabel);
   }
}

void testOnSameUpDiagonal()
{
   {
      const std::string caseLabel = "onSameUpDiagonal(Square, Square)";

      VERIFY(onSameUpDiagonal(a1, a1), caseLabel);
      VERIFY(onSameUpDiagonal(a1, b2), caseLabel);
      VERIFY(onSameUpDiagonal(a1, c3), caseLabel);
      VERIFY(onSameUpDiagonal(a1, d4), caseLabel);
      VERIFY(onSameUpDiagonal(a1, e5), caseLabel);
      VERIFY(onSameUpDiagonal(a1, f6), caseLabel);
      VERIFY(onSameUpDiagonal(a1, g7), caseLabel);
      VERIFY(onSameUpDiagonal(a1, h8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, b1), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, c1), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, d1), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, e1), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, f1), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, g1), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, h1), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, a8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, b8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, c8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, d8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, e8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, f8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, g8), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, b7), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, d6), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, f5), caseLabel);
      VERIFY(!onSameUpDiagonal(a1, h3), caseLabel);

      VERIFY(onSameUpDiagonal(c7, a5), caseLabel);
      VERIFY(onSameUpDiagonal(c7, b6), caseLabel);
      VERIFY(onSameUpDiagonal(c7, d8), caseLabel);
      VERIFY(!onSameUpDiagonal(c7, b5), caseLabel);
      VERIFY(!onSameUpDiagonal(c7, c6), caseLabel);
      VERIFY(!onSameUpDiagonal(c7, d7), caseLabel);
      VERIFY(!onSameUpDiagonal(c7, e4), caseLabel);
      VERIFY(!onSameUpDiagonal(c7, g1), caseLabel);
      VERIFY(!onSameUpDiagonal(c7, h2), caseLabel);
   }
}

void testOnSameDownDiagonal()
{
   {
      const std::string caseLabel = "onSameDownDiagonal(Square, Square)";

      VERIFY(onSameDownDiagonal(a8, a8), caseLabel);
      VERIFY(onSameDownDiagonal(a8, b7), caseLabel);
      VERIFY(onSameDownDiagonal(a8, c6), caseLabel);
      VERIFY(onSameDownDiagonal(a8, d5), caseLabel);
      VERIFY(onSameDownDiagonal(a8, e4), caseLabel);
      VERIFY(onSameDownDiagonal(a8, f3), caseLabel);
      VERIFY(onSameDownDiagonal(a8, g2), caseLabel);
      VERIFY(onSameDownDiagonal(a8, h1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, b8), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, c8), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, d8), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, e8), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, f8), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, g8), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, h8), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, a1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, b1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, c1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, d1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, e1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, f1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, g1), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, b3), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, d2), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, f4), caseLabel);
      VERIFY(!onSameDownDiagonal(a8, h5), caseLabel);

      VERIFY(onSameDownDiagonal(d2, a5), caseLabel);
      VERIFY(onSameDownDiagonal(d2, b4), caseLabel);
      VERIFY(onSameDownDiagonal(d2, c3), caseLabel);
      VERIFY(onSameDownDiagonal(d2, e1), caseLabel);
      VERIFY(!onSameDownDiagonal(d2, b5), caseLabel);
      VERIFY(!onSameDownDiagonal(d2, c6), caseLabel);
      VERIFY(!onSameDownDiagonal(d2, d7), caseLabel);
      VERIFY(!onSameDownDiagonal(d2, e4), caseLabel);
      VERIFY(!onSameDownDiagonal(d2, g1), caseLabel);
      VERIFY(!onSameDownDiagonal(d2, h2), caseLabel);
   }
}

void testOnSameDiagonal()
{
   {
      const std::string caseLabel = "onSameDiagonal(Square, Square)";

      VERIFY(onSameDiagonal(d5, d5), caseLabel);
      VERIFY(onSameDiagonal(d5, e4), caseLabel);
      VERIFY(onSameDiagonal(d5, e6), caseLabel);
      VERIFY(!onSameDiagonal(d5, e5), caseLabel);
      VERIFY(!onSameDiagonal(d5, d4), caseLabel);
   }
}

///////////////////

void testOffsetEquality()
{
   {
      const std::string caseLabel = "operator==(Offset, Offset)";

      VERIFY(Offset(3, -5) == Offset(3, -5), caseLabel);
      VERIFY(!(Offset(3, 3) == Offset(3, 4)), caseLabel);
      VERIFY(!(Offset(3, 3) == Offset(1, 3)), caseLabel);
   }
}

void testOffsetInequality()
{
   {
      const std::string caseLabel = "operator!=(Offset, Offset)";

      VERIFY(!(Offset(3, -5) != Offset(3, -5)), caseLabel);
      VERIFY(Offset(3, 3) != Offset(3, 4), caseLabel);
      VERIFY(Offset(3, 3) != Offset(1, 3), caseLabel);
   }
}

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

      VERIFY(isOnBoard(d4, {-3, -3}), caseLabel);
      VERIFY(isOnBoard(d4, {3, 3}), caseLabel);
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

void testOffsetOffset()
{
   {
      const std::string caseLabel = "offset(Square, Square)";

      VERIFY(offset(a1, h8) == Offset(-7, -7), caseLabel);
      VERIFY(offset(h8, a1) == Offset(7, 7), caseLabel);
      VERIFY(offset(d4, d4) == Offset(0, 0), caseLabel);
      VERIFY(offset(b7, e3) == Offset(-3, 4), caseLabel);
   }
}

void testOffsetMinDistance()
{
   {
      const std::string caseLabel = "minDistance(Square, Square)";

      VERIFY(minDistance(a1, h8) == 7, caseLabel);
      VERIFY(minDistance(h8, a1) == 7, caseLabel);
      VERIFY(minDistance(a1, a1) == 0, caseLabel);
      VERIFY(minDistance(g2, d4) == 2, caseLabel);
      VERIFY(minDistance(h2, f6) == 2, caseLabel);
   }
}

///////////////////

void testQuadrantFromSquare()
{
   {
      const std::string caseLabel = "quadrant(Square)";

      VERIFY(quadrant(a1) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(a2) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(a3) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(a4) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(a5) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(a6) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(a7) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(a8) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(b1) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(b2) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(b3) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(b4) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(b5) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(b6) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(b7) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(b8) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(c1) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(c2) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(c3) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(c4) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(c5) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(c6) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(c7) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(c8) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(d1) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(d2) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(d3) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(d4) == Quadrant::a1, caseLabel);
      VERIFY(quadrant(d5) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(d6) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(d7) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(d8) == Quadrant::a8, caseLabel);
      VERIFY(quadrant(e1) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(e2) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(e3) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(e4) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(e5) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(e6) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(e7) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(e8) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(f1) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(f2) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(f3) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(f4) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(f5) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(f6) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(f7) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(f8) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(g1) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(g2) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(g3) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(g4) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(g5) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(g6) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(g7) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(g8) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(h1) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(h2) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(h3) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(h4) == Quadrant::h1, caseLabel);
      VERIFY(quadrant(h5) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(h6) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(h7) == Quadrant::h8, caseLabel);
      VERIFY(quadrant(h8) == Quadrant::h8, caseLabel);
   }
}

void testQuadrantInQuadrant()
{
   {
      const std::string caseLabel = "inQuadrant(Square, Quadrant)";

      VERIFY(inQuadrant(a1, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(a1, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(a1, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(a1, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(a4, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(a4, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(a4, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(a4, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(a5, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(a5, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(a5, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(a5, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(a8, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(a8, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(a8, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(a8, Quadrant::h8), caseLabel);

      VERIFY(inQuadrant(d1, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(d1, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(d1, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(d1, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(d4, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(d4, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(d4, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(d4, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(d5, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(d5, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(d5, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(d5, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(d8, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(d8, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(d8, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(d8, Quadrant::h8), caseLabel);

      VERIFY(inQuadrant(e1, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(e1, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(e1, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(e1, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(e4, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(e4, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(e4, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(e4, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(e5, Quadrant::h8), caseLabel);
      VERIFY(!inQuadrant(e5, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(e5, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(e5, Quadrant::h1), caseLabel);
      VERIFY(inQuadrant(e8, Quadrant::h8), caseLabel);
      VERIFY(!inQuadrant(e8, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(e8, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(e8, Quadrant::h1), caseLabel);

      VERIFY(inQuadrant(h1, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(h1, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(h1, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(h1, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(h4, Quadrant::h1), caseLabel);
      VERIFY(!inQuadrant(h4, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(h4, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(h4, Quadrant::h8), caseLabel);
      VERIFY(inQuadrant(h5, Quadrant::h8), caseLabel);
      VERIFY(!inQuadrant(h5, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(h5, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(h5, Quadrant::h1), caseLabel);
      VERIFY(inQuadrant(h8, Quadrant::h8), caseLabel);
      VERIFY(!inQuadrant(h8, Quadrant::a1), caseLabel);
      VERIFY(!inQuadrant(h8, Quadrant::a8), caseLabel);
      VERIFY(!inQuadrant(h8, Quadrant::h1), caseLabel);
   }
}

void testQuadrantIsFriendlyQuadrant()
{
   {
      const std::string caseLabel = "isFriendlyQuadrant(Quadrant, Color)";

      VERIFY(isFriendlyQuadrant(Quadrant::a1, White), caseLabel);
      VERIFY(!isFriendlyQuadrant(Quadrant::a1, Black), caseLabel);
      VERIFY(isFriendlyQuadrant(Quadrant::h1, White), caseLabel);
      VERIFY(!isFriendlyQuadrant(Quadrant::h1, Black), caseLabel);
      VERIFY(isFriendlyQuadrant(Quadrant::a8, Black), caseLabel);
      VERIFY(!isFriendlyQuadrant(Quadrant::a8, White), caseLabel);
      VERIFY(isFriendlyQuadrant(Quadrant::h8, Black), caseLabel);
      VERIFY(!isFriendlyQuadrant(Quadrant::h8, White), caseLabel);
   }
}

} // namespace


///////////////////

void testFile()
{
   testFileIsValid();
   testFileIncrementOperator();
   testFileDecrementOperator();
   testFileToLowercaseChar();
   testFileIsLowerAdjacent();
   testFileIsHigherAdjacent();
   testFileIsAdjacent();
   testFileDistance();
}

void testRank()
{
   testRankIsValid();
   testRankIncrementOperator();
   testRankDecrementOperator();
   testRankToLowercaseChar();
   testRankIsLowerAdjacent();
   testRankIsHigherAdjacent();
   testRankIsAdjacent();
   testRankDistance();
}

void testSquare()
{
   testMakeSquareFromNotation();
   testMakeSquareFromFileRank();
   testGettingFileOfSquare();
   testGettingRankOfSquare();
   testSquareIsValid();
   testSquareIncrementOperator();
}

void testDiagonal()
{
   testUpDiagonal();
   testDownDiagonal();
   testOnSameUpDiagonal();
   testOnSameDownDiagonal();
   testOnSameDiagonal();
}

void testOffset()
{
   testOffsetEquality();
   testOffsetInequality();
   testOffsetIsOnBoard();
   testOffsetAdditionToSquare();
   testOffsetOffset();
   testOffsetMinDistance();
}

void testQuadrant()
{
   testQuadrantFromSquare();
   testQuadrantInQuadrant();
   testQuadrantIsFriendlyQuadrant();
}