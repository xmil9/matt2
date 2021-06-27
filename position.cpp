//
// Jun-2021, Michael Lindner
// MIT license
//
#include "position.h"
#include <cassert>
#include <vector>

using namespace matt2;


namespace
{
///////////////////

static constexpr char PieceDelimCh = ' ';
static constexpr std::size_t PieceDelimLen = 1;

} // namespace


namespace matt2
{
///////////////////

Position::Position(std::string_view placements)
{
   populate(placements);
}


void Position::populate(std::string_view placements)
{
   std::size_t pos = 0;
   std::size_t next = placements.find(PieceDelimCh, pos);
   while (next != -1)
   {
      if (next > pos)
         add(placements.substr(pos, next - pos));

      pos = next + PieceDelimLen;
      next = placements.find(PieceDelimCh, pos);
   }

   // Process last placement.
   if (pos < placements.size())
      add(placements.substr(pos));
}


void Position::add(const Placement& placement)
{
   m_board[toIdx(placement.at())] = placement.piece();
   m_pieces[toColorIdx(placement.piece())].add(placement);
}


void Position::remove(const Placement& placement)
{
   m_board[toIdx(placement.at())] = std::nullopt;
   m_pieces[toColorIdx(placement.piece())].remove(placement);
}


void Position::move(const Relocation& relocation)
{
   m_board[toIdx(relocation.from())] = std::nullopt;
   m_board[toIdx(relocation.to())] = relocation.piece();

   m_pieces[toColorIdx(relocation.piece())].move(relocation.placement(), relocation.to());
}


///////////////////

void Position::PiecePlacements::add(const Placement& placement)
{
   const Piece& piece = placement.piece();
   const Square& at = placement.at();

   if (isKing(piece))
   {
      m_king = at;
   }
   else if (isQueen(piece))
   {
      m_queens.add(at);
   }
   else if (isRook(piece))
   {
      m_rooks.add(at);
   }
   else if (isBishop(piece))
   {
      m_bishops.add(at);
   }
   else if (isKnight(piece))
   {
      m_knights.add(at);
   }
   else
   {
      assert(isPawn(piece));
      m_pawns.add(at);
   }
}


void Position::PiecePlacements::remove(const Placement& placement)
{
   const Piece& piece = placement.piece();
   const Square& at = placement.at();

   if (isKing(piece))
   {
      m_king = std::nullopt;
   }
   else if (isQueen(piece))
   {
      m_queens.remove(at);
   }
   else if (isRook(piece))
   {
      m_rooks.remove(at);
   }
   else if (isBishop(piece))
   {
      m_bishops.remove(at);
   }
   else if (isKnight(piece))
   {
      m_knights.remove(at);
   }
   else
   {
      assert(isPawn(piece));
      m_pawns.remove(at);
   }
}


void Position::PiecePlacements::move(const Placement& from, Square to)
{
   const Piece& piece = from.piece();
   const Square& at = from.at();

   if (isKing(piece))
   {
      m_king = to;
   }
   else if (isQueen(piece))
   {
      m_queens.move(at, to);
   }
   else if (isRook(piece))
   {
      m_rooks.move(at, to);
   }
   else if (isBishop(piece))
   {
      m_bishops.move(at, to);
   }
   else if (isKnight(piece))
   {
      m_knights.move(at, to);
   }
   else
   {
      assert(isPawn(piece));
      m_pawns.move(at, to);
   }
}


std::vector<Square> Position::PiecePlacements::locations(Piece piece) const
{
   if (isKing(piece))
   {
      if (m_king.has_value())
         return {*m_king};
   }
   else if (isQueen(piece))
   {
      return m_queens.locations();
   }
   else if (isRook(piece))
   {
      return m_rooks.locations();
   }
   else if (isBishop(piece))
   {
      return m_bishops.locations();
   }
   else if (isKnight(piece))
   {
      return m_knights.locations();
   }
   else if (isPawn(piece))
   {
      return m_pawns.locations();
   }

   return {};
}


///////////////////

// clang-format off
const Position StartPos{
   "Rba8 Nbb8 Bbc8 Qbd8 Kbe8 Bbf8 Nbg8 Rbh8 "
   "ba7  bb7  bc7  bd7  be7  bf7  bg7  bh7 "
   "wa2  wb2  wc2  wd2  we2  wf2  wg2  wh2 "
   "Rwa1 Nwb1 Bwc1 Qwd1 Kwe1 Bwf1 Nwg1 Rwh1"
};
// clang-format on

} // namespace matt2
