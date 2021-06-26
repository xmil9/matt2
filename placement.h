//
// Jun-2021, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include "square.h"
#include <string>


namespace matt2
{
///////////////////

// Represents a piece and a location.
class Placement
{
 public:
   Placement(Piece piece, Square at);
   // Notation examples: "Kbb3", "Rwh1", "bc7"
   explicit Placement(std::string_view notation);

   Piece piece() const { return m_piece; }
   Square at() const { return m_at; }

 private:
   Piece m_piece;
   Square m_at;
};


inline Placement::Placement(Piece piece, Square at) : m_piece{piece}, m_at{at}
{
}

inline Placement::Placement(std::string_view notation) : m_piece{makePiece(notation)}
{
   m_at = makeSquare(notation.substr(isPawn(m_piece) ? 1 : 2));
}

inline bool operator==(const Placement& a, const Placement& b)
{
   return a.piece() == b.piece() && a.at() == b.at();
}

inline bool operator!=(const Placement& a, const Placement& b)
{
   return !(a == b);
}

} // namespace matt2
