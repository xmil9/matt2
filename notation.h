//
// Nov-2021, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include "move.h"
#include <string>
#include <variant>


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

 private:
   static constexpr bool WithPieceColor = true;
   static constexpr bool WithStartingLocation = true;
};


///////////////////

using Notation = std::variant<Lan, DetailedNotation>;


inline NotationScheme scheme(const Notation& n)
{
   auto dispatch = [](const auto& specificNotation) { return specificNotation.scheme(); };
   return std::visit(dispatch, n);
}

inline std::string& notate(std::string& out, Color c, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, c); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, Piece p, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, p); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, File f, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, f); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, Rank r, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, r); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, Square sq, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, sq); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, const Placement& placement, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, placement); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, const BasicMove& move, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, move); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, const Castling& move, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, move); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, const EnPassant& move, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, move); };
   std::visit(dispatch, n);
   return out;
}

inline std::string& notate(std::string& out, const Promotion& move, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, move); };
   std::visit(dispatch, n);
   return out;
}

} // namespace matt2
