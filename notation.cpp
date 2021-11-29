//
// Nov-2021, Michael Lindner
// MIT license
//
#include "notation.h"


namespace
{
///////////////////

std::string& notateMoveAN(std::string& out, const matt2::BasicMove& move, bool withColor,
                          bool withPawnStart)
{
   out += toString(move.piece(), withColor);

   if (auto taken = move.taken(); taken.has_value())
   {
      // When a pawn captures indicate its start file before the capture indicator.
      if (withPawnStart && isPawn(*taken))
         out += toString(file(move.from()));
      out += 'x';
   }

   out += toString(move.to());
   return out;
}

} // namespace


namespace matt2
{
///////////////////

std::string& San::notate(std::string& out, const BasicMove& move) const
{
   out += notateMoveAN(out, move, WithPieceColor, WithPawnStart);
   return out;
}


///////////////////

std::string& Lan::notate(std::string& out, const BasicMove& move) const
{
   out += notateMoveAN(out, move, WithPieceColor, WithPawnStart);
   return out;
}

///////////////////

std::string& DetailedNotation::notate(std::string& out, const BasicMove& move) const
{
   out += notateMoveAN(out, move, WithPieceColor, WithPawnStart);
   
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
