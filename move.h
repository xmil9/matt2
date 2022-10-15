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

// Holds information about a position's state before a move. This info is used to restore
// the position to that state when a move is reversed.
class ReversibleState
{
 protected:
   ReversibleState() = default;

   void setState(std::optional<Square> enPassantSquare, Position& pos);
   void resetState(Position& pos);

   friend bool operator==(const ReversibleState& a, const ReversibleState& b)
   {
      return a.m_prevEnPassantSquare == b.m_prevEnPassantSquare;
   }

 private:
   // State of position before move is made. Needed to reverse moves.
   std::optional<Square> m_prevEnPassantSquare;
};


inline void ReversibleState::setState(std::optional<Square> enPassantSquare,
                                      Position& pos)
{
   // Remember the position's previous state, so that we can reset it.
   m_prevEnPassantSquare = pos.enPassantSquare();

   // Set the state caused by this move as the position's new state.
   pos.setEnPassantSquare(enPassantSquare);
}

inline void ReversibleState::resetState(Position& pos)
{
   pos.setEnPassantSquare(m_prevEnPassantSquare);
}

inline bool operator!=(const ReversibleState& a, const ReversibleState& b)
{
   return !(a == b);
}


///////////////////

// clang-format off
struct EnablesEnPassant_t {};
constexpr EnablesEnPassant_t EnablesEnPassant;
// clang-format on

// Executes a "normal" chess move where a piece is relocated and optionally
// takes a piece on the target square.
// Does not validate that the move is legal.
class BasicMove : public ReversibleState
{
 public:
   explicit BasicMove(const Relocation& moved, std::optional<Piece> taken = std::nullopt);
   // Ctor for moves that allow opponent to use en-passant rule as next move.
   BasicMove(const Relocation& moved, EnablesEnPassant_t);

   void move(Position& pos);
   void reverse(Position& pos);

   Piece piece() const { return m_moved.piece(); }
   Square from() const { return m_moved.from(); }
   Square to() const { return m_moved.to(); }
   std::optional<Piece> taken() const { return m_taken; }
   std::optional<Square> enPassantSquare() const { return m_enPassantSquare; }

   friend bool operator==(const BasicMove& a, const BasicMove& b)
   {
      return static_cast<ReversibleState>(a) == static_cast<ReversibleState>(b) &&
             a.m_moved == b.m_moved && a.m_taken == b.m_taken &&
             a.m_enPassantSquare == b.m_enPassantSquare;
   }

 private:
   Relocation m_moved;
   std::optional<Piece> m_taken;
   // Square that this move enables en-passant on.
   std::optional<Square> m_enPassantSquare;
};


inline BasicMove::BasicMove(const Relocation& moved, std::optional<Piece> taken)
: m_moved{moved}, m_taken{taken}
{
}

inline BasicMove::BasicMove(const Relocation& moved, EnablesEnPassant_t)
: m_moved{moved}, m_enPassantSquare{moved.to()}
{
}

inline void BasicMove::move(Position& pos)
{
   if (m_taken)
      pos.remove(Placement{*m_taken, m_moved.to()});
   pos.move(m_moved);

   setState(m_enPassantSquare, pos);
}

inline void BasicMove::reverse(Position& pos)
{
   pos.move(m_moved.reverse());
   if (m_taken)
      pos.add(Placement{*m_taken, m_moved.to()});

   resetState(pos);
}

inline bool operator!=(const BasicMove& a, const BasicMove& b)
{
   return !(a == b);
}


///////////////////

// clang-format off
struct Kingside_t {};
constexpr Kingside_t Kingside;

struct Queenside_t {};
constexpr Queenside_t Queenside;
// clang-format on


// Executes a castling move.
// Does not validate that the move is legal.
class Castling : public ReversibleState
{
 public:
   Castling(Kingside_t, Color side);
   Castling(Queenside_t, Color side);

   void move(Position& pos);
   void reverse(Position& pos);

   bool isKingside() const { return file(m_king.to()) == fg; }
   Piece king() const { return m_king.piece(); }
   Square kingFrom() const { return m_king.from(); }
   Square kingTo() const { return m_king.to(); }
   Piece rook() const { return m_rook.piece(); }
   Square rookFrom() const { return m_rook.from(); }
   Square rookTo() const { return m_rook.to(); }

   // Generic functions for all Move types.
   // For castling they refer to the king.
   Square from() const { return kingFrom(); }
   Square to() const { return kingTo(); }


   friend bool operator==(const Castling& a, const Castling& b)
   {
      return static_cast<ReversibleState>(a) == static_cast<ReversibleState>(b) &&
             a.m_king == b.m_king && a.m_rook == b.m_rook;
   }

 private:
   Relocation m_king;
   Relocation m_rook;
};


inline Castling::Castling(Kingside_t, Color side)
: m_king{side == Color::White ? Relocation{Kw, e1, g1} : Relocation{Kb, e8, g8}},
  m_rook{side == Color::White ? Relocation{Rw, h1, f1} : Relocation{Rb, h8, f8}}
{
}

