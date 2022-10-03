//
// May-2021, Michael Lindner
// MIT license
//
#pragma once
#include <string>


namespace matt2
{
///////////////////

enum class Color
{
   White,
   Black
};


inline Color operator!(Color c)
{
   return c == Color::White ? Color::Black : Color::White;
}


///////////////////

// Unique identifier for each chess piece.
// Enum for type safety.
// clang-format off
enum class Piece : unsigned char
{
   Kw, Qw, Rw, Bw, Nw, Pw,
   Kb, Qb, Rb, Bb, Nb, Pb
};
// clang-format on

// Aliases for convenience.
constexpr Piece Kw = Piece::Kw;
constexpr Piece Qw = Piece::Qw;
constexpr Piece Rw = Piece::Rw;
constexpr Piece Bw = Piece::Bw;
constexpr Piece Nw = Piece::Nw;
constexpr Piece Pw = Piece::Pw;
constexpr Piece Kb = Piece::Kb;
constexpr Piece Qb = Piece::Qb;
constexpr Piece Rb = Piece::Rb;
constexpr Piece Bb = Piece::Bb;
constexpr Piece Nb = Piece::Nb;
constexpr Piece Pb = Piece::Pb;


// Create a piece from a given notation. A piece notation consists of a code for
// the figure, e.g. 'K', 'Q', 'B', and a code for the piece color, e.g. 'w', 'b'.
// Examples: "Kb", "Rw", "b" (for black pawn).
Piece makePiece(std::string_view notation);

inline bool isKing(Piece p) { return p == Kw || p == Kb; }
inline bool isQueen(Piece p) { return p == Qw || p == Qb; }
inline bool isRook(Piece p) { return p == Rw || p == Rb; }
inline bool isBishop(Piece p) { return p == Bw || p == Bb; }
inline bool isKnight(Piece p) { return p == Nw || p == Nb; }
inline bool isPawn(Piece p) { return p == Pw || p == Pb; }

inline Color color(Piece p) { return p < Kb ? Color::White : Color::Black; }
inline bool isWhite(Piece p) { return color(p) == Color::White; }
inline bool isBlack(Piece p) { return color(p) == Color::Black; }
inline bool haveSameColor(Piece a, Piece b) { return color(a) == color(b); }

} // namespace matt2
