//
// Jul-2021, Michael Lindner
// MIT license
//
#include "rules.h"
#include <cmath>
#include <functional>

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
   // Note that validity of the moves has already been verified.
   using Promotions_t = std::array<Piece, 4>;
   static constexpr Promotions_t WhitePromotions = {Qw, Rw, Bw, Nw};
   static constexpr Promotions_t BlackPromotions = {Qb, Rb, Bb, Nb};

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
      assert(std::abs(forward.dr) == 1 || std::abs(forward.dr) == 2);
      if (std::abs(forward.dr) == 1)
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
         moves.push_back(BasicMove{Relocation{pawn, at, to}, EnablesEnPassant});
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


///////////////////

enum class SquareFilterPolicy
{
   OnlyEmpty,
   EmptyOrOpponent
};

using SquareFilterFn = std::function<bool(Square)>;

SquareFilterFn makeEmptySquareFilter(const Position& pos)
{
   return [&pos](Square at) { return !pos[at]; };
}

SquareFilterFn makeEmptyOrOpponentSquareFilter(Color opponent, const Position& pos)
{
   return [opponent, &pos](Square at) {
      const auto destPiece = pos[at];
      return !destPiece || color(*destPiece) == opponent;
   };
}

SquareFilterFn makeSquareFilter(SquareFilterPolicy policy, Piece piece,
                                const Position& pos)
{
   switch (policy)
   {
   case SquareFilterPolicy::OnlyEmpty:
      return makeEmptySquareFilter(pos);
   case SquareFilterPolicy::EmptyOrOpponent:
      return makeEmptyOrOpponentSquareFilter(!color(piece), pos);
   default:
      assert(false && "Unknown square filter policy.");
      return {};
   }
}


///////////////////

template <std::size_t N>
void collectOffsetSquares(Piece piece, Square at, const Position& pos,
                          SquareFilterPolicy policy, const std::array<Offset, N>& offsets,
                          std::vector<Square>& squares)
{
   SquareFilterFn filterFn = makeSquareFilter(policy, piece, pos);

   for (const auto& off : offsets)
      if (isOnBoard(at, off))
      {
         const Square to = at + off;
         if (filterFn(to))
            squares.push_back(to);
      }
}


template <std::size_t N>
void collectDirectionalSquares(Piece piece, Square at, const Position& pos,
                               SquareFilterPolicy policy,
                               const std::array<Offset, N>& directions,
                               std::vector<Square>& squares)
{
   SquareFilterFn filterFn = makeSquareFilter(policy, piece, pos);

   for (const auto& off : directions)
   {
      Square to = at;

      // Keep moving into direction until the board ends or another piece is in
      // the way.
      while (isOnBoard(to, off))
      {
         to = to + off;
         if (!filterFn(to))
            break;
         squares.push_back(to);
      }
   }
}


///////////////////

bool isPawnOnInitialRank(Piece pawn, Square at)
{
   const Rank initialRank = isWhite(pawn) ? r2 : r7;
   return rank(at) == initialRank;
}


bool areCastlingSquaresOccupied(Color side, bool onKingside, const Position& pos)
{
   // Squares between king and rook that cannot be occupied for each castling type.
   // Note that these are not the same as the squares that cannot be attacked for
   // castling.
   static const std::vector<Square> WhiteKingside = {f1, g1};
   static const std::vector<Square> WhiteQueenside = {b1, c1, d1};
   static const std::vector<Square> BlackKingside = {f8, g8};
   static const std::vector<Square> BlackQueenside = {b8, c8, d8};

   const std::vector<Square>& squares =
      side == Color::White ? (onKingside ? WhiteKingside : WhiteQueenside)
                           : (onKingside ? BlackKingside : BlackQueenside);

   for (Square sq : squares)
      if (pos[sq].has_value())
         return true;
   return false;
}


bool areCastlingSquaresAttacked(Color side, bool onKingside, const Position& pos)
{
   // Squares that castling king moves across for each castling type, including the king's
   // initial square.
   // Note that castling can still happen if the rook is attacked or moving across an
   // attacked square (i.e. g1/g8 when castling queen-side).
   // Note that these are not the same as the squares that cannot be
   // occupied for castling.
   static const std::vector<Square> WhiteKingside = {e1, f1, g1};
   static const std::vector<Square> WhiteQueenside = {c1, d1, e1};
   static const std::vector<Square> BlackKingside = {e8, f8, g8};
   static const std::vector<Square> BlackQueenside = {c8, d8, e8};

   const std::vector<Square>& squares =
      side == Color::White ? (onKingside ? WhiteKingside : WhiteQueenside)
                           : (onKingside ? BlackKingside : BlackQueenside);
   const Color opponent = !side;

   for (auto sq : squares)
      if (pos.canAttack(sq, opponent))
         return true;
   return false;
}


