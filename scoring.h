//
// Sept-2021, Michael Lindner
// MIT license
//
#pragma once
#include <optional>

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

inline double getWorstScore(bool calcMax)
{
   return calcMax ? std::numeric_limits<double>::lowest()
                  : std::numeric_limits<double>::max();
}

// Score based only on piece values.
double calcPieceValueScore(const Position& pos);

// Score based on rules by Daily Chess website.
// https://www.dailychess.com/rival/programming/evaluation.php
double calcDailyChessScore(const Position& pos);

} // namespace matt2
