//
// Nov-2021, Michael Lindner
// MIT license
//
#include "notation.h"

using namespace matt2;

namespace
{
///////////////////

// Algebraic notation

std::string& notateColorAn(std::string& out, Color c)
{
   return out += c == Color::White ? "w" : "b";
}

std::string& notatePieceAn(std::string& out, Piece p, bool withColor = false)
{
   switch (p)
   {
   case Kw:
   case Kb:
      out += "K";
      break;
   case Qw:
   case Qb:
      out += "Q";
      break;
   case Rw:
   case Rb:
      out += "R";
      break;
   case Bw:
   case Bb:
      out += "B";
      break;
   case Nw:
   case Nb:
      out += "N";
      break;
   case Pw:
   case Pb:
      break;
   default:
      throw std::runtime_error("Invalid piece.");
   }

   if (withColor)
      notateColorAn(out, color(p));

   return out;
}

std::string& notateFileAn(std::string& out, File f)
{
   return out += toLowercaseChar(f);
}

std::string& notateRankAn(std::string& out, Rank r)
{
   return out += toLowercaseChar(r);
}

std::string& notateSquareAn(std::string& out, Square sq)
{
   notateFileAn(out, file(sq));
   return notateRankAn(out, rank(sq));
}

std::string& notatePlacementAn(std::string& out, const Placement& placement,
                               bool withColor = false)
{
   notatePieceAn(out, placement.piece(), withColor);
   return notateSquareAn(out, placement.at());
}

std::string& notateMoveInAlgebraicNotation(std::string& out, const matt2::BasicMove& move,
                                           bool withColor, bool withStart)
{
   notatePieceAn(out, move.piece(), withColor);
   if (withStart)
      notateSquareAn(out, move.from());
   if (auto taken = move.taken(); taken.has_value())
      out += 'x';
   notateSquareAn(out, move.to());
   return out;
}

std::string& notateCastlingInAlgebraicNotation(std::string& out,
                                               const matt2::Castling& move,
                                               bool withColor)
{
   if (withColor)
      notateColorAn(out, color(move.king()));

   const bool isKingside = file(move.kingTo()) == fg;
   out += isKingside ? "0-0" : "0-0-0";
   return out;
}

std::string& notateEnPassantInAlgebraicNotation(std::string& out,
                                                const matt2::EnPassant& move,
                                                bool withColor)
{
   if (withColor)
      notateColorAn(out, color(move.pawn()));

   // Moved pawn's starting file.
   notateFileAn(out, file(move.from()));
   // Capturing indicator.
   out += "x";
   // Moved pawn's destination square.
   notateSquareAn(out, move.to());

   return out;
}

std::string& notatePromotionInAlgebraicNotation(std::string& out,
                                                const matt2::Promotion& move,
                                                bool withColor, bool withStart)
{
   notatePieceAn(out, move.pawn(), withColor);
   if (withStart)
      notateSquareAn(out, move.from());

   if (move.taken().has_value())
      out += "x";

   notateSquareAn(out, move.to());
   // Piece that pawn was promoted to.
   out += '=';
   notatePieceAn(out, move.promotedTo(), withColor);

   return out;
}

///////////////////

// Detailed notation

std::string& notateTakenPieceDn(std::string& out, Piece taken, bool withColor)
{
   out += "[x:";
   notatePieceAn(out, taken, withColor);
   out += "]";
   return out;
}

} // namespace


namespace matt2
{
///////////////////

std::string& Lan::notate(std::string& out, Color c) const
{
   return notateColorAn(out, c);
}

std::string& Lan::notate(std::string& out, Piece p) const
{
   return notatePieceAn(out, p, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, File f) const
{
   return notateFileAn(out, f);
}

std::string& Lan::notate(std::string& out, Rank r) const
{
   return notateRankAn(out, r);
}

std::string& Lan::notate(std::string& out, Square sq) const
{
   return notateSquareAn(out, sq);
}

std::string& Lan::notate(std::string& out, const Placement& placement) const
{
   return notatePlacementAn(out, placement, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const BasicMove& move) const
{
   return notateMoveInAlgebraicNotation(out, move, WithoutPieceColor,
                                        WithStartingLocation);
}

std::string& Lan::notate(std::string& out, const Castling& move) const
{
   return notateCastlingInAlgebraicNotation(out, move, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const EnPassant& move) const
{
   return notateEnPassantInAlgebraicNotation(out, move, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const Promotion& move) const
{
   return notatePromotionInAlgebraicNotation(out, move, WithoutPieceColor,
                                             WithStartingLocation);
}

///////////////////

std::string& DetailedNotation::notate(std::string& out, Color c) const
{
   return notateColorAn(out, c);
}

std::string& DetailedNotation::notate(std::string& out, Piece p) const
{
   return notatePieceAn(out, p, WithPieceColor);
}

std::string& DetailedNotation::notate(std::string& out, File f) const
{
   return notateFileAn(out, f);
}

std::string& DetailedNotation::notate(std::string& out, Rank r) const
{
   return notateRankAn(out, r);
}

std::string& DetailedNotation::notate(std::string& out, Square sq) const
{
   return notateSquareAn(out, sq);
}

std::string& DetailedNotation::notate(std::string& out, const Placement& placement) const
{
   return notatePlacementAn(out, placement, WithPieceColor);
}

std::string& DetailedNotation::notate(std::string& out, const BasicMove& move) const
{
   notateMoveInAlgebraicNotation(out, move, WithPieceColor, WithStartingLocation);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      notateTakenPieceDn(out, *taken, WithPieceColor);

   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Castling& move) const
{
   return notateCastlingInAlgebraicNotation(out, move, WithPieceColor);
}

std::string& DetailedNotation::notate(std::string& out, const EnPassant& move) const
{
   notateEnPassantInAlgebraicNotation(out, move, WithPieceColor);

   // Append info about taken piece.
   notateTakenPieceDn(out, move.taken(), WithPieceColor);

   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Promotion& move) const
{
   notatePromotionInAlgebraicNotation(out, move, WithPieceColor, WithStartingLocation);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      notateTakenPieceDn(out, *taken, WithPieceColor);

   return out;
}

} // namespace matt2
