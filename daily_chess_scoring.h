//
// Jan-2023, Michael Lindner
// MIT license
//
#pragma once

namespace matt2
{
class Position;
}

namespace matt2
{

// Score based on rules by Daily Chess website.
// https://www.dailychess.com/rival/programming/evaluation.php
double calcDailyChessScore(const Position& pos);

} // namespace matt2
