//
// Jun-2021, Michael Lindner
// MIT license
//
#include "square.h"
#include <stdexcept>


namespace matt2
{
///////////////////

Square makeSquare(std::string_view notation)
{
   if (notation.size() < 2)
      throw std::runtime_error("Invalid notation for square.");

   // Read file.
   const unsigned char file = notation[0];
   if (file < 'a' || file > 'h')
      throw std::runtime_error("Invalid file notation for square.");

   // Read rank.
   const unsigned char rank = notation[1];
   if (rank < '1' || rank > '8')
      throw std::runtime_error("Invalid rank notation for square.");

   return static_cast<Square>((file - 'a') * 8 + (rank - '1'));
}

} // namespace matt2
