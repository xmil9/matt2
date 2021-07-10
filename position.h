//
// May-2021, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include "placement.h"
#include "relocation.h"
#include "square.h"
#include <array>
#include <cassert>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <vector>

// clang-format off
namespace matt2 { class PlacementIterator; }
// clang-format on


namespace matt2
{
///////////////////

// Represents a position of pieces on the chess board.
class Position
{
   friend class PlacementIterator;

 public:
   Position() = default;
   explicit Position(std::string_view placements);

   // Returns piece at given location on board.
   std::optional<Piece> operator[](Square at) const { return m_board[toIdx(at)]; }

   // Returns all locations of a given piece.
   // Caution - Not meant to be used in performance critical code.
   std::vector<Square> locations(Piece piece) const;

   // Adds given piece at given square. Does not validate correctness of position.
   void add(std::string_view placement) { add(Placement{placement}); }
   void add(const Placement& placement);

   // Removes given piece from given square. Does not validate correctness of position.
   void remove(std::string_view placement) { remove(Placement{placement}); }
   void remove(const Placement& placement);

   // Moves a given piece to another square. Does not validate correctness of position or
   // move.
   void move(const Relocation& relocation);

   bool operator==(const Position& other) const;
   bool operator!=(const Position& other) const;

   PlacementIterator begin(Color side) const;
   PlacementIterator end(Color side) const;

   double score() const { return 100.; }

   std::optional<File> enPassantFile() const { return m_enPassantFile; }
   void setEnPassantFile(std::optional<File> file) { m_enPassantFile = file; }

   bool hasKingMoved(Color side) const;
   bool hasRookMoved(Color side, bool onKingside) const;

   bool canAttack(Square sq, Color side) const;
   bool canAttack(Square sq, const Placement& placement) const;

 private:
   // Array indices for piece locations of each color.
   static constexpr std::size_t White = 0;
   static constexpr std::size_t Black = 1;

   using Count = unsigned char;

   // Placements for pieces of a type, e.g. rooks, pawns.
   template <std::size_t N> class PiecePlacements
   {
    public:
      void add(Square at);
      void remove(Square at);
      void move(Square from, Square to);
      std::vector<Square> locations() const;
      std::size_t count() const { return m_numPieces; }
      Square operator[](std::size_t idx) const { return m_locations[idx]; }

      bool operator==(const PiecePlacements& other) const;
      bool operator!=(const PiecePlacements& other) const;

    private:
      std::array<Square, N> m_locations = {static_cast<Square>(0)};
      Count m_numPieces = 0;
   };

   // Placements of all pieces of one color.
   class ColorPlacements
   {
    public:
      void add(const Placement& placement);
      void remove(const Placement& placement);
      void move(const Placement& from, Square to);
      std::vector<Square> locations(Piece piece) const;
      std::size_t count() const;
      Square placement(std::size_t idx) const;
      bool hasKingMoved() const { return m_hasKingMoved; }
      bool hasRookMoved(bool onKingside) const;

      bool operator==(const ColorPlacements& other) const;
      bool operator!=(const ColorPlacements& other) const;

    private:
      void initKingMovedFlag(Color side, Square at);
      void initRookMovedFlag(Color side);
      void updateRookMovedFlag(Square from);

    private:
      // Flags needed to check if castling is allowed.
      bool m_hasKingMoved = false;
      bool m_hasKingsideRookMoved = false;
      bool m_hasQueensideRookMoved = false;
      // Placements of pieces.
      std::optional<Square> m_king;
      PiecePlacements<8> m_pawns;
      PiecePlacements<9> m_queens;
      PiecePlacements<10> m_rooks;
      PiecePlacements<10> m_bishops;
      PiecePlacements<10> m_knights;
   };

 private:
   void populate(std::string_view placements);

   Square piece(Color side, std::size_t idx) const;

   // Returns placements of pieces with the same color as a given piece.
   ColorPlacements& pieces(Color side);
   const ColorPlacements& pieces(Color side) const;

