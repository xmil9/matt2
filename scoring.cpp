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
   return calcDailyChessScore(pos);
}

} // namespace matt2
