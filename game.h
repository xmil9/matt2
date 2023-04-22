//
// May-2021, Michael Lindner
// MIT license
//
#pragma once
#include "move.h"
#include "position.h"
#include <cstddef>
#include <optional>
#include <utility>
#include <vector>


namespace matt2
{

struct MoveResult;

///////////////////

class Game
{
 public:
   Game();
   Game(Position pos, Color nextTurn);

   // Taking turns.
   Color nextTurn() const { return m_nextTurn; }
   MoveResult calcNextMove(size_t turnDepth);
   MoveResult enterNextMove(std::string_view movePacnNotation);

   // Iterate over game positions.
   const Position& current() const { return m_currPos; }
   std::optional<Position> forward();
   std::optional<Position> backward();

   // Access the move history.
   size_t countMoves() const { return m_moves.size(); }
   const Move& getMove(size_t idx) const { return m_moves[idx]; }
   // Returns -1, if no move has been made.
   size_t currentMoveIdx() const { return m_currMove; }

 private:
   bool atStart() const { return m_currMove == -1; }
   bool atEnd() const { return m_moves.empty() || m_currMove == m_moves.size() - 1; }
   void switchTurn() { m_nextTurn = !m_nextTurn; }

   // Removes all moves after the current move from the history.
   void trimMoves();

   // Converts a given move description to an actual move that can be applied to the
   // current position. Returns failure description.
   std::pair<std::optional<Move>, std::string>
   buildMove(const MoveDescription& moveDescr) const;

   // Applies a given move.
   void apply(Move& m);

 private:
   // Side that has the next turn.
   Color m_nextTurn = Color::White;
   // Current position.
   Position m_currPos;
   // History of moves.
   std::vector<Move> m_moves;
   // Index of move that leads to current position.
   size_t m_currMove = static_cast<size_t>(-1);
};


inline Game::Game() : m_currPos{StartPos}
{
}

inline Game::Game(Position pos, Color nextTurn)
: m_nextTurn{nextTurn}, m_currPos{std::move(pos)}
{
}

///////////////////

struct MoveResult
{
   static MoveResult success(const std::string& notation, const Position& p);
   static MoveResult failure(const std::string& error, const Position& p);

   bool ok;
   std::string descr;
   const Position& pos;
};

inline MoveResult MoveResult::success(const std::string& notation, const Position& p)
{
   return {true, notation, p};
}

inline MoveResult MoveResult::failure(const std::string& error, const Position& p)
{
   return {false, error, p};
}

} // namespace matt2
