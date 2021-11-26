//
// May-2021, Michael Lindner
// MIT license
//
#include "piece.h"
#include <stdexcept>
#include <unordered_map>

using namespace matt2;


namespace
{
///////////////////

// Create unique integer for given figure and color codes.
constexpr unsigned short PieceHash(unsigned char figure, unsigned char color)
{
   return (figure << 8) + color;
}

} // namespace


namespace matt2
{
///////////////////

Piece makePiece(std::string_view notation)
{
   if (notation.empty())
      throw std::runtime_error("Invalid notation for piece.");

   // Read piece type.
   std::size_t idx = 0;
   unsigned char figure = 0;
   switch (notation[idx])
   {
   // Pieces with letter notation.
   case 'K':
   case 'Q':
   case 'R':
   case 'B':
   case 'N':
      figure = notation[idx++];
      break;
   // Pawns have no letter to identify them. Use 'P' as identifier.
   case 'w':
   case 'b':
      figure = 'P';
      break;
   default:
      throw std::runtime_error("Invalid notation for piece.");
   }

   if (idx >= notation.size())
      throw std::runtime_error("Invalid notation for piece.");

   // Read piece color.
   unsigned char color = 0;
   switch (notation[idx])
   {
   case 'w':
   case 'b':
      color = notation[idx];
      break;
   default:
      throw std::runtime_error("Invalid notation for piece.");
   }

   // Lookup table for pieces by figure and color codes.
   static const std::unordered_map<unsigned short, Piece> PieceTable = {
      {PieceHash('K', 'w'), Kw}, {PieceHash('Q', 'w'), Qw}, {PieceHash('R', 'w'), Rw},
      {PieceHash('B', 'w'), Bw}, {PieceHash('N', 'w'), Nw}, {PieceHash('P', 'w'), Pw},
      {PieceHash('K', 'b'), Kb}, {PieceHash('Q', 'b'), Qb}, {PieceHash('R', 'b'), Rb},
      {PieceHash('B', 'b'), Bb}, {PieceHash('N', 'b'), Nb}, {PieceHash('P', 'b'), Pb},
   };

   const auto pos = PieceTable.find(PieceHash(figure, color));
   if (pos == PieceTable.end())
      throw std::runtime_error("Unable to find piece in lookup table.");

   return pos->second;
}


std::string toString(Piece p, bool withColor)
{
   std::string s;
   switch (p)
   {
   case Kw:
   case Kb:
      s += "K";
      break;
   case Qw:
   case Qb:
      s += "Q";
      break;
   case Rw:
   case Rb:
      s += "R";
      break;
   case Bw:
   case Bb:
      s += "B";
      break;
   case Nw:
   case Nb:
      s += "N";
      break;
   case Pw:
   case Pb:
      break;
   default:
      throw std::runtime_error("Invalid piece.");
   }

   if (withColor)
      s += toString(color(p));
   
   return s;
}

} // namespace matt2
