//
// Jul-2021, Michael Lindner
// MIT license
//
#pragma once
#include "move.h"
#include "piece.h"
#include "position.h"
#include "square.h"
#include <vector>


namespace matt2
{
///////////////////

// Collect moves that are possible for a given piece.
void collectKingMoves(Piece king, Square at, const Position& pos,
                      std::vector<Move>& moves);
void collectQueenMoves(Piece queen, Square at, const Position& pos,
                       std::vector<Move>& moves);
void collectRookMoves(Piece rook, Square at, const Position& pos,
                      std::vector<Move>& moves);
void collectBishopMoves(Piece bishop, Square at, const Position& pos,
                        std::vector<Move>& moves);
void collectKnightMoves(Piece knight, Square at, const Position& pos,
                        std::vector<Move>& moves);
void collectPawnMoves(Piece pawn, Square at, const Position& pos,
                      std::vector<Move>& moves);

// Collect moves for special situations.
void collectCastlingMoves(Color side, const Position& pos, std::vector<Move>& moves);
void collectEnPassantMoves(Color side, const Position& pos, std::vector<Move>& moves);


///////////////////

// Collect squares that can be reached by a piece and are either empty or occupied by an
// opponent's piece.
void collectAttackedByKing(Piece king, Square at, const Position& pos,
                           std::vector<Square>& attacked);
void collectAttackedByQueen(Piece queen, Square at, const Position& pos,
                            std::vector<Square>& attacked);
void collectAttackedByRook(Piece rook, Square at, const Position& pos,
                           std::vector<Square>& attacked);
void collectAttackedByBishop(Piece bishop, Square at, const Position& pos,
                             std::vector<Square>& attacked);
void collectAttackedByKnight(Piece knight, Square at, const Position& pos,
                             std::vector<Square>& attacked);
void collectAttackedByPawn(Piece pawn, Square at, const Position& pos,
                           std::vector<Square>& attacked);

// Output collection is sorted.
void collectAttackedBySide(Color side, const Position& pos,
                           std::vector<Square>& attacked);

} // namespace matt2
