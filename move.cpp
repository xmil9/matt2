//
// Nov-2021, Michael Lindner
// MIT license
//
#include "move.h"

namespace matt2
{
///////////////////

std::string Castling::toString(bool withColor) const
{
   std::string s;
   if (withColor)
      s += matt2::toString(color(m_king.piece()));

   const bool isKingside = file(m_king.to()) == fg;
   s += isKingside ? "0-0" : "0-0-0";

   return s;
}


///////////////////

std::string EnPassant::toString(bool withColor) const
{
   std::string s;
   if (withColor)
      s += matt2::toString(color(m_movedPawn.piece()));

   // Moved pawn's starting file.
   s += matt2::toString(file(m_movedPawn.from()));
   // Capturing indicator.
   s += "x";
   // Moved pawn's destination square.
   s += matt2::toString(m_movedPawn.to());

   return s;
}


///////////////////

std::string Promotion::toString(bool withColor, bool withStart) const
{
   std::string s;
   s += matt2::toString(m_movedPawn.piece(), withColor);
   if (withStart)
      s += matt2::toString(m_movedPawn.at());

   if (m_taken.has_value())
      s += "x";

   s += matt2::toString(m_promoted.at());
   // Piece that pawn was promoted to.
   s += '=';
   s += matt2::toString(m_promoted.piece(), withColor);

   return s;
}

} // namespace matt2
