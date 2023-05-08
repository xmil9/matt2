//
// Jan-2023, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"

namespace matt2
{
class Position;
}

// Scoring policy based on rules by Daily Chess website.
// https://www.dailychess.com/rival/programming/evaluation.php

namespace matt2
{
namespace dcs
{
///////////////////

// Flags for individual scoring rules
// Exposed to allow more focused unit testing.
enum class Rules : uint64_t
{
   // Pawn
   PawnPieceValue = 0x01000001,
   PawnPositionBonus = 0x01000002,
   PassedPawnBonus = 0x01000004,
   DoublePawnPenalty = 0x01000008,
   IsolatedPawnPenalty = 0x01000010,
   // Knight
   KnightPieceValue = 0x02000001,
   KnightCenterBonus = 0x02000002,
   KnightKingClosenessBonus = 0x02000004,
   // Bishop
   BishopPieceValue = 0x04000001,
   MultipleBishopBonus = 0x04000002,
   BishopAdjacentPawnPenality = 0x04000004,
   // Rook
   RookPieceValue = 0x08000001,
   RookKingClosenessBonus = 0x08000002,
   RookSeventhRankBonus = 0x08000004,
   RookSharedFileBonus = 0x08000008,
   RookPawnsOnFileBonus = 0x08000010,
   // Queen
   QueenPieceValue = 0x10000001,
   QueenKingClosenessValue = 0x10000002,
   QueenBishopDiagonalClosenessValue = 0x10000004,
   // King
   KingPieceValue = 0x20000001,
   KingQuadrantPenalty = 0x20000002,
   KingCastlingPenalty = 0x20000004,
   // All
   All = 0xffffffff
};

double score(const Position& pos, Color side, Rules rules = Rules::All);
double score(const Position& pos, Rules rules = Rules::All);
double scoreMate(const Position& pos, size_t atDepth, Color side);
double scoreTie(const Position& pos, Color side);

} // namespace dcs
} // namespace matt2
