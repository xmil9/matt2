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
#include <vector>


namespace matt2
{
///////////////////

// Represents a position of pieces on the chess board.
class Position
{
 public:
   Position() = default;
   explicit Position(std::string_view placements);

   // Returns piece at given location on board.
   std::optional<Piece> operator[](Square at) const { return m_board[toIdx(at)]; }

   // Returns all locations of a given piece.
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

 private:
   // Array indices for piece locations of each color.
   static constexpr std::size_t White = 0;
   static constexpr std::size_t Black = 1;

   using Count = unsigned char;

   // Placements for pieces of a type, e.g. rooks, pawns.
   template <std::size_t N> class TypePlacements
   {
    public:
      void add(Square at);
      void remove(Square at);
      void move(Square from, Square to);
      std::vector<Square> locations() const;

      bool operator==(const TypePlacements& other) const;
      bool operator!=(const TypePlacements& other) const;

    private:
      std::array<Square, N> m_locations = {static_cast<Square>(0)};
      Count m_numPieces = 0;
   };

   // Placements of all pieces of one color.
   class PiecePlacements
   {
    public:
      void add(const Placement& placement);
      void remove(const Placement& placement);
      void move(const Placement& from, Square to);
      std::vector<Square> locations(Piece piece) const;

      bool operator==(const PiecePlacements& other) const;
      bool operator!=(const PiecePlacements& other) const;

    private:
      // todo - handle up to 9 queens and 10 rooks, bishops, knights because of promotions
      TypePlacements<8> m_pawns;
      TypePlacements<2> m_rooks;
      TypePlacements<2> m_bishops;
      TypePlacements<2> m_knights;
      std::optional<Square> m_king;
      std::optional<Square> m_queen;
   };

 private:
   void populate(std::string_view placements);

   // Returns placements of pieces with the same color as a given piece.
   PiecePlacements& colorPieces(Piece piece);
   const PiecePlacements& colorPieces(Piece piece) const;

   static std::size_t toIdx(Square at) { return static_cast<std::size_t>(at); }
   static std::size_t toColorIdx(Piece piece) { return isWhite(piece) ? White : Black; }

 private:
   // Board indexed by squares with information what piece is located there.
   std::array<std::optional<Piece>, 64> m_board;
   // Locations of each piece separated by color.
   std::array<PiecePlacements, 2> m_pieces;
};


///////////////////
// Implementation of Position.

inline std::vector<Square> Position::locations(Piece piece) const
{
   return colorPieces(piece).locations(piece);
}


inline Position::PiecePlacements& Position::colorPieces(Piece piece)
{
   return m_pieces[toColorIdx(piece)];
}


inline const Position::PiecePlacements& Position::colorPieces(Piece piece) const
{
   return m_pieces[toColorIdx(piece)];
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
// Implementation of Position::PiecePlacements.

inline bool Position::PiecePlacements::operator==(const PiecePlacements& other) const
{
   return m_pawns == other.m_pawns && m_rooks == other.m_rooks &&
          m_bishops == other.m_bishops && m_knights == other.m_knights &&
          m_king == other.m_king && m_queen == other.m_queen;
}


inline bool Position::PiecePlacements::operator!=(const PiecePlacements& other) const
{
   return !(*this == other);
}


///////////////////
// Implementation of Position::TypePlacements.

template <std::size_t N> void Position::TypePlacements<N>::add(Square at)
{
   assert(m_numPieces < m_locations.max_size());
   m_locations[m_numPieces++] = at;
}


template <std::size_t N> void Position::TypePlacements<N>::remove(Square at)
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


template <std::size_t N> void Position::TypePlacements<N>::move(Square from, Square to)
{
   for (std::size_t i = 0; i < m_numPieces; ++i)
      if (m_locations[i] == from)
         m_locations[i] = to;
}


template <std::size_t N>
std::vector<Square> Position::TypePlacements<N>::locations() const
{
   std::vector<Square> locs;
   locs.reserve(m_numPieces);

   auto first = std::begin(m_locations);
   std::copy(first, first + m_numPieces, std::back_inserter(locs));

   return locs;
}


template <std::size_t N>
bool Position::TypePlacements<N>::operator==(const TypePlacements& other) const
{
   if (m_numPieces != other.m_numPieces)
      return false;
   for (std::size_t i = 0; i < m_numPieces; ++i)
      if (m_locations[i] != other.m_locations[i])
         return false;
   return true;
}


template <std::size_t N>
bool Position::TypePlacements<N>::operator!=(const TypePlacements& other) const
{
   return !(*this == other);
}


///////////////////

extern const Position StartPos;

} // namespace matt2
