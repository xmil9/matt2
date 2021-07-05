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

class BaseMove
{
 protected:
   BaseMove() = default;

   // State of position before move is made. Needed to reverse moves.
   std::optional<File> m_prevEnPassantFile;
};


///////////////////

// clang-format off
struct EnabesEnPassant_t {};
constexpr EnabesEnPassant_t EnabesEnPassant;
// clang-format on

// Executes a "normal" chess move where a piece is relocated and optionally
// takes a piece on the target square.
// Does not validate that the move is legal.
class BasicMove : public BaseMove
{
 public:
   explicit BasicMove(const Relocation& moved, std::optional<Piece> taken = std::nullopt);
   BasicMove(const Relocation& moved, EnabesEnPassant_t);

   void move(Position& pos);
   void reverse(Position& pos);
   std::optional<File> enPassantFile() const { return m_enPassantFile; }

 private:
   // File that this move enables en-passant on.
   std::optional<File> m_enPassantFile;
   std::optional<Piece> m_taken;
   Relocation m_moved;
};


inline BasicMove::BasicMove(const Relocation& moved, std::optional<Piece> taken)
: m_taken{taken}, m_moved{moved}
{
}

inline BasicMove::BasicMove(const Relocation& moved, EnabesEnPassant_t)
: m_enPassantFile{file(moved.from())}, m_moved{moved}
{
}

inline void BasicMove::move(Position& pos)
{
   if (m_taken)
      pos.remove(Placement{*m_taken, m_moved.to()});
   pos.move(m_moved);

   m_prevEnPassantFile = pos.enPassantFile();
   pos.setEnPassantFile(m_enPassantFile);
}

inline void BasicMove::reverse(Position& pos)
{
   pos.move(m_moved.reverse());
   if (m_taken)
      pos.add(Placement{*m_taken, m_moved.to()});

   pos.setEnPassantFile(m_prevEnPassantFile);
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
class Castling : public BaseMove
{
 public:
   Castling(Kingside_t, Color color);
   Castling(Queenside_t, Color color);

   void move(Position& pos);
   void reverse(Position& pos);
   bool enablesEnPassant() const { return false; }

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

inline void Castling::move(Position& pos)
{
   pos.move(m_king);
   pos.move(m_rook);

   m_prevEnPassantFile = pos.enPassantFile();
   pos.setEnPassantFile(m_prevEnPassantFile);
}

inline void Castling::reverse(Position& pos)
{
   pos.move(m_rook.reverse());
   pos.move(m_king.reverse());

   pos.setEnPassantFile(m_prevEnPassantFile);
}


///////////////////

// Executes an en-passant move.
// Does not validate that the move is legal.
class EnPassant : public BaseMove
{
 public:
   EnPassant(const Relocation& pawn);

   void move(Position& pos);
   void reverse(Position& pos);
   bool enablesEnPassant() const { return false; }

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

   m_prevEnPassantFile = pos.enPassantFile();
   pos.setEnPassantFile(m_prevEnPassantFile);
}

inline void EnPassant::reverse(Position& pos)
{
   pos.move(m_movedPawn.reverse());
   pos.add(m_takenPawn);

   pos.setEnPassantFile(m_prevEnPassantFile);
}


///////////////////

// Executes a move that promotes a pawn.
// Does not validate that the move is legal.
class Promotion : public BaseMove
{
 public:
   Promotion(const Relocation& pawn, Piece promotedTo,
             std::optional<Piece> taken = std::nullopt);

   void move(Position& pos);
   void reverse(Position& pos);
   bool enablesEnPassant() const { return false; }

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

   m_prevEnPassantFile = pos.enPassantFile();
   pos.setEnPassantFile(m_prevEnPassantFile);
}

inline void Promotion::reverse(Position& pos)
{
   pos.remove(m_promoted);
   if (m_taken)
      pos.add(Placement{*m_taken, m_promoted.at()});
   pos.add(m_movedPawn);

   pos.setEnPassantFile(m_prevEnPassantFile);
}


///////////////////

// Any of the possible move types.
using Move = std::variant<BasicMove, Castling, EnPassant, Promotion>;


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
