//
// Nov-2021, Michael Lindner
// MIT license
//
#include "notation.h"


namespace matt2
{
///////////////////

std::string& DetailedNotation::notate(std::string& out, const BasicMove& move) const
{
   out += move.toString(WithPieceColor, WithPawnStart);
   
   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      out += "[x: " + toString(*taken, WithPieceColor) + "]";

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
   out += move.toString(WithPieceColor, WithPawnStart);
   
   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
   {
      out += "[x: " + toString(*taken, WithPieceColor) + "]";
   }

   return out;
}

} // namespace matt2