inline Castling::Castling(Queenside_t, Color side)
: m_king{side == Color::White ? Relocation{Kw, e1, c1} : Relocation{Kb, e8, c8}},
  m_rook{side == Color::White ? Relocation{Rw, a1, d1} : Relocation{Rb, a8, d8}}
{
}

inline void Castling::move(Position& pos)
{
   pos.move(m_king);
   pos.move(m_rook);

   setState(std::nullopt, pos);
}

inline void Castling::reverse(Position& pos)
{
   pos.move(m_rook.reverse());
   pos.move(m_king.reverse());

   resetState(pos);
}

inline bool operator!=(const Castling& a, const Castling& b)
{
   return !(a == b);
}


///////////////////

// Executes an en-passant move.
// Does not validate that the move is legal.
class EnPassant : public ReversibleState
{
 public:
   EnPassant(const Relocation& pawn);

   void move(Position& pos);
   void reverse(Position& pos);

   Piece pawn() const { return m_movedPawn.piece(); }
   Square from() const { return m_movedPawn.from(); }
   Square to() const { return m_movedPawn.to(); }
   Piece taken() const { return m_takenPawn.piece(); }
   Square takenAt() const { return m_takenPawn.at(); }

   friend bool operator==(const EnPassant& a, const EnPassant& b)
   {
      return static_cast<ReversibleState>(a) == static_cast<ReversibleState>(b) &&
             a.m_movedPawn == b.m_movedPawn && a.m_takenPawn == b.m_takenPawn;
   }

 private:
   Relocation m_movedPawn;
   Placement m_takenPawn;
};


inline EnPassant::EnPassant(const Relocation& pawn)
: m_movedPawn{pawn}, m_takenPawn{isWhite(pawn.piece()) ? Pb : Pw,
                                 makeSquare(file(pawn.to()), rank(pawn.from()))}
{
}

inline void EnPassant::move(Position& pos)
{
   pos.move(m_movedPawn);
   pos.remove(m_takenPawn);

   setState(std::nullopt, pos);
}

inline void EnPassant::reverse(Position& pos)
{
   pos.move(m_movedPawn.reverse());
   pos.add(m_takenPawn);

   resetState(pos);
}

inline bool operator!=(const EnPassant& a, const EnPassant& b)
{
   return !(a == b);
}


///////////////////

// Executes a move that promotes a pawn.
// Does not validate that the move is legal.
class Promotion : public ReversibleState
{
 public:
   Promotion(const Relocation& pawn, Piece promotedTo,
             std::optional<Piece> taken = std::nullopt);

   void move(Position& pos);
   void reverse(Position& pos);

   Piece pawn() const { return m_movedPawn.piece(); }
   Square from() const { return m_movedPawn.at(); }
   Square to() const { return m_promoted.at(); }
   Piece promotedTo() const { return m_promoted.piece(); }
   std::optional<Piece> taken() const { return m_taken; }

   friend bool operator==(const Promotion& a, const Promotion& b)
   {
      return static_cast<ReversibleState>(a) == static_cast<ReversibleState>(b) &&
             a.m_movedPawn == b.m_movedPawn && a.m_promoted == b.m_promoted &&
             a.m_taken == b.m_taken;
   }

 private:
   Placement m_movedPawn;
   Placement m_promoted;
   std::optional<Piece> m_taken;
};


inline Promotion::Promotion(const Relocation& pawn, Piece promotedTo,
                            std::optional<Piece> taken)
: m_movedPawn{pawn.placement()}, m_promoted{promotedTo, pawn.to()}, m_taken{taken}
{
}

inline void Promotion::move(Position& pos)
{
   if (m_taken)
      pos.remove(Placement{*m_taken, m_promoted.at()});
   pos.remove(m_movedPawn);
   pos.add(m_promoted);

   setState(std::nullopt, pos);
}

inline void Promotion::reverse(Position& pos)
{
   pos.remove(m_promoted);
   if (m_taken)
      pos.add(Placement{*m_taken, m_promoted.at()});
   pos.add(m_movedPawn);

   resetState(pos);
}

inline bool operator!=(const Promotion& a, const Promotion& b)
{
   return !(a == b);
}


///////////////////

// Any of the possible move types.
using Move = std::variant<BasicMove, Castling, EnPassant, Promotion>;


inline Square to(const Move& move)
{
   auto dispatch = [](const auto& specificMove) { return specificMove.to(); };
   return std::visit(dispatch, move);
}

inline Square from(const Move& move)
{
   auto dispatch = [](const auto& specificMove) { return specificMove.from(); };
   return std::visit(dispatch, move);
}

inline Position& makeMove(Position& pos, Move& move)
{
   auto dispatch = [&pos](auto& specificMove) { specificMove.move(pos); };
   std::visit(dispatch, move);
   return pos;
}

inline Position& reverseMove(Position& pos, Move& move)
{
   auto dispatch = [&pos](auto& specificMove) { specificMove.reverse(pos); };
   std::visit(dispatch, move);
   return pos;
}

} // namespace matt2
