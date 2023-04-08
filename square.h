//
// May-2021, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include <cassert>
#include <string>

#ifdef min
#undef min
#endif

namespace matt2
{
///////////////////

// Identifiers for each file of a chess board.
// clang-format off
enum class File : unsigned char
{
   fa, fb, fc, fd, fe, ff, fg, fh
};
// clang-format on

// Aliases for convenience.
constexpr File fa = File::fa;
constexpr File fb = File::fb;
constexpr File fc = File::fc;
constexpr File fd = File::fd;
constexpr File fe = File::fe;
constexpr File ff = File::ff;
constexpr File fg = File::fg;
constexpr File fh = File::fh;


inline bool isValid(File f)
{
   // File enum is based on an unsigned value and can never be less than zero.
   return static_cast<unsigned char>(f) <= static_cast<unsigned char>(fh);
}

inline bool isValid(File f, int offset)
{
   return isValid(static_cast<File>(static_cast<int>(f) + offset));
}

inline File operator+(File f, int inc)
{
   return static_cast<File>(static_cast<unsigned char>(f) + inc);
}

inline File operator-(File f, int dec)
{
   return static_cast<File>(static_cast<unsigned char>(f) - dec);
}

inline char toLowercaseChar(File f)
{
   return 'a' + static_cast<char>(f);
}

inline File fileFromChar(char c)
{
   return static_cast<File>(c - 'a');
}

// Is file a lower than file b and adjacent to it?
inline bool isLowerAdjacent(File a, File b)
{
   return isValid(a, 1) && a + 1 == b;
}

// Is file a higher than file b and adjacent to it?
inline bool isHigherAdjacent(File a, File b)
{
   return isValid(a, -1) && a - 1 == b;
}

// Is file a adjacent to file b?
inline bool isAdjacent(File a, File b)
{
   return isLowerAdjacent(a, b) || isHigherAdjacent(a, b);
}

inline int distance(File a, File b)
{
   return static_cast<char>(a) - static_cast<char>(b);
}

///////////////////

// Identifiers for each rank of a chess board.
// clang-format off
enum class Rank : unsigned char
{
   r1, r2, r3, r4, r5, r6, r7, r8
};
// clang-format on

// Aliases for convenience.
constexpr Rank r1 = Rank::r1;
constexpr Rank r2 = Rank::r2;
constexpr Rank r3 = Rank::r3;
constexpr Rank r4 = Rank::r4;
constexpr Rank r5 = Rank::r5;
constexpr Rank r6 = Rank::r6;
constexpr Rank r7 = Rank::r7;
constexpr Rank r8 = Rank::r8;


inline bool isValid(Rank r)
{
   // File enum is based on an unsigned value and can never be less than zero.
   return static_cast<unsigned char>(r) <= static_cast<unsigned char>(r8);
}

inline bool isValid(Rank r, int offset)
{
   return isValid(static_cast<Rank>(static_cast<int>(r) + offset));
}

inline Rank operator+(Rank r, int inc)
{
   return static_cast<Rank>(static_cast<unsigned char>(r) + inc);
}

inline Rank operator-(Rank r, int dec)
{
   return static_cast<Rank>(static_cast<unsigned char>(r) - dec);
}

inline char toLowercaseChar(Rank r)
{
   return '1' + static_cast<char>(r);
}

inline Rank rankFromChar(char c)
{
   return static_cast<Rank>(c - '1');
}

// Is rank a lower than rank b and adjacent to it?
inline bool isLowerAdjacent(Rank a, Rank b)
{
   return isValid(a, 1) && a + 1 == b;
}

// Is rank a higher than rank b and adjacent to it?
inline bool isHigherAdjacent(Rank a, Rank b)
{
   return isValid(a, -1) && a - 1 == b;
}

// Is rank a adjacent to rank b?
inline bool isAdjacent(Rank a, Rank b)
{
   return isLowerAdjacent(a, b) || isHigherAdjacent(a, b);
}

inline int distance(Rank a, Rank b)
{
   return static_cast<char>(a) - static_cast<char>(b);
}

///////////////////

// Identifier for each square on the board.
// Enum for type safety.
// clang-format off
enum class Square : unsigned char
{
   a1, a2, a3, a4, a5, a6, a7, a8,
   b1, b2, b3, b4, b5, b6, b7, b8,
   c1, c2, c3, c4, c5, c6, c7, c8,
   d1, d2, d3, d4, d5, d6, d7, d8,
   e1, e2, e3, e4, e5, e6, e7, e8,
   f1, f2, f3, f4, f5, f6, f7, f8,
   g1, g2, g3, g4, g5, g6, g7, g8,
   h1, h2, h3, h4, h5, h6, h7, h8
};
// clang-format on

// Aliases for convenience.
constexpr Square a1 = Square::a1;
constexpr Square a2 = Square::a2;
constexpr Square a3 = Square::a3;
constexpr Square a4 = Square::a4;
constexpr Square a5 = Square::a5;
constexpr Square a6 = Square::a6;
constexpr Square a7 = Square::a7;
constexpr Square a8 = Square::a8;
constexpr Square b1 = Square::b1;
constexpr Square b2 = Square::b2;
constexpr Square b3 = Square::b3;
constexpr Square b4 = Square::b4;
constexpr Square b5 = Square::b5;
constexpr Square b6 = Square::b6;
constexpr Square b7 = Square::b7;
constexpr Square b8 = Square::b8;
constexpr Square c1 = Square::c1;
constexpr Square c2 = Square::c2;
constexpr Square c3 = Square::c3;
constexpr Square c4 = Square::c4;
constexpr Square c5 = Square::c5;
constexpr Square c6 = Square::c6;
constexpr Square c7 = Square::c7;
constexpr Square c8 = Square::c8;
constexpr Square d1 = Square::d1;
constexpr Square d2 = Square::d2;
constexpr Square d3 = Square::d3;
constexpr Square d4 = Square::d4;
constexpr Square d5 = Square::d5;
constexpr Square d6 = Square::d6;
constexpr Square d7 = Square::d7;
constexpr Square d8 = Square::d8;
constexpr Square e1 = Square::e1;
constexpr Square e2 = Square::e2;
constexpr Square e3 = Square::e3;
constexpr Square e4 = Square::e4;
constexpr Square e5 = Square::e5;
constexpr Square e6 = Square::e6;
constexpr Square e7 = Square::e7;
constexpr Square e8 = Square::e8;
constexpr Square f1 = Square::f1;
constexpr Square f2 = Square::f2;
constexpr Square f3 = Square::f3;
constexpr Square f4 = Square::f4;
constexpr Square f5 = Square::f5;
constexpr Square f6 = Square::f6;
constexpr Square f7 = Square::f7;
constexpr Square f8 = Square::f8;
constexpr Square g1 = Square::g1;
constexpr Square g2 = Square::g2;
constexpr Square g3 = Square::g3;
constexpr Square g4 = Square::g4;
constexpr Square g5 = Square::g5;
constexpr Square g6 = Square::g6;
constexpr Square g7 = Square::g7;
constexpr Square g8 = Square::g8;
constexpr Square h1 = Square::h1;
constexpr Square h2 = Square::h2;
constexpr Square h3 = Square::h3;
constexpr Square h4 = Square::h4;
constexpr Square h5 = Square::h5;
constexpr Square h6 = Square::h6;
constexpr Square h7 = Square::h7;
constexpr Square h8 = Square::h8;


// Create a square from a given notation. A square notation consists of a code for
// the file, e.g. 'c', 'h', and a code for the rank, e.g. '2', '7'.
// Examples: "f6", "b8".
Square makeSquare(std::string_view notation);

// Create square from file and rank.
inline Square makeSquare(File f, Rank r)
{
   return static_cast<Square>(static_cast<unsigned char>(f) * 8 +
                              static_cast<unsigned char>(r));
}

inline File file(Square sq)
{
   return static_cast<File>(static_cast<unsigned char>(sq) / 8);
}

inline Rank rank(Square sq)
{
   return static_cast<Rank>(static_cast<unsigned char>(sq) % 8);
}

inline bool isValid(Square sq)
{
   // Square enum is based on an unsigned value and can never be less than zero.
   return static_cast<unsigned char>(sq) <= static_cast<unsigned char>(h8);
}

inline Square operator++(Square& sq)
{
   sq = static_cast<Square>((static_cast<std::size_t>(sq) + 1) % 64);
   return sq;
}

///////////////////

using Diagonal = char;

// Diagonal going upwards from lower to higher files.
inline Diagonal upDiagonal(Square sq)
{
   return static_cast<char>(file(sq)) - static_cast<char>(rank(sq));
}

// Diagonal going downwards from lower to higher files.
inline Diagonal downDiagonal(Square sq)
{
   return static_cast<char>(rank(sq)) + static_cast<char>(file(sq));
}

inline bool onSameUpDiagonal(Square a, Square b)
{
   return upDiagonal(a) == upDiagonal(b);
}

inline bool onSameDownDiagonal(Square a, Square b)
{
   return downDiagonal(a) == downDiagonal(b);
}

inline bool onSameDiagonal(Square a, Square b)
{
   return onSameUpDiagonal(a, b) || onSameDownDiagonal(a, b);
}

///////////////////

struct Offset
{
   int df = 0;
   int dr = 0;
};

inline bool operator==(Offset a, Offset b)
{
   return a.df == b.df && a.dr == b.dr;
}

inline bool operator!=(Offset a, Offset b)
{
   return !(a == b);
}

inline bool isOnBoard(Square sq, Offset off)
{
   return isValid(file(sq), off.df) && isValid(rank(sq), off.dr);
}

inline Square operator+(Square sq, Offset off)
{
   return makeSquare(file(sq) + off.df, rank(sq) + off.dr);
}

inline Offset offset(Square a, Square b)
{
   return {distance(file(a), file(b)), distance(rank(a), rank(b))};
}

// Calculates the minimum absolute distance across file and rank between two squares.
inline int minDistance(Square a, Square b)
{
   const Offset off = offset(a, b);
   return std::min(std::abs(off.df), std::abs(off.dr));
}

///////////////////

enum class Quadrant
{
   a1,
   a8,
   h1,
   h8
};

inline Quadrant quadrant(Square sq)
{
   const File f = static_cast<char>(file(sq)) <= static_cast<char>(fd) ? fa : fh;
   const Rank r = static_cast<char>(rank(sq)) <= static_cast<char>(r4) ? r1 : r8;
   if (f == fa)
      return r == r1 ? Quadrant::a1 : Quadrant::a8;
   return r == r1 ? Quadrant::h1 : Quadrant::h8;
}

inline bool inQuadrant(Square sq, Quadrant q)
{
   return quadrant(sq) == q;
}

inline bool isFriendlyQuadrant(Quadrant q, Color side)
{
   return (side == White && (q == Quadrant::a1 || q == Quadrant::h1)) ||
          (side == Black && (q == Quadrant::a8 || q == Quadrant::h8));
}

} // namespace matt2
