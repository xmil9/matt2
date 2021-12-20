//
// Nov-2021, Michael Lindner
// MIT license
//
#include "notation.h"


namespace
{
///////////////////

std::string& notateMoveAlgebraicNotation(std::string& out, const matt2::BasicMove& move,
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

} // namespace


namespace matt2
{
///////////////////

std::string& Lan::notate(std::string& out, const BasicMove& move) const
{
   notateMoveAlgebraicNotation(out, move, WithPieceColor, WithStartingLocation);
   return out;
}

///////////////////

std::string& DetailedNotation::notate(std::string& out, const BasicMove& move) const
{
   notateMoveAlgebraicNotation(out, move, WithPieceColor, WithStartingLocation);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      out += "[x:" + toString(*taken, WithPieceColor) + "]";

   return out;
}


std::string& DetailedNotation::notate(std::string& out, const EnPassant& move) const
{
   out += move.toString(WithPieceColor);

   // Append info about taken piece.
   out += "[x: " + toString(move.taken(), WithPieceColor) + "]";

   return out;
}


std::string& DetailedNotation::notate(std::string& out, const Promotion& move) const
{
   out += move.toString(WithPieceColor, true);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
   {
      out += "[x: " + toString(*taken, WithPieceColor) + "]";
   }

   return out;
}

} // namespace matt2
