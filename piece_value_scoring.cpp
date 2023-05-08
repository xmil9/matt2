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

double scoreMate(const Position& /*pos*/, size_t atDepth, Color side,
                 const std::optional<PieceValueTable>& values)
{
   const PieceValueTable& valueTable = values.has_value() ? *values : DefaultPieceValues;

   // Use king value as base score and adjust for the depth at which the mate happens to
   // encourage faster mates over later ones.
   const double amount = lookupValue(king(side), valueTable) - atDepth;

   // Needs to be a bad score for the side passed in.
   const double sign = side == White ? -1. : 1.;

   return sign * amount;
}

double scoreTie(const Position& pos, Color /*side*/,
                const std::optional<PieceValueTable>& values)
{
   // Use regular position score.
   const double amount = score(pos, values);

   // A tie is bad for the side with a good score, so flip it.
   return -1. * amount;
}

} // namespace pvs
} // namespace matt2
