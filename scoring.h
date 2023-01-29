//
// Sept-2021, Michael Lindner
// MIT license
//
#pragma once
#include <limits>

namespace matt2
{
class Position;
}


namespace matt2
{
///////////////////

inline bool isBetterScore(double a, double b, bool calcMax)
{
   return calcMax ? a > b : a < b;
}

inline double getWorstScoreValue(bool calcMax)
{
   return calcMax ? std::numeric_limits<double>::lowest()
                  : std::numeric_limits<double>::max();
}

double calcScore(const Position& pos);

} // namespace matt2
