//
// Nov-2021, Michael Lindner
// MIT license
//
#include "move.h"
#include "build_env.h"
#include "piece.h"
#include "rules.h"
#include "square.h"
#include <format>

using namespace matt2;

///////////////////

static std::string formatPieceAtError(Piece piece, Square at)
{
   const std::string colorStr = toString(color(piece));
   const std::string pieceStr = toString(piece);
   const std::string atStr = toString(at);

#ifdef HAVE_STD_FORMAT
   return std::format("The position doesn't have a {} {} on {}.", colorStr, pieceStr,
                      atStr);
#else
   return std::string{"The position doesn't have a "} + colorStr + " " + pieceStr +
          " on " + atStr;
#endif // HAVE_STD_FORMAT
}

namespace matt2
{
///////////////////

std::pair<bool, std::string> BasicMove::isValid(const Position& pos, Color turn) const
{
   // Check some general conditions.
   if (pos[from()] != piece())
      return {false, formatPieceAtError(piece(), from())};

   if (color(piece()) != turn)
      return {false, "The moved piece is not on the side whose turn it is."};

   // Check that the moves is part of the legal moves.
   std::vector<Move> legalMoves;
   collectMoves(piece(), from(), pos, legalMoves);

   auto iter = std::find(legalMoves.begin(), legalMoves.end(), Move{*this});
   if (iter == legalMoves.end())
      return {false, "Illegal move for the current position."};

   return {true, ""};
}

///////////////////

std::pair<bool, std::string> Castling::isValid(const Position& pos, Color turn) const
{
   // Check some general conditions.
   if (pos[from()] != piece())
      return {false, formatPieceAtError(piece(), from())};

   if (color(piece()) != turn)
      return {false, "The moved piece is not on the side whose turn it is."};

   if (!isKing(piece()))
      return {false, "Only a king can castle. The moved piece is not a king."};

   // Check that the moves is part of the legal moves.
   std::vector<Move> legalMoves;
   collectCastlingMoves(turn, pos, legalMoves);

   auto iter = std::find(legalMoves.begin(), legalMoves.end(), Move{*this});
   if (iter == legalMoves.end())
      return {false, "Illegal move for the current position."};

   return {true, ""};
}

///////////////////

std::pair<bool, std::string> EnPassant::isValid(const Position& pos, Color turn) const
{
   // Check some general conditions.
   if (pos[from()] != piece())
      return {false, formatPieceAtError(piece(), from())};

   if (color(piece()) != turn)
      return {false, "The moved piece is not on the side whose turn it is."};

   if (!isPawn(piece()))
      return {false,
              "Only a pawn can make an en-passant move. The moved piece is not a pawn."};

   // Check that the moves is part of the legal moves.
   std::vector<Move> legalMoves;
   collectEnPassantMoves(turn, pos, legalMoves);

   auto iter = std::find(legalMoves.begin(), legalMoves.end(), Move{*this});
   if (iter == legalMoves.end())
      return {false, "Illegal move for the current position."};

   return {true, ""};
}

///////////////////

std::pair<bool, std::string> Promotion::isValid(const Position& pos, Color turn) const
{
   // Check some general conditions.
   if (pos[from()] != piece())
      return {false, formatPieceAtError(piece(), from())};

   if (color(piece()) != turn)
      return {false, "The moved piece is not on the side whose turn it is."};

   if (!isPawn(piece()))
      return {false, "Only a pawn can be promoted. The moved piece is not a pawn."};

   // Check that the moves is part of the legal moves.
   std::vector<Move> legalMoves;
   collectMoves(piece(), from(), pos, legalMoves);

   auto iter = std::find(legalMoves.begin(), legalMoves.end(), Move{*this});
   if (iter == legalMoves.end())
      return {false, "Illegal move for the current position."};

   return {true, ""};
}

} // namespace matt2
