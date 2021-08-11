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

void collectCastlingMoves(Color side, const Position& pos, std::vector<Move>& moves);
void collectEnPassantMoves(Color side, const Position& pos, std::vector<Move>& moves);


///////////////////

void collectAttackedByKing(Piece king, Square at, const Position& pos,
                           std::vector<Square>& attacked);
void collectAttackedByQueen(Piece queen, Square at, const Position& pos,
                            std::vector<Square>& attacks);
void collectAttackedByRook(Piece rook, Square at, const Position& pos,
                           std::vector<Square>& attacks);
void collectAttackedByBishop(Piece bishop, Square at, const Position& pos,
                             std::vector<Square>& attacks);
void collectAttackedByKnight(Piece knight, Square at, const Position& pos,
                             std::vector<Square>& attacks);
void collectAttackedByPawn(Piece pawn, Square at, const Position& pos,
                           std::vector<Square>& attacks);

} // namespace matt2