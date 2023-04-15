//
// Nov-2021, Michael Lindner
// MIT license
//
#include "notation.h"
#include "move.h"
#include <algorithm>

using namespace matt2;

namespace an
{
///////////////////

// Algebraic notation

std::string& notateColor(std::string& out, Color c)
{
   return out += c == White ? "w" : "b";
}

std::string& notatePiece(std::string& out, Piece p, bool withColor = false)
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
      notateColor(out, color(p));

   return out;
}

std::string& notateFile(std::string& out, File f)
{
   return out += toChar(f);
}

std::string& notateRank(std::string& out, Rank r)
{
   return out += toChar(r);
}

std::string& notateSquare(std::string& out, Square sq)
{
   notateFile(out, file(sq));
   return notateRank(out, rank(sq));
}

std::string& notatePlacement(std::string& out, const Placement& placement,
                             bool withColor = false)
{
   notatePiece(out, placement.piece(), withColor);
   return notateSquare(out, placement.at());
}

std::string& notateMove(std::string& out, const matt2::BasicMove& move, bool withColor,
                        bool withStart)
{
   notatePiece(out, move.piece(), withColor);
   if (withStart)
      notateSquare(out, move.from());
   if (auto taken = move.taken(); taken.has_value())
      out += 'x';
   notateSquare(out, move.to());
   return out;
}

std::string& notateCastling(std::string& out, const matt2::Castling& move, bool withColor)
{
   if (withColor)
      notateColor(out, color(move.king()));

   const bool isKingside = file(move.kingTo()) == fg;
   out += isKingside ? "0-0" : "0-0-0";
   return out;
}

std::string& notateEnPassant(std::string& out, const matt2::EnPassant& move,
                             bool withColor)
{
   if (withColor)
      notateColor(out, color(move.pawn()));

   // Moved pawn's starting file.
   notateFile(out, file(move.from()));
   // Capturing indicator.
   out += "x";
   // Moved pawn's destination square.
   notateSquare(out, move.to());

   return out;
}

std::string& notatePromotion(std::string& out, const matt2::Promotion& move,
                             bool withColor, bool withStart)
{
   notatePiece(out, move.pawn(), withColor);
   if (withStart)
      notateSquare(out, move.from());

   if (move.taken().has_value())
      out += "x";

   notateSquare(out, move.to());
   // Piece that pawn was promoted to.
   out += '=';
   notatePiece(out, move.promotedTo(), withColor);

   return out;
}

} // namespace an

namespace dn
{
///////////////////

// Detailed notation

std::string& notateTakenPiece(std::string& out, Piece taken, bool withColor)
{
   out += "[x:";
   an::notatePiece(out, taken, withColor);
   out += "]";
   return out;
}

} // namespace dn

///////////////////

static void trimTrailingChar(std::string& s, char c)
{
   while (!s.empty() && *s.rbegin() == c)
      s.erase(s.begin() + s.length() - 1);
}

static std::string& notatePlacements(std::string& out, const Position& pos, Color color,
                                     char pieceSep)
{
   std::for_each(pos.begin(color), pos.end(color),
                 [&out, pieceSep](const auto& placement)
                 {
                    an::notatePlacement(out, placement, true);
                    out += pieceSep;
                 });
   return out;
}

static std::string& notatePosition(std::string& out, const Position& pos)
{
   static constexpr char PieceSep = ' ';

   notatePlacements(out, pos, White, PieceSep);
   notatePlacements(out, pos, Black, PieceSep);
   trimTrailingChar(out, PieceSep);
   return out;
}

