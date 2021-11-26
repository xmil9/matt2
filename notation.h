//
// Nov-2021, Michael Lindner
// MIT license
//
#pragma once


namespace matt2
{
///////////////////

enum class NotationScheme
{
   // Standard Algebraic Notation (FIDE)
   SAN,
   // Long Algebraic Notation
   // Includes starting locations of pieces.
   LAN,
   // Includes full details, e.g. piece color and starting location.
   Detailed
};

} // namespace matt2