   static std::size_t toIdx(Square at) { return static_cast<std::size_t>(at); }
   static std::size_t toColorIdx(Piece piece) { return isWhite(piece) ? White : Black; }
   static std::size_t toColorIdx(Color side);

 private:
   // File on which a pawn moved two squares forward from its starting location during the
   // last turn.
   std::optional<File> m_enPassantFile;
   // Board indexed by squares with information what piece is located there.
   std::array<std::optional<Piece>, 64> m_board;
   // Locations of each piece separated by color.
   std::array<ColorPlacements, 2> m_pieces;
};


///////////////////
// Implementation of Position.

inline std::vector<Square> Position::locations(Piece piece) const
{
   return pieces(color(piece)).locations(piece);
}

inline bool Position::hasKingMoved(Color side) const
{
   return m_pieces[Position::toColorIdx(side)].hasKingMoved();
}

inline bool Position::hasRookMoved(Color side, bool onKingside) const
{
   return m_pieces[Position::toColorIdx(side)].hasRookMoved(onKingside);
}

inline Square Position::piece(Color side, std::size_t idx) const
{
   return m_pieces[Position::toColorIdx(side)].placement(idx);
}

inline Position::ColorPlacements& Position::pieces(Color side)
{
   return m_pieces[toColorIdx(side)];
}

inline const Position::ColorPlacements& Position::pieces(Color side) const
{
   return m_pieces[toColorIdx(side)];
}

inline std::size_t Position::toColorIdx(Color side)
{
   return side == Color::White ? White : Black;
}

inline bool Position::operator==(const Position& other) const
{
   return m_board == other.m_board && m_pieces == other.m_pieces;
}

inline bool Position::operator!=(const Position& other) const
{
   return !(*this == other);
}


///////////////////
// Implementation of Position::ColorPlacements.

inline std::size_t Position::ColorPlacements::count() const
{
   return m_rooks.count() + m_bishops.count() + m_knights.count() + m_queens.count() +
          m_pawns.count() + (m_king.has_value() ? 1 : 0);
}


inline bool Position::ColorPlacements::hasRookMoved(bool onKingside) const
{
   return onKingside ? m_hasKingsideRookMoved : m_hasQueensideRookMoved;
}


inline bool Position::ColorPlacements::operator==(const ColorPlacements& other) const
{
   return m_rooks == other.m_rooks && m_bishops == other.m_bishops &&
          m_knights == other.m_knights && m_queens == other.m_queens &&
          m_pawns == other.m_pawns && m_king == other.m_king;
}


inline bool Position::ColorPlacements::operator!=(const ColorPlacements& other) const
{
   return !(*this == other);
}


inline void Position::ColorPlacements::initKingMovedFlag(Color side, Square at)
{
   m_hasKingMoved = side == Color::White ? (at == e1) : (at == e8);
}


///////////////////
// Implementation of Position::PiecePlacements.

template <std::size_t N> void Position::PiecePlacements<N>::add(Square at)
{
   assert(m_numPieces < m_locations.max_size());
   m_locations[m_numPieces++] = at;
}


template <std::size_t N> void Position::PiecePlacements<N>::remove(Square at)
{
   for (std::size_t i = 0; i < m_numPieces; ++i)
   {
      if (m_locations[i] == at)
      {
         // Set to
         m_locations[i] = static_cast<Square>(0);
         --m_numPieces;
      }
   }
}


template <std::size_t N> void Position::PiecePlacements<N>::move(Square from, Square to)
{
   for (std::size_t i = 0; i < m_numPieces; ++i)
      if (m_locations[i] == from)
         m_locations[i] = to;
}


template <std::size_t N>
std::vector<Square> Position::PiecePlacements<N>::locations() const
{
   std::vector<Square> locs;
   locs.reserve(m_numPieces);

   auto first = std::begin(m_locations);
   std::copy(first, first + m_numPieces, std::back_inserter(locs));

   return locs;
}


template <std::size_t N>
bool Position::PiecePlacements<N>::operator==(const PiecePlacements& other) const
{
   if (m_numPieces != other.m_numPieces)
      return false;
   for (std::size_t i = 0; i < m_numPieces; ++i)
      if (m_locations[i] != other.m_locations[i])
         return false;
   return true;
}


template <std::size_t N>
bool Position::PiecePlacements<N>::operator!=(const PiecePlacements& other) const
{
   return !(*this == other);
}


///////////////////

// Iterator for placements of a position.
class PlacementIterator
{
   friend class Position;

