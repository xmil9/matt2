//
// Sept-2021, Michael Lindner
// MIT license
//
#include "daily_chess_scoring.h"
#include "piece.h"
#include "piece_value_scoring.h"
#include "position.h"
#include "rules.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <unordered_map>

namespace matt2
{
namespace dcs
{
///////////////////

// Piece values.
constexpr double KingValue = 10000.;
constexpr double QueenValue = 900.;
constexpr double RookValue = 500.;
constexpr double BishopValue = 340.;
constexpr double KnightValue = 325.;
constexpr double PawnValue = 100.;

// Piece value lookup table.
constexpr pvs::PieceValueTable PieceValues{
   KingValue, QueenValue, RookValue, BishopValue, KnightValue, PawnValue,
   KingValue, QueenValue, RookValue, BishopValue, KnightValue, PawnValue,
};

// Max distance between pieces along file or rank.
constexpr int MaxFRDistance = 7;

// Calculates the min distance along file or rank of a piece to the enemy's king.
static std::optional<int> minDistanceToEnemyKing(Piece p, const Position& pos)
{
   if (pos.count(p) == 0)
      return {};

   auto enemyKingSq = pos.kingLocation(!color(p));
   if (!enemyKingSq)
      return {};

   // Find min distance of any instance of the piece along file or rank to the enemy king.
   return std::accumulate(pos.begin(p), pos.end(p), MaxFRDistance,
                          [enemyKingSq](int minDist, Square sq)
                          {
                             const int dist = minDistance(sq, *enemyKingSq);
                             return dist < minDist ? dist : minDist;
                          });
}

///////////////////
// From https://www.dailychess.com/rival/programming/evaluation.php
//
// Static Board Evaluation
//
// The static evaluation function returns a score for the side to move from the given
// position. A score is calculated for both sides and the function returns the score for
// the side on the move minus the score for the side not on the move.
//
// The factors considered in the evaluation function have been chosen because they are
// relatively quick to calculate.Very few of the ideas are entirely original; many
// represent elementary chess knowledge and many have been used in other chess programs.
// Sources that have been of particular influence are Slate & Atkin (1977), Newborn (1975)
// and Hyatt et al (1985). Some of the factors have been added to overcome certain
// weaknesses that the program has shown, others have been left out in the hope that the
// gain in search speed would outweigh the loss in evaluation quality. The work of
// Berliner et al (1990) suggests the opposite to the last assumption.
//
// The majority of factors considered by the evaluation function are described below with
// exact point weightings given where considered appropriate.
//
// Pawn Scoring
// Each pawn scores 100 points. A side is penalised seven points for having two or more
// pawns on the same file (doubled pawns). A two point penalty is inflicted for isolated
// pawns. Passed pawns are awarded a bonus that relates to the pawn's rank number. If
// there is a hostile piece in front of a passed pawn, a value, also relating to the
// pawn's rank number is deducted from the score. Pawns other than those on files one and
// eight are awarded bonuses for advancement ranging from 3 points for a pawn on the third
// rank, third file, to 34 points for a pawn on the seventh rank, fifth file.
//
// Bishop Scoring
// Each bishop scores 340 points. Each of the squares diagonally adjacent to the bishop's
// square are considered with a penalty being inflicted for each square that is occupied
// by a pawn of either colour. A bonus is given for the presence of two bishops.
//
// Rook Scoring
// Each rook scores 500 points. Rooks are awarded a bonus for king tropism that is based
// on the minimum of the rank and file distances from the enemy king. Rooks on the seventh
// rank score 20 points. If two friendly rooks share the same file, the side receives a
// bonus of 15 points. If there are no pawns on the same file as a rook, a bonus of 10
// points is given. If there are e nemy pawns on the same file but no friendly pawns, a
// bonus of three points is given.
//
// Knight Scoring
// Each knight scores 325 points. Knights are awarded bonuses for closeness to the centre
// of the board ranging from -14 points for a corner square to +7 points for a centre
// square. Knights are also awarded points for closeness to the enemy king. Unlike rooks,
// a score is awarded based on the sum of the rank and file distances from the enemy king.
//
// Queen Scoring
// Each queen scores 900 points. Queens are awarded points for closeness to the enemy
// king. A experimental factor has been added for queen scoring since the game in which
// position 3.4 occurred; a small bonus is awarded if a queen is on the same diagonal as a
// friendly bishop.
//
//    King Safety If the number of enemy pieces and pawns in the friendly king's board
//    quadrant is greater than the number of friendly pieces and pawns in the same
//    quadrant, the side is penalised the difference multiplied by five. When considering
//    enemy presence in the quadrant a queen is counted as three pieces.
//
//    If a side has not castled and
//    castling is no longer possible,
//    that side is penalised 15 points.If castling is still possible then a penalty is
//       given if one of the rooks has moved; 12 points for the king's rook, 8 points for
//       the queen's rook.
//
// The evaluation function does not detect checkmate. Evaluation of won, drawn of lost
// positions is left to a function that is called when a position is found in the search
// from which there are no available moves. The value of a won position is 10,000 points
// although the depth at which such a position is discovered is subtracted from this
// score. This encourages the program to take the shortest sequence of moves to win a
// game. Similarly, the depth at which lost positions are discovered is added to the value
// -10,000 to encourage to program to delay the loss for as long as possible in the
// unsportsmanlike hope that the opponent will make a mistake.

class DailyChessScore
{
 public:
   DailyChessScore(const Position& pos, Color side, Rules rules);
   ~DailyChessScore() = default;
   DailyChessScore(const DailyChessScore&) = delete;
   DailyChessScore& operator=(const DailyChessScore&) = delete;