namespace matt2
{
///////////////////

std::string& Lan::notate(std::string& out, Color c) const
{
   return an::notateColor(out, c);
}

std::string& Lan::notate(std::string& out, Piece p) const
{
   return an::notatePiece(out, p, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, File f) const
{
   return an::notateFile(out, f);
}

std::string& Lan::notate(std::string& out, Rank r) const
{
   return an::notateRank(out, r);
}

std::string& Lan::notate(std::string& out, Square sq) const
{
   return an::notateSquare(out, sq);
}

std::string& Lan::notate(std::string& out, const Placement& placement) const
{
   return an::notatePlacement(out, placement, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const BasicMove& move) const
{
   return an::notateMove(out, move, WithoutPieceColor, WithStartingLocation);
}

std::string& Lan::notate(std::string& out, const Castling& move) const
{
   return an::notateCastling(out, move, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const EnPassant& move) const
{
   return an::notateEnPassant(out, move, WithoutPieceColor);
}

std::string& Lan::notate(std::string& out, const Promotion& move) const
{
   return an::notatePromotion(out, move, WithoutPieceColor, WithStartingLocation);
}

std::string& Lan::notate(std::string& out, const Move& move) const
{
   auto dispatch = [this, &out](const auto& specificMove)
   { return notate(out, specificMove); };
   std::visit(dispatch, move);
   return out;
}

std::string& Lan::notate(std::string& out, const Position& pos) const
{
   return notatePosition(out, pos);
}

///////////////////

std::string& DetailedNotation::notate(std::string& out, Color c) const
{
   return an::notateColor(out, c);
}

std::string& DetailedNotation::notate(std::string& out, Piece p) const
{
   return an::notatePiece(out, p, WithPieceColor);
}

std::string& DetailedNotation::notate(std::string& out, File f) const
{
   return an::notateFile(out, f);
}

std::string& DetailedNotation::notate(std::string& out, Rank r) const
{
   return an::notateRank(out, r);
}

std::string& DetailedNotation::notate(std::string& out, Square sq) const
{
   return an::notateSquare(out, sq);
}

std::string& DetailedNotation::notate(std::string& out, const Placement& placement) const
{
   return an::notatePlacement(out, placement, WithPieceColor);
}

std::string& DetailedNotation::notate(std::string& out, const BasicMove& move) const
{
   an::notateMove(out, move, WithPieceColor, WithStartingLocation);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      dn::notateTakenPiece(out, *taken, WithPieceColor);

   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Castling& move) const
{
   return an::notateCastling(out, move, WithPieceColor);
}

std::string& DetailedNotation::notate(std::string& out, const EnPassant& move) const
{
   an::notateEnPassant(out, move, WithPieceColor);

   // Append info about taken piece.
   dn::notateTakenPiece(out, move.taken(), WithPieceColor);

   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Promotion& move) const
{
   an::notatePromotion(out, move, WithPieceColor, WithStartingLocation);

   // Append info about taken piece.
   if (auto taken = move.taken(); taken.has_value())
      dn::notateTakenPiece(out, *taken, WithPieceColor);

   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Move& move) const
{
   auto dispatch = [this, &out](const auto& specificMove)
   { return notate(out, specificMove); };
   std::visit(dispatch, move);
   return out;
}

std::string& DetailedNotation::notate(std::string& out, const Position& pos) const
{
   return notatePosition(out, pos);
}

///////////////////

static std::string& printPiece(std::string& out, Piece p)
{
   switch (p)
   {
   case Kw:
      out += "K";
      break;
   case Kb:
      out += "k";
      break;
   case Qw:
      out += "Q";
      break;
   case Qb:
      out += "q";
      break;
   case Rw:
      out += "R";
      break;
   case Rb:
      out += "r";
      break;
   case Bw:
      out += "B";
      break;
   case Bb:
      out += "b";
      break;
   case Nw:
      out += "N";
      break;
   case Nb:
      out += "n";
      break;
   case Pw:
      out += "P";
      break;
   case Pb:
      out += "p";
      break;
   default:
      throw std::runtime_error("Invalid piece.");
   }

   return out;
}

enum class RankLabel
{
   Left,
   Right
};

static std::string& printRankLabel(std::string& out, Rank r, RankLabel location)
{
   if (location == RankLabel::Right)
      out += '|';

   out += toChar(r);

   if (location == RankLabel::Left)
      out += '|';

   return out;
}

enum class FileLabel
{
   Top,
   Bottom
};

static std::string& printFileSeparators(std::string& out)
{
   out += "  ";
   for (File f = fa; isValid(f); f = f + 1)
      out += '-';
   out += "\n";

   return out;
}

static std::string& printFileLabels(std::string& out, FileLabel location)
{
   if (location == FileLabel::Bottom)
      printFileSeparators(out);

   // File letters.
   out += "  ";
   for (File f = fa; isValid(f); f = f + 1)
      out += toChar(f);
   out += "\n";

   if (location == FileLabel::Top)
      printFileSeparators(out);

   return out;
}

std::string& printPosition(std::string& out, const Position& pos)
{
   if (out.length() > 0)
      out += "\n";

   // File labeling above board.
   printFileLabels(out, FileLabel::Top);

   for (Rank r = r8; isValid(r); r = r - 1)
   {
      // Rank labeling left of board.
      printRankLabel(out, r, RankLabel::Left);

      for (File f = fa; isValid(f); f = f + 1)
      {
         auto piece = pos[makeSquare(f, r)];
         if (piece)
            printPiece(out, *piece);
         else
            out += ".";
      }

      // Rank labeling right of board.
      printRankLabel(out, r, RankLabel::Right);
      out += "\n";
   }

   // File labeling below board.
   printFileLabels(out, FileLabel::Bottom);

   return out;
}

///////////////////
// Pure algebraic coordinate notation

static bool isValidFilePacn(char c)
{
   return 'a' <= c && c <= 'h';
}

static bool isValidRankPacn(char c)
{
   return '1' <= c && c <= '8';
}

static bool isValidPromotionPacn(char c)
{
   return c == 'q' || c == 'r' || c == 'b' || c == 'n';
}

static std::optional<MoveDescription::Promotion> makePromotionPacn(char c)
{
   switch (c)
   {
   case 'q':
      return MoveDescription::Promotion::Queen;
   case 'r':
      return MoveDescription::Promotion::Rook;
   case 'b':
      return MoveDescription::Promotion::Bishop;
   case 'n':
      return MoveDescription::Promotion::Knight;
   default:
      return {};
   }
}

static std::optional<MoveDescription> readCastlingMovePacn(std::string_view notation)
{
   if (notation.length() >= 5 &&
       (notation.starts_with("o-o-o") || notation.starts_with("O-O-O")))
      return MoveDescription{MoveDescription::Castling::Queenside};

   if (notation.length() >= 3 &&
       (notation.starts_with("o-o") || notation.starts_with("O-O")))
      return MoveDescription{MoveDescription::Castling::Kingside};

   return {};
}

std::optional<MoveDescription> readMovePacn(std::string_view notation)
{
   // Handle special castling notiation:
   // Kingside: o-o
   // Queenside: o-o-o
   // Note that castling can also be notated using coordinates (this will be
   // handled by the general code below):
   // Kingside: e1g1 or e8g8
   // Queenside: e1c1 or e8c8
   if (auto castlingDescr = readCastlingMovePacn(notation); castlingDescr.has_value())
      return castlingDescr;

   // Read coordinate info.
   if (notation.length() < 4)
      return {};

   if (!isValidFilePacn(notation[0]) || !isValidRankPacn(notation[1]) ||
       !isValidFilePacn(notation[2]) || !isValidRankPacn(notation[3]))
      return {};

   const Square from = makeSquare(fileFromChar(notation[0]), rankFromChar(notation[1]));
   const Square to = makeSquare(fileFromChar(notation[2]), rankFromChar(notation[3]));

   // Read promotion info.
   std::optional<MoveDescription::Promotion> promoteTo;
   if (notation.length() >= 5 && isValidPromotionPacn(notation[4]))
      promoteTo = makePromotionPacn(notation[4]);

   return MoveDescription{from, to, promoteTo};
}

} // namespace matt2