bool haveCastlingRook(Color side, bool onKingside, const Position& pos)
{
   const Square rookSq = makeSquare(onKingside ? fh : fa, side == Color::White ? r1 : r8);
   const auto piece = pos[rookSq];
   return piece.has_value() && isRook(*piece) && color(*piece) == side;
}


bool canCastle(Color side, bool onKingside, const Position& pos)
{
   return !pos.hasKingMoved(side) && haveCastlingRook(side, onKingside, pos) &&
          !pos.hasRookMoved(side, onKingside) &&
          !areCastlingSquaresOccupied(side, onKingside, pos) &&
          !areCastlingSquaresAttacked(side, onKingside, pos);
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


void collectCastlingMoves(Color side, const Position& pos, std::vector<Move>& moves)
{
   if (canCastle(side, true, pos))
      moves.push_back(Castling{Kingside, side});
   if (canCastle(side, false, pos))
      moves.push_back(Castling{Queenside, side});
}


void collectEnPassantMoves(Color side, const Position& pos, std::vector<Move>& moves)
{
   // Did oppenent make move that allows en-passant?
   if (const auto epFile = pos.enPassantFile(); epFile.has_value())
   {
      const auto fromRank = side == Color::White ? r5 : r4;
      const auto toRank = side == Color::White ? r6 : r3;
      Square to = makeSquare(*epFile, toRank);

      // Try both neighboring files.
      static constexpr std::array<int, 2> FileOffsets = {-1, 1};
      for (int off : FileOffsets)
      {
         // Is the neighboring file on the board?
         if (isValid(*epFile, off))
         {
            const Square from = makeSquare(*epFile + off, fromRank);

            // Is a pawn of matching color on the right square to make an en-passent
            // move?
            if (const auto piece = pos[from];
                piece.has_value() && isPawn(*piece) && color(*piece) == side)
            {
               moves.push_back(EnPassant{Relocation(*piece, from, to)});
            }
         }
      }
   }
}

///////////////////

void collectAttackedByKing(Piece king, Square at, const Position& pos,
                           std::vector<Square>& attacked)
{
   assert(isKing(king));
   static constexpr std::array<Offset, 8> Offsets{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectOffsetSquares(king, at, pos, SquareFilterPolicy::EmptyOrOpponent, Offsets,
                        attacked);
}


void collectAttackedByQueen(Piece queen, Square at, const Position& pos,
                            std::vector<Square>& attacks)
{
   assert(isQueen(queen));
   static constexpr std::array<Offset, 8> Directions{
      Offset{1, 1}, {1, 0}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
   collectDirectionalSquares(queen, at, pos, SquareFilterPolicy::EmptyOrOpponent,
                             Directions, attacks);
}


void collectAttackedByRook(Piece rook, Square at, const Position& pos,
                           std::vector<Square>& attacks)
{
   assert(isRook(rook));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 0}, {0, 1}, {0, -1}, {-1, 0}};
   collectDirectionalSquares(rook, at, pos, SquareFilterPolicy::EmptyOrOpponent,
                             Directions, attacks);
}


void collectAttackedByBishop(Piece bishop, Square at, const Position& pos,
                             std::vector<Square>& attacks)
{
   assert(isBishop(bishop));
   static constexpr std::array<Offset, 4> Directions{
      Offset{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
   collectDirectionalSquares(bishop, at, pos, SquareFilterPolicy::EmptyOrOpponent,
                             Directions, attacks);
}


void collectAttackedByKnight(Piece knight, Square at, const Position& pos,
                             std::vector<Square>& attacks)
{
   assert(isKnight(knight));
   static constexpr std::array<Offset, 8> Offsets{
      Offset{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
   collectOffsetSquares(knight, at, pos, SquareFilterPolicy::EmptyOrOpponent, Offsets,
                        attacks);
}


void collectAttackedByPawn(Piece pawn, Square at, const Position& pos,
                           std::vector<Square>& attacks)
{
   assert(isPawn(pawn));
   static constexpr std::array<Offset, 2> Offsets{Offset{1, 1}, {1, -1}};
   collectOffsetSquares(pawn, at, pos, SquareFilterPolicy::EmptyOrOpponent, Offsets,
                        attacks);
}

} // namespace matt2
