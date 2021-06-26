//
// Jun-2021, Michael Lindner
// MIT license
//
#include "game.h"


namespace matt2
{
///////////////////

const Position& Game::forward()
{
   makeMove(m_currPos, m_moves[++m_currMove]);
   return m_currPos;
}


const Position& Game::backward()
{
   reverseMove(m_currPos, m_moves[m_currMove--]);
   return m_currPos;
}

} // namespace matt2
