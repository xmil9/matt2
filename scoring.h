//
// Sept-2021, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include <limits>

namespace matt2
{
class Position;
}


namespace matt2
{
///////////////////

// Better score than.
inline bool bt(double a, double b, bool calcMax)
{
   return calcMax ? a > b : a < b;
}

inline bool bt(double a, double b, Color side)
{
   return bt(a, b, side == White);
}

// Compare scores: 1, 0, -1
inline int cmp(double a, double b, bool calcMax)
{
   if (a == b)
      return 0;
   return bt(a, b, calcMax) ? 1 : -1;
}

inline int cmp(double a, double b, Color side)
{
   return cmp(a, b, side == White);
}

// Get worst score value.
inline double getWorstScoreValue(bool calcMax)
{
   return calcMax ? std::numeric_limits<double>::lowest()
                  : std::numeric_limits<double>::max();
}

inline double getWorstScoreValue(Color side)
{
   return getWorstScoreValue(side == White);
}

// Calculate the score of a given position.
double calcScore(const Position& pos);
double calcMateScore(Color side, const Position& pos, size_t atDepth);
double calcTieScore(Color side, const Position& pos);

} // namespace matt2
