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

double calcPieceValueScore(const Position& pos);
std::optional<double> calcMateScore(const Position& pos);

} // namespace matt2
