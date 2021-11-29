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
   // Standard Algebraic Notation (FIDE)
   SAN,
   // Long Algebraic Notation
   // Includes starting locations of pieces.
   LAN,
   // Includes full details, e.g. piece color and starting location.
   Detailed
};


///////////////////

// Standard Algebraic Notation (FIDE)
class San
{
 public:
   NotationScheme scheme() const { return NotationScheme::SAN; }
   std::string& notate(std::string& out, Piece p) const;
   std::string& notate(std::string& out, const BasicMove& move) const;
   std::string& notate(std::string& out, const Castling& move) const;
   std::string& notate(std::string& out, const EnPassant& move) const;
   std::string& notate(std::string& out, const Promotion& move) const;

 private:
   static constexpr bool WithPieceColor = false;
   static constexpr bool WithPawnStart = false;
};


inline std::string& San::notate(std::string& out, Piece p) const
{
   out += toString(p, false);
   return out;
}

inline std::string& San::notate(std::string& out, const Castling& move) const
{
   out += move.toString(WithPieceColor);
   return out;
}

inline std::string& San::notate(std::string& out, const EnPassant& move) const
{
   out += move.toString(WithPieceColor);
   return out;
}

inline std::string& San::notate(std::string& out, const Promotion& move) const
{
   out += move.toString(WithPieceColor, WithPawnStart);
   return out;
}


///////////////////

// Long Algebraic Notation
class Lan
{
 public:
   NotationScheme scheme() const { return NotationScheme::LAN; }
   std::string& notate(std::string& out, Piece p) const;
   std::string& notate(std::string& out, const BasicMove& move) const;
   std::string& notate(std::string& out, const Castling& move) const;
   std::string& notate(std::string& out, const EnPassant& move) const;
   std::string& notate(std::string& out, const Promotion& move) const;

 private:
   static constexpr bool WithPieceColor = false;
   static constexpr bool WithPawnStart = true;
};


inline std::string& Lan::notate(std::string& out, Piece p) const
{
   out += matt2::toString(p, false);
   return out;
}

inline std::string& Lan::notate(std::string& out, const Castling& move) const
{
   out += move.toString(WithPieceColor);
   return out;
}

inline std::string& Lan::notate(std::string& out, const EnPassant& move) const
{
   out += move.toString(WithPieceColor);
   return out;
}

inline std::string& Lan::notate(std::string& out, const Promotion& move) const
{
   out += move.toString(WithPieceColor, WithPawnStart);
   return out;
}


///////////////////

class DetailedNotation
{
 public:
   NotationScheme scheme() const { return NotationScheme::Detailed; }
   std::string& notate(std::string& out, Piece p) const;
   std::string& notate(std::string& out, const BasicMove& move) const;
   std::string& notate(std::string& out, const Castling& move) const;
   std::string& notate(std::string& out, const EnPassant& move) const;
   std::string& notate(std::string& out, const Promotion& move) const;

 private:
   static constexpr bool WithPieceColor = true;
   static constexpr bool WithPawnStart = true;
};


inline std::string& DetailedNotation::notate(std::string& out, Piece p) const
{
   out += matt2::toString(p, true);
   return out;
}

inline std::string& DetailedNotation::notate(std::string& out, const Castling& move) const
{
   out += move.toString(WithPieceColor);
   return out;
}


///////////////////

using Notation = std::variant<San, Lan, DetailedNotation>;


inline NotationScheme scheme(const Notation& n)
{
   auto dispatch = [](const auto& specificNotation) { return specificNotation.scheme(); };
   return std::visit(dispatch, n);
}

inline std::string& notate(std::string& out, Piece p, const Notation& n)
{
   auto dispatch = [&](const auto& specificNotation)
   { return specificNotation.notate(out, p); };
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