 public:
   using iterator_category = std::bidirectional_iterator_tag;
   using value_type = Placement;
   using pointer = const Piece*;
   using reference = const Piece&;

 private:
   // Only the position class needs access to this ctor.
   PlacementIterator(const Position* pos, Color side, std::size_t idx);

 public:
   PlacementIterator() = default;
   ~PlacementIterator() = default;
   PlacementIterator(const PlacementIterator&) = default;
   PlacementIterator& operator=(const PlacementIterator&) = default;
   PlacementIterator(PlacementIterator&&) = default;
   PlacementIterator& operator=(PlacementIterator&&) = default;

   value_type operator*() const;
   PlacementIterator& operator++();
   PlacementIterator operator++(int);
   PlacementIterator& operator--();
   PlacementIterator operator--(int);

   Piece piece() const;
   Square at() const;


   friend bool operator==(const PlacementIterator& a, const PlacementIterator& b)
   {
      return a.m_pos == b.m_pos && a.m_side == b.m_side && a.m_idx == b.m_idx;
   }

   friend bool operator!=(const PlacementIterator& a, const PlacementIterator& b)
   {
      return !(a == b);
   }

   friend bool operator<(const PlacementIterator& a, const PlacementIterator& b)
   {
      assert(a.m_pos == b.m_pos && a.m_side == b.m_side);
      return a.m_idx < b.m_idx;
   }

   friend bool operator>(const PlacementIterator& a, const PlacementIterator& b)
   {
      assert(a.m_pos == b.m_pos && a.m_side == b.m_side);
      return a.m_idx > b.m_idx;
   }

   friend bool operator<=(const PlacementIterator& a, const PlacementIterator& b)
   {
      return !(a > b);
   }

   friend bool operator>=(const PlacementIterator& a, const PlacementIterator& b)
   {
      return !(a < b);
   }

   friend void swap(PlacementIterator& a, PlacementIterator& b)
   {
      std::swap(a.m_pos, b.m_pos);
      std::swap(a.m_side, b.m_side);
      std::swap(a.m_idx, b.m_idx);
   }

 public:
   const Position* m_pos = nullptr;
   Color m_side = Color::White;
   // Index of piece.
   std::size_t m_idx = 0;
};


inline PlacementIterator::PlacementIterator(const Position* pos, Color side,
                                            std::size_t idx)
: m_pos{pos}, m_side{side}, m_idx{idx}
{
   assert(m_pos);
}

inline PlacementIterator::value_type PlacementIterator::operator*() const
{
   return Placement{piece(), at()};
}

inline PlacementIterator& PlacementIterator::operator++()
{
   ++m_idx;
   return *this;
}

inline PlacementIterator PlacementIterator::operator++(int)
{
   auto before = *this;
   ++(*this);
   return before;
}

inline PlacementIterator& PlacementIterator::operator--()
{
   --m_idx;
   return *this;
}

inline PlacementIterator PlacementIterator::operator--(int)
{
   auto before = *this;
   --(*this);
   return before;
}

inline Piece PlacementIterator::piece() const
{
   return (*m_pos)[at()].value();
}

inline Square PlacementIterator::at() const
{
   return m_pos->piece(m_side, m_idx);
}


///////////////////
// More implementation of Position because class PlacementIterator needs to be
// fully defined.

inline PlacementIterator Position::begin(Color side) const
{
   return PlacementIterator{this, side, 0};
}

inline PlacementIterator Position::end(Color side) const
{
   return PlacementIterator{this, side, m_pieces[toColorIdx(side)].count()};
}


///////////////////

extern const Position StartPos;

} // namespace matt2
