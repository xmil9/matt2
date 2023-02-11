//
// Jan-2023, Michael Lindner
// MIT license
//
#include "piece_value_scoring.h"
#include "piece.h"
#include "position.h"
#include "square.h"
#include <numeric>
#include <stdexcept>

namespace matt2
{
namespace pvs
{
///////////////////

// Default piece values.
constexpr double KingValue = 10000.;
constexpr double QueenValue = 900.;
constexpr double RookValue = 500.;
constexpr double BishopValue = 340.;
constexpr double KnightValue = 325.;
constexpr double PawnValue = 100.;

// Default lookup table.
constexpr PieceValueTable DefaultPieceValues{
   KingValue, QueenValue, RookValue, BishopValue, KnightValue, PawnValue,
   KingValue, QueenValue, RookValue, BishopValue, KnightValue, PawnValue,
};

///////////////////

double score(const Position& pos, Piece piece,
             const std::optional<PieceValueTable>& values)
{
   const PieceValueTable& valueTable = values.has_value() ? *values : DefaultPieceValues;

   return std::accumulate(pos.begin(piece), pos.end(piece), 0.,
                          [piece, &valueTable](double currValue, Square /*sq*/)
                          { return currValue + lookupValue(piece, valueTable); });
}

double score(const Position& pos, Color side,
             const std::optional<PieceValueTable>& values)
{
   const PieceValueTable& valueTable = values.has_value() ? *values : DefaultPieceValues;

   return std::accumulate(
      pos.begin(side), pos.end(side), 0.,
      [&valueTable](double currValue, const Placement& placement)
      { return currValue + lookupValue(placement.piece(), valueTable); });
}

double score(const Position& pos, const std::optional<PieceValueTable>& values)
{
   return score(pos, White, values) - score(pos, Black, values);
}

} // namespace pvs
} // namespace matt2
