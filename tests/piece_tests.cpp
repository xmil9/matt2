//
// Jun-2021, Michael Lindner
// MIT license
//
#include "piece_tests.h"
#include "piece.h"
#include "test_util.h"
#include <stdexcept>

using namespace matt2;


namespace
{
///////////////////

void testColorNegation()
{
   {
      const std::string caseLabel = "Color negation";

      VERIFY(!Color::White == Color::Black, caseLabel);
      VERIFY(!Color::Black == Color::White, caseLabel);
   }
}


///////////////////

void testMakePiece()
{
   {
      const std::string caseLabel = "makePiece for valid pieces";

      VERIFY(makePiece("Kw") == Kw, caseLabel);
      VERIFY(makePiece("Qw") == Qw, caseLabel);
      VERIFY(makePiece("Rw") == Rw, caseLabel);
      VERIFY(makePiece("Bw") == Bw, caseLabel);
      VERIFY(makePiece("Nw") == Nw, caseLabel);
      VERIFY(makePiece("w") == Pw, caseLabel);
      VERIFY(makePiece("Kb") == Kb, caseLabel);
      VERIFY(makePiece("Qb") == Qb, caseLabel);
      VERIFY(makePiece("Rb") == Rb, caseLabel);
      VERIFY(makePiece("Bb") == Bb, caseLabel);
      VERIFY(makePiece("Nb") == Nb, caseLabel);
      VERIFY(makePiece("b") == Pb, caseLabel);
   }
   {
      const std::string caseLabel = "makePiece for empty notation";

      try
      {
         makePiece("");
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
      const std::string caseLabel = "makePiece for invalid figure notation";

      try
      {
         makePiece("Tw");
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
      const std::string caseLabel = "makePiece for invalid color notation";

      try
      {
         makePiece("Kf");
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
      const std::string caseLabel = "makePiece for missing color notation";

      try
      {
         makePiece("K");
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


void testIsKing()
{
   {
      const std::string caseLabel = "isKing for kings";

      VERIFY(isKing(Kw), caseLabel);
      VERIFY(isKing(Kb), caseLabel);
   }
   {
      const std::string caseLabel = "isKing for other pieces";

      VERIFY(!isKing(Qw), caseLabel);
      VERIFY(!isKing(Qb), caseLabel);
      VERIFY(!isKing(Rw), caseLabel);
      VERIFY(!isKing(Rb), caseLabel);
      VERIFY(!isKing(Bw), caseLabel);
      VERIFY(!isKing(Bb), caseLabel);
      VERIFY(!isKing(Nw), caseLabel);
      VERIFY(!isKing(Nb), caseLabel);
      VERIFY(!isKing(Pw), caseLabel);
      VERIFY(!isKing(Pb), caseLabel);
   }
}


void testIsQueen()
{
   {
      const std::string caseLabel = "isQueen for queens";

      VERIFY(isQueen(Qw), caseLabel);
      VERIFY(isQueen(Qb), caseLabel);
   }
   {
      const std::string caseLabel = "isQueen for other pieces";

      VERIFY(!isQueen(Kw), caseLabel);
      VERIFY(!isQueen(Kb), caseLabel);
      VERIFY(!isQueen(Rw), caseLabel);
      VERIFY(!isQueen(Rb), caseLabel);
      VERIFY(!isQueen(Bw), caseLabel);
      VERIFY(!isQueen(Bb), caseLabel);
      VERIFY(!isQueen(Nw), caseLabel);
      VERIFY(!isQueen(Nb), caseLabel);
      VERIFY(!isQueen(Pw), caseLabel);
      VERIFY(!isQueen(Pb), caseLabel);
   }
}


void testIsRook()
{
   {
      const std::string caseLabel = "isRook for rooks";

      VERIFY(isRook(Rw), caseLabel);
      VERIFY(isRook(Rb), caseLabel);
   }
   {
      const std::string caseLabel = "isRook for other pieces";

      VERIFY(!isRook(Kw), caseLabel);
      VERIFY(!isRook(Kb), caseLabel);
      VERIFY(!isRook(Qw), caseLabel);
      VERIFY(!isRook(Qb), caseLabel);
      VERIFY(!isRook(Bw), caseLabel);
      VERIFY(!isRook(Bb), caseLabel);
      VERIFY(!isRook(Nw), caseLabel);
      VERIFY(!isRook(Nb), caseLabel);
      VERIFY(!isRook(Pw), caseLabel);
      VERIFY(!isRook(Pb), caseLabel);
   }
}


void testIsBishop()
{
   {
      const std::string caseLabel = "isBishop for bishops";

      VERIFY(isBishop(Bw), caseLabel);
      VERIFY(isBishop(Bb), caseLabel);
   }
   {
      const std::string caseLabel = "isBishop for other pieces";

      VERIFY(!isBishop(Kw), caseLabel);
      VERIFY(!isBishop(Kb), caseLabel);
      VERIFY(!isBishop(Qw), caseLabel);
      VERIFY(!isBishop(Qb), caseLabel);
      VERIFY(!isBishop(Rw), caseLabel);
      VERIFY(!isBishop(Rb), caseLabel);
      VERIFY(!isBishop(Nw), caseLabel);
      VERIFY(!isBishop(Nb), caseLabel);
      VERIFY(!isBishop(Pw), caseLabel);
      VERIFY(!isBishop(Pb), caseLabel);
   }
}


void testIsKnight()
{
   {
      const std::string caseLabel = "isKnight for knights";

      VERIFY(isKnight(Nw), caseLabel);
      VERIFY(isKnight(Nb), caseLabel);
   }
   {
      const std::string caseLabel = "isKnight for other pieces";

      VERIFY(!isKnight(Kw), caseLabel);
      VERIFY(!isKnight(Kb), caseLabel);
      VERIFY(!isKnight(Qw), caseLabel);
      VERIFY(!isKnight(Qb), caseLabel);
      VERIFY(!isKnight(Rw), caseLabel);
      VERIFY(!isKnight(Rb), caseLabel);
      VERIFY(!isKnight(Bw), caseLabel);
      VERIFY(!isKnight(Bb), caseLabel);
      VERIFY(!isKnight(Pw), caseLabel);
      VERIFY(!isKnight(Pb), caseLabel);
   }
}


void testIsPawn()
{
   {
      const std::string caseLabel = "isPawn for pawns";

      VERIFY(isPawn(Pw), caseLabel);
      VERIFY(isPawn(Pb), caseLabel);
   }
   {
      const std::string caseLabel = "isPawn for other pieces";

      VERIFY(!isPawn(Kw), caseLabel);
      VERIFY(!isPawn(Kb), caseLabel);
      VERIFY(!isPawn(Qw), caseLabel);
      VERIFY(!isPawn(Qb), caseLabel);
      VERIFY(!isPawn(Rw), caseLabel);
      VERIFY(!isPawn(Rb), caseLabel);
      VERIFY(!isPawn(Bw), caseLabel);
      VERIFY(!isPawn(Bb), caseLabel);
      VERIFY(!isPawn(Nw), caseLabel);
      VERIFY(!isPawn(Nb), caseLabel);
   }
}


void testPieceColor()
{
   {
      const std::string caseLabel = "color(Piece)";

      VERIFY(color(Kw) == Color::White, caseLabel);
      VERIFY(color(Qw) == Color::White, caseLabel);
      VERIFY(color(Rw) == Color::White, caseLabel);
      VERIFY(color(Bw) == Color::White, caseLabel);
      VERIFY(color(Nw) == Color::White, caseLabel);
      VERIFY(color(Pw) == Color::White, caseLabel);
      VERIFY(color(Kb) == Color::Black, caseLabel);
      VERIFY(color(Qb) == Color::Black, caseLabel);
      VERIFY(color(Rb) == Color::Black, caseLabel);
      VERIFY(color(Bb) == Color::Black, caseLabel);
      VERIFY(color(Nb) == Color::Black, caseLabel);
      VERIFY(color(Pb) == Color::Black, caseLabel);
   }
}


void testPieceIsWhite()
{
   {
      const std::string caseLabel = "isWhite for white pieces";

      VERIFY(isWhite(Kw), caseLabel);
      VERIFY(isWhite(Qw), caseLabel);
      VERIFY(isWhite(Rw), caseLabel);
      VERIFY(isWhite(Bw), caseLabel);
      VERIFY(isWhite(Nw), caseLabel);
      VERIFY(isWhite(Pw), caseLabel);
   }
   {
      const std::string caseLabel = "isWhite for black pieces";

      VERIFY(!isWhite(Kb), caseLabel);
      VERIFY(!isWhite(Qb), caseLabel);
      VERIFY(!isWhite(Rb), caseLabel);
      VERIFY(!isWhite(Bb), caseLabel);
      VERIFY(!isWhite(Nb), caseLabel);
      VERIFY(!isWhite(Pb), caseLabel);
   }
}


void testPieceIsBlack()
{
   {
      const std::string caseLabel = "isBlack for black pieces";

      VERIFY(isBlack(Kb), caseLabel);
      VERIFY(isBlack(Qb), caseLabel);
      VERIFY(isBlack(Rb), caseLabel);
      VERIFY(isBlack(Bb), caseLabel);
      VERIFY(isBlack(Nb), caseLabel);
      VERIFY(isBlack(Pb), caseLabel);
   }
   {
      const std::string caseLabel = "isBlack for white pieces";

      VERIFY(!isBlack(Kw), caseLabel);
      VERIFY(!isBlack(Qw), caseLabel);
      VERIFY(!isBlack(Rw), caseLabel);
      VERIFY(!isBlack(Bw), caseLabel);
      VERIFY(!isBlack(Nw), caseLabel);
      VERIFY(!isBlack(Pw), caseLabel);
   }
}


void testPieceHaveSameColor()
{
   {
      const std::string caseLabel = "haveSameColor for pieces of same color";

      VERIFY(haveSameColor(Kb, Qb), caseLabel);
      VERIFY(haveSameColor(Qb, Kb), caseLabel);
      VERIFY(haveSameColor(Pb, Rb), caseLabel);
      VERIFY(haveSameColor(Bb, Nb), caseLabel);
      VERIFY(haveSameColor(Bb, Bb), caseLabel);
      VERIFY(haveSameColor(Kw, Qw), caseLabel);
      VERIFY(haveSameColor(Qw, Kw), caseLabel);
      VERIFY(haveSameColor(Pw, Rw), caseLabel);
      VERIFY(haveSameColor(Bw, Nw), caseLabel);
      VERIFY(haveSameColor(Bw, Bw), caseLabel);
   }
   {
      const std::string caseLabel = "haveSameColor for pieces of other color";

      VERIFY(!haveSameColor(Kw, Qb), caseLabel);
      VERIFY(!haveSameColor(Qb, Kw), caseLabel);
      VERIFY(!haveSameColor(Pw, Rb), caseLabel);
      VERIFY(!haveSameColor(Bb, Nw), caseLabel);
      VERIFY(!haveSameColor(Bw, Bb), caseLabel);
   }
}

} // namespace


///////////////////

void testColor()
{
   testColorNegation();
}


void testPiece()
{
   testMakePiece();
   testIsKing();
   testIsQueen();
   testIsRook();
   testIsBishop();
   testIsKnight();
   testIsPawn();
   testPieceColor();
   testPieceIsWhite();
   testPieceIsBlack();
   testPieceHaveSameColor();
}
