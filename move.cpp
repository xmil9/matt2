//
// Nov-2021, Michael Lindner
// MIT license
//
#include "move.h"

namespace matt2
{
///////////////////

std::string BasicMove::toString(NotationScheme scheme) const
{
   const bool withColor = scheme == NotationScheme::Detailed;
   const bool withStartLocation =
      scheme == NotationScheme::LAN || scheme == NotationScheme::Detailed;
   const bool withTakenPiece = scheme == NotationScheme::Detailed;

   std::string s = matt2::toString(m_moved.piece(), withColor);

   if (withStartLocation)
      s += matt2::toString(m_moved.from());

   if (m_taken.has_value())
   {
      // When a pawn captures indicate its start file before the 'x',
      // unless we already added the full start location.
      if (isPawn(m_moved.piece()) && !withStartLocation)
         s += matt2::toString(file(m_moved.from()));

      s += "x";
      
      if (withTakenPiece)
         s += "(" + matt2::toString(*m_taken, withColor) + ")";
   }

   s += matt2::toString(m_moved.to());

   return s;
}


///////////////////

std::string Castling::toString(NotationScheme scheme) const
{
   const bool withColor = scheme == NotationScheme::Detailed;

   std::string s;
   if (withColor)
      s += matt2::toString(color(m_king.piece()));

   const bool isKingside = file(m_king.to()) == fg;
   s += isKingside ? "0-0" : "0-0-0";

   return s;
}


///////////////////

std::string EnPassant::toString(NotationScheme scheme) const
{
   const bool withColor = scheme == NotationScheme::Detailed;

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

std::string Promotion::toString(NotationScheme scheme) const
{
   const bool withColor = scheme == NotationScheme::Detailed;
   const bool withStartLocation =
      scheme == NotationScheme::LAN || scheme == NotationScheme::Detailed;
   const bool withTakenPiece = scheme == NotationScheme::Detailed;

   std::string s;
   if (withColor)
      s += matt2::toString(color(m_movedPawn.piece()));

   if (m_taken.has_value())
   {
      // When a piece was captured indicate the start file before the 'x',
      // unless we already added the full start location.
      if (!withStartLocation)
         s += matt2::toString(file(m_movedPawn.at()));

      s += "x";
      
      if (withTakenPiece)
         s += "(" + matt2::toString(*m_taken, withColor) + ")";
   }

   s += matt2::toString(m_promoted.at());
   // Piece that pawn was promoted to.
   s += matt2::toString(m_promoted.piece(), withColor);

   return s;
}

} // namespace matt2
