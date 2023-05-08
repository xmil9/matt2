//
// Sept-2021, Michael Lindner
// MIT license
//
#include "scoring.h"
#include "daily_chess_scoring.h"

namespace matt2
{
///////////////////

double calcScore(const Position& pos)
{
   return dcs::score(pos);
}

double calcMateScore(Color side, const Position& pos, size_t atDepth)
{
   return dcs::scoreMate(pos, atDepth, side);
}

double calcTieScore(Color side, const Position& pos)
{
   return dcs::scoreTie(pos, side);
}

} // namespace matt2
