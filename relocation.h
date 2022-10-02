//
// Jun-2021, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include "square.h"
#include "placement.h"
#include <string>


namespace matt2
{
///////////////////

// Represents a piece changing its location.
// Relocation vs move:
// This is not the same as a move. A move involves more information, e.g. about
// a taken piece or a promotion. A relocation is usually one of the pieces of
// information that make up a move.
class Relocation
{
 public:
   Relocation(Piece piece, Square from, Square to);
   Relocation(const Placement& from, Square to);
   // Notation examples: "Kbb3b4", "Rwh1d1", "bc7c6"
   explicit Relocation(std::string_view notation);

   Piece piece() const { return m_from.piece(); }
   Square from() const { return m_from.at(); }
   Square to() const { return m_to; }
   const Placement& placement() const { return m_from; }

   Relocation reverse() const;

 private:
   Placement m_from;
   Square m_to;
};


inline Relocation::Relocation(Piece piece, Square from, Square to)
: Relocation{Placement{piece, from}, to}
{
}

inline Relocation::Relocation(const Placement& from, Square to) : m_from{from}, m_to{to}
{
}

inline Relocation::Relocation(std::string_view notation) : m_from{notation}
{
   m_to = makeSquare(notation.substr(isPawn(m_from.piece()) ? 3 : 4));
}

inline Relocation Relocation::reverse() const
{
   return {piece(), to(), from()};
}

inline bool operator==(const Relocation& a, const Relocation& b)
{
   return a.placement() == b.placement() && a.to() == b.to();
}

inline bool operator!=(const Relocation& a, const Relocation& b)
{
   return !(a == b);
}

} // namespace matt2

