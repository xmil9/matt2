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
///////////////////

// Lookup table for piece values. Indexed by Piece values.
using PieceValueTable = std::array<double, 12>;

// Simplify lookup for individual piece.
inline double lookupValue(Piece piece, const PieceValueTable& pieceValues)
{
   return pieceValues[static_cast<size_t>(piece)];
}

///////////////////

// Calculate score for one type of piece.
double
calcPieceValueScore(const Position& pos, Piece piece,
                    const std::optional<PieceValueTable>& valueTable = std::nullopt);

// Calculate score for one side.
double
calcPieceValueScore(const Position& pos, Color side,
                    const std::optional<PieceValueTable>& pieceValues = std::nullopt);

// Calculate score for entire position.
double
calcPieceValueScore(const Position& pos,
                    const std::optional<PieceValueTable>& valueTable = std::nullopt);

} // namespace matt2
