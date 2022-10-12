//
// Nov-2021, Michael Lindner
// MIT license
//
#pragma once
#include "move.h"
#include "piece.h"
#include "square.h"
#include <string>
#include <variant>

// clang-format off
namespace matt2 { class Placement; }
namespace matt2 { class Position; }
// clang-format on

namespace matt2
{
///////////////////

enum class NotationScheme
{
   // Long Algebraic Notation
   // Includes starting locations of pieces.
   LAN,
   // Includes full details, e.g. piece color and starting location.
   Detailed
};


///////////////////

// Long Algebraic Notation
class Lan
{
 public:
   NotationScheme scheme() const { return NotationScheme::LAN; }
   std::string& notate(std::string& out, Color c) const;
   std::string& notate(std::string& out, Piece p) const;
   std::string& notate(std::string& out, File f) const;
   std::string& notate(std::string& out, Rank r) const;
   std::string& notate(std::string& out, Square sq) const;
   std::string& notate(std::string& out, const Placement& placement) const;
   std::string& notate(std::string& out, const BasicMove& move) const;
   std::string& notate(std::string& out, const Castling& move) const;
   std::string& notate(std::string& out, const EnPassant& move) const;
   std::string& notate(std::string& out, const Promotion& move) const;
   std::string& notate(std::string& out, const Move& move) const;
   std::string& notate(std::string& out, const Position& pos) const;

 private:
   static constexpr bool WithoutPieceColor = false;
   static constexpr bool WithStartingLocation = true;
};


///////////////////

// Internal, non-standard notation that contains more information about a move, e.g.
// which piece was taken.
class DetailedNotation
{
 public:
   NotationScheme scheme() const { return NotationScheme::Detailed; }
   std::string& notate(std::string& out, Color c) const;
   std::string& notate(std::string& out, Piece p) const;
   std::string& notate(std::string& out, File f) const;
   std::string& notate(std::string& out, Rank r) const;
   std::string& notate(std::string& out, Square sq) const;
   std::string& notate(std::string& out, const Placement& placement) const;
   std::string& notate(std::string& out, const BasicMove& move) const;
   std::string& notate(std::string& out, const Castling& move) const;
   std::string& notate(std::string& out, const EnPassant& move) const;
   std::string& notate(std::string& out, const Promotion& move) const;
   std::string& notate(std::string& out, const Move& move) const;
   std::string& notate(std::string& out, const Position& pos) const;

 private:
   static constexpr bool WithPieceColor = true;
   static constexpr bool WithStartingLocation = true;
};


///////////////////

using Notation = std::variant<Lan, DetailedNotation>;


// Notation api as free functions taking a notation instance.

inline NotationScheme scheme(const Notation& n)
{
   auto dispatch = [](const auto& specificNotation) { return specificNotation.scheme(); };
   return std::visit(dispatch, n);
}

template<typename T>
inline std::string& notate(std::string& out, const T& item, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, item); };
   std::visit(dispatch, n);
   return out;
}

///////////////////

std::string& printPosition(std::string& out, const Position& pos);

} // namespace matt2
