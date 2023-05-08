//
// Jan-2023, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include <array>
#include <optional>

namespace matt2
{
class Position;
}

namespace matt2
{
namespace pvs
{
///////////////////

// Lookup table for piece values. Indexed by Piece values.
using PieceValueTable = std::array<double, 12>;

// Simplify lookup for individual piece.
inline double lookupValue(Piece piece, const PieceValueTable& values)
{
   return values[static_cast<size_t>(piece)];
}

///////////////////

// Calculate score for one type of piece.
double score(const Position& pos, Piece piece,
             const std::optional<PieceValueTable>& values = std::nullopt);

// Calculate score for one side.
double score(const Position& pos, Color side,
             const std::optional<PieceValueTable>& values = std::nullopt);

// Calculate score for entire position.
double score(const Position& pos,
             const std::optional<PieceValueTable>& values = std::nullopt);

// Calculate score for a mate.
double scoreMate(const Position& pos, size_t atDepth, Color side,
                 const std::optional<PieceValueTable>& values = std::nullopt);

// Calculate score for a tie.
double scoreTie(const Position& pos, Color side,
                const std::optional<PieceValueTable>& values = std::nullopt);

} // namespace pvs
} // namespace matt2
