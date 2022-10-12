//
// Jun-2021, Michael Lindner
// MIT license
//
#include "position.h"
#include "rules.h"
#include "scoring.h"
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
   
   invalidateScore();
}


void Position::remove(const Placement& placement)
{
   m_board[toIdx(placement.at())] = std::nullopt;
   m_pieces[toColorIdx(placement.piece())].remove(placement);
   
   invalidateScore();
}


void Position::move(const Relocation& relocation)
{
   m_board[toIdx(relocation.from())] = std::nullopt;
   m_board[toIdx(relocation.to())] = relocation.piece();

   m_pieces[toColorIdx(relocation.piece())].move(relocation.placement(), relocation.to());

   invalidateScore();
}

double Position::updateScore()
{
   m_score = calcPieceValueScore(*this);
   return *m_score;
}


bool Position::canAttack(Square sq, Color side) const
{
   for (auto it = begin(side), endIt = end(side); it != endIt; ++it)
      if (canAttack(sq, *it))
         return true;
   return false;
}


bool Position::canAttack(Square sq, const Placement& placement) const
{
   const auto& piece = placement.piece();
   const auto& at = placement.at();

   // todo - cache the attacked squares and inval cache after each move
   std::vector<Square> attacked;
   // Reserve some space to avoid too many allocations.
   attacked.reserve(100);

   if (isKing(piece))
      collectAttackedByKing(piece, at, *this, attacked);
   else if (isQueen(piece))
      collectAttackedByQueen(piece, at, *this, attacked);
   else if (isRook(piece))
      collectAttackedByRook(piece, at, *this, attacked);
   else if (isBishop(piece))
      collectAttackedByBishop(piece, at, *this, attacked);
   else if (isKnight(piece))
      collectAttackedByKnight(piece, at, *this, attacked);
   else if (isPawn(piece))
      collectAttackedByPawn(piece, at, *this, attacked);
   else
      throw std::runtime_error("Unknown piece.");

   return std::find(std::begin(attacked), std::end(attacked), sq) != std::end(attacked);
}


///////////////////

void Position::ColorPlacements::add(const Placement& placement)
{
   const Piece& piece = placement.piece();
   const Square& at = placement.at();

   if (isKing(piece))
   {
      m_king = at;
      initKingMovedFlag(color(piece), at);
   }
   else if (isQueen(piece))
   {
      m_queens.add(at);
   }
   else if (isRook(piece))
   {
      m_rooks.add(at);
      initRookMovedFlag(color(piece));
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


void Position::ColorPlacements::remove(const Placement& placement)
{
   const Piece& piece = placement.piece();
   const Square& at = placement.at();

   if (isKing(piece))
   {
      m_king = std::nullopt;
      m_hasKingMoved = true;
   }
   else if (isQueen(piece))
   {
      m_queens.remove(at);
   }
   else if (isRook(piece))
   {
      m_rooks.remove(at);
      updateRookMovedFlag(at);
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


void Position::ColorPlacements::move(const Placement& from, Square to)
{
   const Piece& piece = from.piece();
   const Square& at = from.at();

   if (isKing(piece))
   {
      m_king = to;
      m_hasKingMoved = true;
   }
   else if (isQueen(piece))
   {
      m_queens.move(at, to);
   }
   else if (isRook(piece))
   {
      m_rooks.move(at, to);
      updateRookMovedFlag(at);
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


std::vector<Square> Position::ColorPlacements::locations(Piece piece) const
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


Square Position::ColorPlacements::placement(std::size_t idx) const
{
   std::size_t numPieces = m_rooks.count();
   if (idx < numPieces)
      return m_rooks[idx];
   idx -= numPieces;

   numPieces = m_bishops.count();
   if (idx < numPieces)
      return m_bishops[idx];
   idx -= numPieces;

   numPieces = m_knights.count();
   if (idx < numPieces)
      return m_knights[idx];
   idx -= numPieces;

   numPieces = m_queens.count();
   if (idx < numPieces)
      return m_queens[idx];
   idx -= numPieces;

   numPieces = m_pawns.count();
   if (idx < numPieces)
      return m_pawns[idx];
   idx -= numPieces;

   numPieces = m_king.has_value() ? 1 : 0;
   if (idx < numPieces)
      return *m_king;

   throw std::out_of_range("Invalid index for placement in position.");
}


void Position::ColorPlacements::initRookMovedFlag(Color side)
{
   const Square kingsideLoc = side == Color::White ? h1 : h8;
   const Square queensideLoc = side == Color::White ? a1 : a8;

   // Init flags for both rooks based on the current state of the position.
   m_hasKingsideRookMoved = true;
   m_hasQueensideRookMoved = true;

   for (std::size_t i = 0; i < m_rooks.count(); ++i)
   {
      if (m_rooks[i] == kingsideLoc)
         m_hasKingsideRookMoved = false;
      if (m_rooks[i] == queensideLoc)
         m_hasQueensideRookMoved = false;
   }
}


void Position::ColorPlacements::updateRookMovedFlag(Square from)
{
   if (!m_hasKingsideRookMoved && file(from) == fh)
      m_hasKingsideRookMoved = true;
   if (!m_hasQueensideRookMoved && file(from) == fa)
      m_hasQueensideRookMoved = true;
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