   double score() const { return m_score; }
   double calc();

 private:
   double calcPawnScore();
   double calcKnightScore();
   double calcBishopScore();
   double calcRookScore();
   double calcQueenScore();
   double calcKingScore();

 private:
   bool useRule(Rules rule) const;

 private:
   const Position& m_pos;
   Color m_side = White;
   Rules m_rules = Rules::All;
   double m_score = 0.;
};

DailyChessScore::DailyChessScore(const Position& pos, Color side, Rules rules)
: m_pos{pos}, m_side{side}, m_rules{rules}
{
}

double DailyChessScore::calc()
{
   m_score = 0.;
   m_score += calcPawnScore();
   m_score += calcKnightScore();
   m_score += calcBishopScore();
   m_score += calcRookScore();
   m_score += calcQueenScore();
   m_score += calcKingScore();
   return m_score;
}

using Ranks_t = std::vector<Rank>;
// Occupied ranks by file.
using FileStats_t = std::unordered_map<File, Ranks_t>;

static double collectPawnStats(Color side, const Position& pos, FileStats_t& stats)
{
   // Calc piece value of pawns while we are iterating the pawn.
   double pawnValue = 0.;

   stats = {{fa, {}}, {fb, {}}, {fc, {}}, {fd, {}},
            {fe, {}}, {ff, {}}, {fg, {}}, {fh, {}}};

   const Piece p = pawn(side);
   auto end = pos.end(p);
   for (auto it = pos.begin(p); it != end; ++it)
   {
      const Square sq = *it;
      const Rank r = rank(sq);
      stats[file(sq)].push_back(r);

      pawnValue += pvs::lookupValue(p, PieceValues);
   }

   return pawnValue;
}

static bool isDoublePawn(const Ranks_t& pawnRanksOfFile)
{
   return pawnRanksOfFile.size() > 1;
}

static size_t countDoublePawns(const FileStats_t& stats)
{
   return std::accumulate(std::begin(stats), std::end(stats), 0,
                          [](size_t val, const auto& fileElem)
                          { return val + isDoublePawn(fileElem.second) ? 1 : 0; });
}

// A side is penalised for having two or more pawns on the same file (doubled
// pawns).
static double calcDoublePawnPenalty(const FileStats_t& pawnStats)
{
   constexpr double DoublePawnPenality = 7.;
   return countDoublePawns(pawnStats) * DoublePawnPenality;
}

static bool isIsolatedPawn(File f, const FileStats_t& stats)
{
   // Is pawn on checked file?
   if (stats.at(f).empty())
      return false;

   // Count its neighbors.
   size_t numNeighbors = 0;
   if (f != fa && !stats.at(f - 1).empty())
      ++numNeighbors;
   if (f != fh && !stats.at(f + 1).empty())
      ++numNeighbors;

   return numNeighbors == 0;
}

static size_t countIsolatedPawns(const FileStats_t& stats)
{
   return std::accumulate(std::begin(stats), std::end(stats), 0,
                          [&stats](size_t val, const auto& fileElem) {
                             return val + isIsolatedPawn(fileElem.first, stats) ? 1 : 0;
                          });
}

// A penalty is inflicted for isolated pawns.
static double calcIsolatedPawnPenalty(const FileStats_t& pawnStats)
{
   constexpr double IsolatedPawnPenality = 2.;
   return countIsolatedPawns(pawnStats) * IsolatedPawnPenality;
}

static bool hasOpponentPawnInFront(Color side, Rank sideRank,
                                   const Ranks_t& opponentRanks)
{
   auto isInFront = [](Color side, Rank a, Rank b)
   { return side == White ? a > b : a < b; };

   for (Rank opponentRank : opponentRanks)
      if (isInFront(side, opponentRank, sideRank))
         return true;
   return false;
}

static double calcPassedPawnBonus(Color side, File f, Rank r,
                                  const FileStats_t& opponentsStats)
{
   // Check for opponent pawns in front on file of pawn.
   if (hasOpponentPawnInFront(side, r, opponentsStats.at(f)))
      return 0.;

   // Check for opponent pawns in front on adjacent files.
   if (f != fa && hasOpponentPawnInFront(side, r, opponentsStats.at(f - 1)))
      return 0.;
   if (f != fh && hasOpponentPawnInFront(side, r, opponentsStats.at(f + 1)))
      return 0.;

   constexpr double PassedPawnRankFactor = 1.;
   const size_t rankNumber =
      side == White ? static_cast<size_t>(r) : 9 - static_cast<size_t>(r);
   return rankNumber * PassedPawnRankFactor;
}

// Passed pawns are awarded a bonus that relates to the pawn's rank number. If there is a
// hostile piece in front of a passed pawn, a value, also relating to the pawn's rank
// number is deducted from the score.
static double calcPassedPawnBonus(Color side, const FileStats_t& pawnStats,
                                  const FileStats_t& opponentsStats)
{
   return std::accumulate(std::begin(pawnStats), std::end(pawnStats), 0.,
                          [side, &opponentsStats](double val, const auto& fileElem)
                          {
                             double fileBonus = 0.;
                             for (Rank r : fileElem.second)
                                fileBonus += calcPassedPawnBonus(side, fileElem.first, r,
                                                                 opponentsStats);
                             return val + fileBonus;
                          });
}

// Pawns other than those on files one and eight are awarded bonuses for advancement.
static double calcPawnPositionBonus(Color side, const FileStats_t& pawnStats)
{
   // clang-format off
   static const std::unordered_map<Square, double> WhitePosScore = {
      {a2, 0.}, {b2, 0.},  {c2, 0.},  {d2, 0.},  {e2, 0.},  {f2, 0.},  {g2, 0.},  {h2, 0.},
      {a3, 0.}, {b3, 2.},  {c3, 12.}, {d3, 22.}, {e3, 22.}, {f3, 12.}, {g3, 2.},  {h3, 0.},
      {a4, 0.}, {b4, 4.},  {c4, 14.}, {d4, 24.}, {e4, 24.}, {f4, 14.}, {g4, 4.},  {h4, 0.},
      {a5, 0.}, {b5, 6.},  {c5, 16.}, {d5, 26.}, {e5, 26.}, {f5, 16.}, {g5, 6.},  {h5, 0.},
      {a6, 0.}, {b6, 8.},  {c6, 18.}, {d6, 28.}, {e6, 28.}, {f6, 18.}, {g6, 8.},  {h6, 0.},
      {a7, 0.}, {b7, 10.}, {c7, 20.}, {d7, 30.}, {e7, 30.}, {f7, 20.}, {g7, 10.}, {h7, 0.},
   };
   static const std::unordered_map<Square, double> BlackPosScore = {
      {a7, 0.}, {b7, 0.},  {c7, 0.},  {d7, 0.},  {e7, 0.},  {f7, 0.},  {g7, 0.},  {h7, 0.},
      {a6, 0.}, {b6, 2.},  {c6, 12.}, {d6, 22.}, {e6, 22.}, {f6, 12.}, {g6, 2.},  {h6, 0.},
      {a5, 0.}, {b5, 4.},  {c5, 14.}, {d5, 24.}, {e5, 24.}, {f5, 14.}, {g5, 4.},  {h5, 0.},
      {a4, 0.}, {b4, 6.},  {c4, 16.}, {d4, 26.}, {e4, 26.}, {f4, 16.}, {g4, 6.},  {h4, 0.},
      {a3, 0.}, {b3, 8.},  {c3, 18.}, {d3, 28.}, {e3, 28.}, {f3, 18.}, {g3, 8.},  {h3, 0.},
      {a2, 0.}, {b2, 10.}, {c2, 20.}, {d2, 30.}, {e2, 30.}, {f2, 20.}, {g2, 10.}, {h2, 0.},
   };
   // clang-format on

   const auto& posScore = side == White ? WhitePosScore : BlackPosScore;
   return std::accumulate(std::begin(pawnStats), std::end(pawnStats), 0.,
                          [side, &posScore](double val, const auto& fileElem)
                          {
                             double fileBonus = 0.;
                             const File& f = fileElem.first;
                             for (Rank r : fileElem.second)
                                fileBonus += posScore.at(makeSquare(f, r));
                             return val + fileBonus;
                          });
}

double DailyChessScore::calcPawnScore()
{
   double score = 0.;

   // Calc piece value and collect pawn stats.
   FileStats_t pawnStats;
   const double pieceValue = collectPawnStats(m_side, m_pos, pawnStats);
   FileStats_t opponentStats;
   collectPawnStats(!m_side, m_pos, opponentStats);

   if (useRule(Rules::PawnPieceValue))
      score += pieceValue;
   if (useRule(Rules::PawnPositionBonus))
      score += calcPawnPositionBonus(m_side, pawnStats);
   if (useRule(Rules::PassedPawnBonus))
      score += calcPassedPawnBonus(m_side, pawnStats, opponentStats);
   if (useRule(Rules::DoublePawnPenalty))
      score -= calcDoublePawnPenalty(pawnStats);
   if (useRule(Rules::IsolatedPawnPenalty))
      score -= calcIsolatedPawnPenalty(pawnStats);

   return score;
}

// Knights are awarded bonuses for closeness to the centre of the board.
static double calcKnightCenterBonus(Color side, const Position& pos)
{
   // clang-format off
   static const std::unordered_map<Square, double> PosScore = {
      {a1, -14.}, {b1, -7.}, {c1, -7.}, {d1, -7.}, {e1, -7.}, {f1, -7.}, {g1, -7.}, {h1, -14.},
      {a2, -7.},  {b2, 0.},  {c2, 0.},  {d2, 0.},  {e2, 0.},  {f2, 0.},  {g2, 0.},  {h2, -7.},
      {a3, -7.},  {b3, 0.},  {c3, 4.},  {d3, 4.},  {e3, 4.},  {f3, 4.},  {g3, 0.},  {h3, -7.},
      {a4, -7.},  {b4, 0.},  {c4, 4.},  {d4, 7.},  {e4, 7.},  {f4, 4.},  {g4, 0.},  {h4, -7.},
      {a5, -7.},  {b5, 0.},  {c5, 4.},  {d5, 7.},  {e5, 7.},  {f5, 4.},  {g5, 0.},  {h5, -7.},
      {a6, -7.},  {b6, 0.},  {c6, 4.},  {d6, 4.},  {e6, 4.},  {f6, 4.},  {g6, 0.},  {h6, -7.},
      {a7, -7.},  {b7, 0.},  {c7, 0.},  {d7, 0.},  {e7, 0.},  {f7, 0.},  {g7, 0.},  {h7, -7.},
      {a8, -14.}, {b8, -7.}, {c8, -7.}, {d8, -7.}, {e8, -7.}, {f8, -7.}, {g8, -7.}, {h8, -14.},
   };
   // clang-format on

   const Piece kn = knight(side);
   return std::accumulate(pos.begin(kn), pos.end(kn), 0.,
                          [side](double val, Square sq)
                          { return val + PosScore.at(sq); });
}

// Calculate bonus for an individual knight's closeness to the enemy king.
static double calcKnightKingClosenessBonus(Square knightSq, Square enemyKingSq)
{
   constexpr int MaxDistSum = 2 * MaxFRDistance;

   const Offset off = offset(knightSq, enemyKingSq);
   const int distSum = std::abs(off.df) + std::abs(off.dr);

   // Higher bonus the closer the distance is.
   constexpr double EnemyKingDistanceBonus = 1.;
   return (MaxDistSum - distSum) * EnemyKingDistanceBonus;
}

// Calculate bonus for an all knights' closeness to the enemy king.
static double calcKnightKingClosenessBonus(Color side, const Position& pos)
{
   const Piece kn = knight(side);

   auto enemyKingSq = pos.kingLocation(!side);
   if (!enemyKingSq)
      return 0.;

   // Sum up bonus for all knights.
   return std::accumulate(
      pos.begin(kn), pos.end(kn), 0.,
      [enemyKingSq](double val, Square knightSq)
      { return val + calcKnightKingClosenessBonus(knightSq, *enemyKingSq); });
}

double DailyChessScore::calcKnightScore()
{
   double score = 0.;

   if (useRule(Rules::KnightPieceValue))
      score += pvs::score(m_pos, m_side == White ? Nw : Nb, PieceValues);
   if (useRule(Rules::KnightCenterBonus))
      score += calcKnightCenterBonus(m_side, m_pos);
   if (useRule(Rules::KnightKingClosenessBonus))
      score += calcKnightKingClosenessBonus(m_side, m_pos);

   return score;
}

// A bonus is given for the presence of two bishops.
static double calcMultipleBishopBonus(Color side, const Position& pos)
{
   if (pos.count(bishop(side)) < 2)
      return 0.;

   constexpr double MultipleBishopBonus = 20.;
   return MultipleBishopBonus;
}

static bool isPawnDiagonalNeighbor(Square sq, const Position& pos)
{
   const File f = file(sq);
   const Rank r = rank(sq);

   std::vector<std::optional<Piece>> diagNeighbors;
   if (isValid(f, 1) && isValid(r, 1))
      diagNeighbors.push_back(pos[makeSquare(f + 1, r + 1)]);
   if (isValid(f, 1) && isValid(r, -1))
      diagNeighbors.push_back(pos[makeSquare(f + 1, r - 1)]);
   if (isValid(f, -1) && isValid(r, 1))
      diagNeighbors.push_back(pos[makeSquare(f - 1, r + 1)]);
   if (isValid(f, -1) && isValid(r, -1))
      diagNeighbors.push_back(pos[makeSquare(f - 1, r - 1)]);

   return std::count_if(std::begin(diagNeighbors), std::end(diagNeighbors),
                        [](const std::optional<Piece>& piece)
                        { return piece.has_value() && isPawn(*piece); }) > 0;
}

// Each of the squares diagonally adjacent to the bishop's square are considered with a
// penalty being inflicted for each square that is occupied by a pawn of either colour.
static double calcAdjacentPawnBishopPenalty(Color side, const Position& pos)
{
   const Piece p = bishop(side);
   return std::accumulate(pos.begin(p), pos.end(p), 0.,
                          [&pos](double val, Square sq)
                          {
                             constexpr double AdjacentPawnPenality = 5.;
                             return val + isPawnDiagonalNeighbor(sq, pos)
                                       ? AdjacentPawnPenality
                                       : 0.;
                          });
}

double DailyChessScore::calcBishopScore()
{
   double score = 0.;

   if (useRule(Rules::BishopPieceValue))
      score += pvs::score(m_pos, bishop(m_side), PieceValues);
   if (useRule(Rules::MultipleBishopBonus))
      score += calcMultipleBishopBonus(m_side, m_pos);
   if (useRule(Rules::BishopAdjacentPawnPenality))
      score -= calcAdjacentPawnBishopPenalty(m_side, m_pos);

   return score;
}

static std::vector<File> collectPieceFilesSorted(Piece p, const Position& pos)
{
   std::vector<File> files;
   std::transform(pos.begin(p), pos.end(p), std::back_inserter(files),
                  [](Square sq) { return file(sq); });
   std::sort(std::begin(files), std::end(files));

   return files;
}

// Rooks are awarded a bonus for king tropism that is based on the minimum of the rank and
// file distances from the enemy king.
static double calcRookKingClosenessBonus(Color side, const Position& pos)
{
   auto minDist = minDistanceToEnemyKing(rook(side), pos);
   if (!minDist)
      return 0.;

   // Higher bonus the closer the distance is.
   constexpr double EnemyKingDistanceBonus = 5.;
   return (MaxFRDistance - *minDist) * EnemyKingDistanceBonus;
}

// Rooks on the seventh rank receive a bonus.
static double calcRookSeventhRankBonus(Color side, const Position& pos)
{
   const Piece r = rook(side);
   const Rank seventhRank = side == White ? r7 : r2;
   const bool on7th =
      std::any_of(pos.begin(r), pos.end(r),
                  [seventhRank](Square sq) { return rank(sq) == seventhRank; });

   constexpr double SeventhRankBonus = 20.;
   return on7th ? SeventhRankBonus : 0.;
}

// If two friendly rooks share the same file, the side receives a bonus.
static double calcRookSharedFileBonus(const std::vector<File>& sortedRookFiles)
{
   // Check if any consecutive files are the same, i.e. shared between those pieces.
   const auto last = std::end(sortedRookFiles);
   const bool onSameFile = std::adjacent_find(std::begin(sortedRookFiles), last) != last;

   constexpr double SharedFileBonus = 15.;
   return onSameFile ? SharedFileBonus : 0.;
}

// If there are no pawns on the same file as a rook, a bonus is given.
// If there are enemy pawns on the same file but no friendly pawns, a smaller bonus is
// given.
static double calcRookPawnsOnFileBonus(Color side, const Position& pos,
                                       const std::vector<File>& sortedRookFiles)
{
   const std::vector<File> ownPawnFilesSorted = collectPieceFilesSorted(pawn(side), pos);
   auto ownFirst = std::begin(ownPawnFilesSorted);
   auto ownLast = std::end(ownPawnFilesSorted);

   const std::vector<File> enemyPawnFilesSorted =
      collectPieceFilesSorted(pawn(!side), pos);
   auto enemyFirst = std::begin(enemyPawnFilesSorted);
   auto enemyLast = std::end(enemyPawnFilesSorted);

   size_t numFilesWithoutPawn = 0;
   size_t numFilesWithOnlyEnemyPawn = 0;
   for (File rookFile : sortedRookFiles)
   {
      const bool hasOwnPawn = std::find(ownFirst, ownLast, rookFile) != ownLast;
      const bool hasEnemyPawn = std::find(enemyFirst, enemyLast, rookFile) != enemyLast;

      if (!hasOwnPawn && !hasEnemyPawn)
         ++numFilesWithoutPawn;
      if (!hasOwnPawn && hasEnemyPawn)
         ++numFilesWithOnlyEnemyPawn;
   }

   constexpr double NoPawnsOnFileBonus = 10.;
   constexpr double OnlyEnemyPawnsOnFileBonus = 3.;
   return numFilesWithoutPawn * NoPawnsOnFileBonus +
          numFilesWithOnlyEnemyPawn * OnlyEnemyPawnsOnFileBonus;
}

double DailyChessScore::calcRookScore()
{
   const std::vector<File> sortedRookFiles = collectPieceFilesSorted(rook(m_side), m_pos);

   double score = 0.;

   if (useRule(Rules::RookPieceValue))
      score += pvs::score(m_pos, rook(m_side), PieceValues);
   if (useRule(Rules::RookKingClosenessBonus))
      score += calcRookKingClosenessBonus(m_side, m_pos);
   if (useRule(Rules::RookSeventhRankBonus))
      score += calcRookSeventhRankBonus(m_side, m_pos);
   if (useRule(Rules::RookSharedFileBonus))
      score += calcRookSharedFileBonus(sortedRookFiles);
   if (useRule(Rules::RookPawnsOnFileBonus))
      score += calcRookPawnsOnFileBonus(m_side, m_pos, sortedRookFiles);

   return score;
}

// Queens are awarded points for closeness to the enemy king.
static double calcQueenKingClosenessBonus(Color side, const Position& pos)
{
   auto minDist = minDistanceToEnemyKing(queen(side), pos);
   if (!minDist)
      return 0.;

   // Higher bonus the closer the distance is.
   constexpr double EnemyKingDistanceBonus = 5.;
   return (MaxFRDistance - *minDist) * EnemyKingDistanceBonus;
}

// A small bonus is awarded if a queen is on the same diagonal as a friendly bishop.
static double calcQueenBishopDiagonalBonus(Color side, const Position& pos)
{
   const Piece q = queen(side);
   const Piece b = bishop(side);

   // Collect all friendly bishop squares.
   std::vector<Square> bishopSquares;
   std::transform(pos.begin(b), pos.end(b), std::back_inserter(bishopSquares),
                  [](Square sq) { return sq; });

   // For all queens calculate the bonus of shared diagonals with bishops.
   return std::accumulate(pos.begin(q), pos.end(q), 0.,
                          [&bishopSquares](double bonus, Square queenSq)
                          {
                             // Count shared diagonals with bishops.
                             auto numSharedDiags = std::count_if(
                                std::begin(bishopSquares), std::end(bishopSquares),
                                [queenSq](Square bishopSq)
                                { return onSameDiagonal(bishopSq, queenSq); });

                             constexpr double QueenBishopDiagonalBonus = 1.;
                             return bonus + numSharedDiags * QueenBishopDiagonalBonus;
                          });
}

double DailyChessScore::calcQueenScore()
{
   double score = 0.;

   if (useRule(Rules::QueenPieceValue))
      score += pvs::score(m_pos, queen(m_side), PieceValues);
   if (useRule(Rules::QueenKingClosenessValue))
      score += calcQueenKingClosenessBonus(m_side, m_pos);
   if (useRule(Rules::QueenBishopDiagonalClosenessValue))
      score += calcQueenBishopDiagonalBonus(m_side, m_pos);

   return score;
}

static size_t countPiecesInQuadrant(Quadrant quad, Color side, const Position& pos,
                                    size_t queenValue, size_t kingValue)
{
   size_t count = 0;

   const auto end = pos.end(side);
   for (auto iter = pos.begin(side); iter != end; ++iter)
   {
      if (inQuadrant(iter.at(), quad))
      {
         Piece p = iter.piece();
         // Queen can have custom value.
         if (isQueen(p))
            count += queenValue;
         // King can have custom value - used to exclude it from own piece count.
         else if (isKing(p))
            count += kingValue;
         else
            count += 1;
      }
   }

   return count;
}

// If the number of enemy pieces and pawns in the friendly king's board quadrant is
// greater than the number of friendly pieces and pawns in the same quadrant, the side is
// penalised the difference multiplied by five. When considering enemy presence in the
// quadrant a queen is counted as three pieces.
static double calcKingQuadrantPenality(Color side, const Position& pos)
{
   const auto kingSq = pos.kingLocation(side);
   if (!kingSq)
      return 0.;
   // King has to be in quadrant on its side of the board.
   const Quadrant kingQuad = quadrant(*kingSq);
   if (!isFriendlyQuadrant(kingQuad, side))
      return 0.;

   constexpr size_t QueenValueInKingQuadrant = 3;
   const size_t numFriendly =
      countPiecesInQuadrant(kingQuad, side, pos, QueenValueInKingQuadrant, 0);
   const size_t numEnemy =
      countPiecesInQuadrant(kingQuad, !side, pos, QueenValueInKingQuadrant, 1);
   if (numEnemy <= numFriendly)
      return 0.;

   constexpr double KingQuadrantPenaltyFactor = 5.;
   return (numEnemy - numFriendly) * KingQuadrantPenaltyFactor;
}

// If a side has not castled and castling is no longer possible, that side is penalised.
// If castling is still possible then a penalty is given if one of the rooks has
// moved; more points for the king's rook than for the queen's rook.
static double calcKingCastlingPenality(Color side, const Position& pos)
{
   const bool canCastleKingside = canCastle(side, true, pos);
   const bool canCastleQueenside = canCastle(side, false, pos);
   const bool canCastle = canCastleKingside || canCastleQueenside;
   const Position::CastlingState castleState = pos.castlingState(side);

   if (!canCastle && !castleState.hasCastled)
   {
      constexpr double NeverCastledPenality = 15.;
      return NeverCastledPenality;
   }

   if (canCastle)
   {
      constexpr double KingsideRookMovedBeforeCastlingPenality = 12.;
      constexpr double QueensideRookMovedBeforeCastlingPenality = 8.;

      if (castleState.hasKingsideRookMoved)
         return KingsideRookMovedBeforeCastlingPenality;
      else if (castleState.hasQueensideRookMoved)
         return QueensideRookMovedBeforeCastlingPenality;
   }

   return 0.;
}

double DailyChessScore::calcKingScore()
{
   double score = 0.;

   if (useRule(Rules::KingPieceValue))
      score += pvs::score(m_pos, king(m_side), PieceValues);
   if (useRule(Rules::KingQuadrantPenalty))
      score -= calcKingQuadrantPenality(m_side, m_pos);
   if (useRule(Rules::KingCastlingPenalty))
      score -= calcKingCastlingPenality(m_side, m_pos);

   return score;
}

bool DailyChessScore::useRule(Rules rule) const
{
   const uint64_t ruleFlag = static_cast<uint64_t>(rule);
   return (static_cast<uint64_t>(m_rules) & ruleFlag) == ruleFlag;
}

///////////////////

double score(const Position& pos, Color side, Rules rules)
{
   return DailyChessScore{pos, side, rules}.calc();
}

double score(const Position& pos, Rules rules)
{
   return score(pos, White, rules) - score(pos, Black, rules);
}

} // namespace dcs
} // namespace matt2
