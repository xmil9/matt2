//
// Jun-2021, Michael Lindner
// MIT license
//
#pragma once
#include "piece.h"
#include "position.h"
#include "relocation.h"
#include <optional>
#include <string>
#include <variant>


namespace matt2
{
///////////////////

class BasicMove
{
 public:
   explicit BasicMove(const Relocation& moved);
   BasicMove(const Relocation& moved, Piece taken);

   void move(Position& pos) const;
   void reverse(Position& pos) const;

 private:
   Relocation m_moved;
   std::optional<Piece> m_taken;
};


inline BasicMove::BasicMove(const Relocation& moved) : m_moved{moved}
{
}

inline BasicMove::BasicMove(const Relocation& moved, Piece taken)
: m_moved{moved}, m_taken{taken}
{
}

inline void BasicMove::move(Position& pos) const
{
   if (m_taken)
      pos.remove(Placement{*m_taken, m_moved.to()});
   pos.move(m_moved);
}

inline void BasicMove::reverse(Position& pos) const
{
   pos.move(m_moved.reverse());
   if (m_taken)
      pos.add(Placement{*m_taken, m_moved.to()});
}


///////////////////

// clang-format off
struct Kingside_t {};
constexpr Kingside_t Kingside;

struct Queenside_t {};
constexpr Queenside_t Queenside;
// clang-format on


class Castling
{
 public:
   Castling(Kingside_t, Color color);
   Castling(Queenside_t, Color color);

   void move(Position& pos) const;
   void reverse(Position& pos) const;

 private:
   Relocation m_king;
   Relocation m_rook;
};


inline Castling::Castling(Kingside_t, Color color)
: m_king{color == Color::White ? Relocation{Kw, e1, g1} : Relocation{Kb, e8, g8}},
  m_rook{color == Color::White ? Relocation{Rw, h1, f1} : Relocation{Rb, h8, f8}}
{
}

inline Castling::Castling(Queenside_t, Color color)
: m_king{color == Color::White ? Relocation{Kw, e1, c1} : Relocation{Kb, e8, c8}},
  m_rook{color == Color::White ? Relocation{Rw, a1, d1} : Relocation{Rb, a8, d8}}
{
}

inline void Castling::move(Position& pos) const
{
   pos.move(m_king);
   pos.move(m_rook);
}

inline void Castling::reverse(Position& pos) const
{
   pos.move(m_rook.reverse());
   pos.move(m_king.reverse());
}


///////////////////

class EnPassant
{
 public:
   EnPassant(const Relocation& pawn);

   void move(Position& pos) const;
   void reverse(Position& pos) const;

 private:
   Relocation m_movedPawn;
   Placement m_takenPawn;
};


inline EnPassant::EnPassant(const Relocation& pawn)
: m_movedPawn{pawn}, m_takenPawn{isWhite(pawn.piece()) ? Pb : Pw,
                                 makeSquare(file(pawn.to()), rank(pawn.from()))}
{
}

inline void EnPassant::move(Position& pos) const
{
   pos.move(m_movedPawn);
   pos.remove(m_takenPawn);
}

inline void EnPassant::reverse(Position& pos) const
{
   pos.move(m_movedPawn.reverse());
   pos.add(m_takenPawn);
}


///////////////////

class Promotion
{
 public:
   Promotion(const Relocation& pawn, Piece promotedTo);
   Promotion(const Relocation& pawn, Piece promotedTo, Piece taken);

   void move(Position& pos) const;
   void reverse(Position& pos) const;

 private:
   Placement m_movedPawn;
   Placement m_promoted;
   std::optional<Piece> m_taken;
};


inline Promotion::Promotion(const Relocation& pawn, Piece promotedTo)
: m_movedPawn{pawn.placement()}, m_promoted{promotedTo, pawn.to()}
{
}

inline Promotion::Promotion(const Relocation& pawn, Piece promotedTo, Piece taken)
: m_movedPawn{pawn.placement()}, m_promoted{promotedTo, pawn.to()}, m_taken{taken}
{
}

inline void Promotion::move(Position& pos) const
{
   if (m_taken)
      pos.remove(Placement{*m_taken, m_promoted.at()});
   pos.remove(m_movedPawn);
   pos.add(m_promoted);
}

inline void Promotion::reverse(Position& pos) const
{
   pos.remove(m_promoted);
   if (m_taken)
      pos.add(Placement{*m_taken, m_promoted.at()});
   pos.add(m_movedPawn);
}


///////////////////

using Move = std::variant<BasicMove, Castling, EnPassant, Promotion>;


inline Position& makeMove(Position& pos, const Move& move)
{
   auto dispatch = [&pos](const auto& specificMove) { specificMove.move(pos); };
   std::visit(dispatch, move);
   return pos;
}

inline Position& reverseMove(Position& pos, const Move& move)
{
   auto dispatch = [&pos](const auto& specificMove) { specificMove.reverse(pos); };
   std::visit(dispatch, move);
   return pos;
}

} // namespace matt2
