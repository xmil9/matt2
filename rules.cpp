//
// Jul-2021, Michael Lindner
// MIT license
//
#include "rules.h"

using namespace matt2;


namespace
{
///////////////////

std::optional<Piece> collectBasicMove(Piece piece, Square from, Square to,
                                      const Position& pos, std::vector<Move>& moves)
{
   auto destPiece = pos[to];
   if (!destPiece || !haveSameColor(piece, *destPiece))
      moves.push_back(BasicMove{Relocation{piece, from, to}, destPiece});
   return destPiece;
}


template <std::size_t N>
void collectOffsetMoves(Piece piece, Square at, const Position& pos,
                        const std::array<Offset, N>& offsets, std::vector<Move>& moves)
{
   for (const auto& off : offsets)
      if (isOnBoard(at, off))
         collectBasicMove(piece, at, at + off, pos, moves);
}


template <std::size_t N>
void collectDirectionalMoves(Piece piece, Square at, const Position& pos,
                             const std::array<Offset, N>& directions,
                             std::vector<Move>& moves)
{
   for (const auto& off : directions)
   {
      Square to = at;
      std::optional<Piece> destPiece;

      // Keep moving into direction until the board ends or another piece is in
      // the way.
      while (isOnBoard(to, off) && !destPiece)
      {
         to = to + off;
         destPiece = collectBasicMove(piece, at, to, pos, moves);
      }
   }
}


bool isPromotion(Piece pawn, Square to)
{
   assert(isPawn(pawn));
   return rank(to) == (isWhite(pawn) ? r8 : r1);
}


void collectPromotions(Piece pawn, Square at, Square to, std::optional<Piece> taken,
                       std::vector<Move>& moves)
{
   // Add move for each possible promotion.
   // Note that the moves validity has already been verified.
   using Promotions_t = std::array<Piece, 4>;
   static constexpr Promotions_t WhitePromotions = { Qw, Rw, Bw, Nw }; 
   static constexpr Promotions_t BlackPromotions = { Qb, Rb, Bb, Nb }; 
   
   const Relocation movement{pawn, at, to};

   const Promotions_t& promotions = isWhite(pawn) ? WhitePromotions : BlackPromotions;
   for (const Piece& promotedTo : promotions)
      moves.push_back(Promotion{movement, promotedTo, taken});
}


bool collectForwardPawnMove(Piece pawn, Square at, const Position& pos, Offset forward,
                            std::vector<Move>& moves)
{
   assert(isPawn(pawn));
   // To avoid duplicate checking, caller should check that destination square is on
   // the board.
   assert(isOnBoard(at, forward));

   bool moveCollected = false;

   Square to = at + forward;
   auto destPiece = pos[to];
   if (!destPiece)
   {
      assert(forward.dr == 1 || forward.dr == 2);
      if (forward.dr == 1)
      {
         if (isPromotion(pawn, to))
            collectPromotions(pawn, at, to, destPiece, moves);
         else
            moves.push_back(BasicMove{Relocation{pawn, at, to}});
      }
      else
      {
         // Moving by two squares allows opponent to use en-passant rule in the
         // next move.
         moves.push_back(BasicMove{Relocation{pawn, at, to}, EnabesEnPassant});
      }

      moveCollected = true;
   }

   return moveCollected;
}


void collectDiagonalPawnMove(Piece pawn, Square at, const Position& pos, Offset diagonal,
                             std::vector<Move>& moves)
{
   assert(isPawn(pawn));
   if (isOnBoard(at, diagonal))
   {
      Square to = at + diagonal;
      auto destPiece = pos[to];
      if (destPiece.has_value() && !haveSameColor(pawn, *destPiece))
      {
         if (isPromotion(pawn, to))
            collectPromotions(pawn, at, to, destPiece, moves);
         else
            moves.push_back(BasicMove{Relocation{pawn, at, to}, destPiece});
      }
   }
}


bool isPawnOnInitialRank(Piece pawn, Square at)
{
   const Rank initialRank = isWhite(pawn) ? r2 : r7;
   return rank(at) == initialRank;
}

} // namespace


namespace matt2
{
///////////////////

void collectKingMoves(Piece king, Square at, const Position& pos,
                      std::vector<Move>& moves)
{
   assert(isKing(king));
   static constexpr std::array<Offset, 8> Offsets{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectOffsetMoves(king, at, pos, Offsets, moves);
   // todo - exclude moves that lead to check
}


void collectQueenMoves(Piece queen, Square at, const Position& pos,
                       std::vector<Move>& moves)
{
   assert(isQueen(queen));
   static constexpr std::array<Offset, 8> Directions{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectDirectionalMoves(queen, at, pos, Directions, moves);
}


void collectRookMoves(Piece rook, Square at, const Position& pos,
                      std::vector<Move>& moves)
{
   assert(isRook(rook));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
   collectDirectionalMoves(rook, at, pos, Directions, moves);
}


void collectBishopMoves(Piece bishop, Square at, const Position& pos,
                        std::vector<Move>& moves)
{
   assert(isBishop(bishop));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
   collectDirectionalMoves(bishop, at, pos, Directions, moves);
}


void collectKnightMoves(Piece knight, Square at, const Position& pos,
                        std::vector<Move>& moves)
{
   assert(isKnight(knight));
   static constexpr std::array<Offset, 8> Offsets{
      Offset{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
   collectOffsetMoves(knight, at, pos, Offsets, moves);
}


void collectPawnMoves(Piece pawn, Square at, const Position& pos,
                      std::vector<Move>& moves)
{
   assert(isPawn(pawn));

   // Move one square forward if square is not occupied.
   bool couldMoveForward = false;
   const Offset forward{0, isWhite(pawn) ? 1 : -1};
   if (isOnBoard(at, forward))
      couldMoveForward = collectForwardPawnMove(pawn, at, pos, forward, moves);

   // Move two squares forward if pawn is on initial rank and no other piece is
   // in front of it.
   // Note that checking for the initial rank also covers checking whether the
   // destination square is on the board.
   if (isPawnOnInitialRank(pawn, at) && couldMoveForward)
   {
      const Offset forwardBy2{0, isWhite(pawn) ? 2 : -2};
      collectForwardPawnMove(pawn, at, pos, forwardBy2, moves);
   }

   // Add moves for taking pieces diagonally.
   const Offset diagonalRight{1, forward.dr};
   collectDiagonalPawnMove(pawn, at, pos, diagonalRight, moves);
   const Offset diagonalLeft{-1, forward.dr};
   collectDiagonalPawnMove(pawn, at, pos, diagonalLeft, moves);
}

} // namespace matt2
