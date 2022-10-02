//
// Nov-2021, Michael Lindner
// MIT license
//
#include "notation.h"

using namespace matt2;

namespace
{
///////////////////

std::string& notateMoveInAlgebraicNotation(std::string& out, const matt2::BasicMove& move,
                                           bool withColor, bool withStart)
{
   out += toString(move.piece(), withColor);
   if (withStart)
      out += toString(move.from());
   if (auto taken = move.taken(); taken.has_value())
      out += 'x';
   out += toString(move.to());
   return out;
}

std::string& notateCastlingInAlgebraicNotation(std::string& out,
                                               const matt2::Castling& move,
                                               bool withColor)
{
   if (withColor)
      out += matt2::toString(color(move.king()));

   const bool isKingside = file(move.kingTo()) == fg;
   out += isKingside ? "0-0" : "0-0-0";
   return out;
}

std::string& notateEnPassantInAlgebraicNotation(std::string& out,
                                                const matt2::EnPassant& move,
                                                bool withColor)
{
   if (withColor)
      out += matt2::toString(color(move.pawn()));

   // Moved pawn's starting file.
   out += matt2::toString(file(move.from()));
   // Capturing indicator.
   out += "x";
   // Moved pawn's destination square.
   out += matt2::toString(move.to());

   return out;
}

std::string& notatePromotionInAlgebraicNotation(std::string& out,
                                                const matt2::Promotion& move,
                                                bool withColor, bool withStart)
{
   out += matt2::toString(move.pawn(), withColor);
   if (withStart)
      out += matt2::toString(move.from());

   if (move.taken().has_value())
      out += "x";

   out += matt2::toString(move.to());
   // Piece that pawn was promoted to.
   out += '=';
   out += matt2::toString(move.promotedTo(), withColor);

   return out;
}

} // namespace


namespace matt2
{
///////////////////

std::string& Lan::notate(std::string& out, const BasicMove& move) const
{
   return notateMoveInAlgebraicNotation(out, move, WithoutPieceColor, WithStartingLocation);
}

std::string& Lan::notate(std::string& out, const Castling& move) const
{
   return notateCastlingInAlgebraicNotation(out, move, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const EnPassant& move) const
{
   return notateEnPassantInAlgebraicNotation(out, move, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const Promotion& move) const
{
   return notatePromotionInAlgebraicNotation(out, move, WithoutPieceColor, WithStartingLocation);
}

///////////////////

std::string& DetailedNotation::notate(std::string& out, const BasicMove& move) const
{
   notateMoveInAlgebraicNotation(out, move, WithPieceColor, WithStartingLocation);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      out += "[x:" + toString(*taken, WithPieceColor) + "]";

   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Castling& move) const
{
   return notateCastlingInAlgebraicNotation(out, move, WithPieceColor);
}

std::string& DetailedNotation::notate(std::string& out, const EnPassant& move) const
{
   notateEnPassantInAlgebraicNotation(out, move, WithPieceColor);

   // Append info about taken piece.
   out += "[x:" + toString(move.taken(), WithPieceColor) + "]";

   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Promotion& move) const
{
   notatePromotionInAlgebraicNotation(out, move, WithPieceColor, WithStartingLocation);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      out += "[x:" + toString(*taken, WithPieceColor) + "]";

   return out;
}

} // namespace matt2
