//
// Sept-2021, Michael Lindner
// MIT license
//
#include "scoring.h"
#include "piece.h"
#include "position.h"
#include <algorithm>
#include <functional>
#include <numeric>
#include <unordered_map>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace matt2
{
///////////////////

static double pieceValue(Piece piece)
{
   switch (piece)
   {
   case Pw:
   case Pb:
      return 100.;
   case Nw:
   case Nb:
      return 325.;
   case Bw:
   case Bb:
      return 340.;
   case Rw:
   case Rb:
      return 500.;
   case Qw:
   case Qb:
      return 900.;
   case Kw:
   case Kb:
      return 10000.;
   default:
      throw std::runtime_error("Unknown piece.");
   }
}

///////////////////

static double calcPieceValueScore(const Position& pos, Piece piece)
{
   return std::accumulate(pos.begin(piece), pos.end(piece), 0.,
                          [piece](double currValue, Square /*sq*/)
                          { return currValue + pieceValue(piece); });
}

static double calcPieceValueScore(const Position& pos, Color side)
{
   return std::accumulate(pos.begin(side), pos.end(side), 0.,
                          [](double currValue, const Placement& placement)
                          { return currValue + pieceValue(placement.piece()); });
}

double calcPieceValueScore(const Position& pos)
{
   return calcPieceValueScore(pos, Color::White) - calcPieceValueScore(pos, Color::Black);
}

///////////////////

struct PawnStats
{
};

class DailyChessScore
{
 public:
   DailyChessScore(const Position& pos, Color side);
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
   const Position& m_pos;
   Color m_side;
   double m_score = 0.;
};

DailyChessScore::DailyChessScore(const Position& pos, Color side)
: m_pos{pos}, m_side{side}
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

      pawnValue += pieceValue(p);
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

static double calcIsolatedPawnPenalty(const FileStats_t& pawnStats)
{
   constexpr double IsolatedPawnPenality = 2.;
   return countIsolatedPawns(pawnStats) * IsolatedPawnPenality;
}

static bool hasOpponentPawnInFront(Color side, Rank sideRank,
                                   const Ranks_t& opponentRanks)
{
   auto isInFront = [](Color side, Rank a, Rank b)
   { return side == Color::White ? a > b : a < b; };

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
      side == Color::White ? static_cast<size_t>(r) : 9 - static_cast<size_t>(r);
   return rankNumber * PassedPawnRankFactor;
}

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

   const auto& posScore = side == Color::White ? WhitePosScore : BlackPosScore;
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
   // Calc piece value and collect pawn stats.
   FileStats_t pawnStats;
   double score = collectPawnStats(m_side, m_pos, pawnStats);

   FileStats_t opponentStats;
   collectPawnStats(!m_side, m_pos, opponentStats);

   score += calcPawnPositionBonus(m_side, pawnStats);
   score += calcPassedPawnBonus(m_side, pawnStats, opponentStats);
   score -= calcDoublePawnPenalty(pawnStats);
   score -= calcIsolatedPawnPenalty(pawnStats);

   return score;
}

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
   constexpr int MaxDistSum = 14;

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
   double score = calcPieceValueScore(m_pos, m_side == Color::White ? Nw : Nb);
   score += calcKnightCenterBonus(m_side, m_pos);
   score += calcKnightKingClosenessBonus(m_side, m_pos);
   return score;
}

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
   double score = calcPieceValueScore(m_pos, bishop(m_side));
   score += calcMultipleBishopBonus(m_side, m_pos);
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

static double calcRookKingClosenessBonus(Color side, const Position& pos)
{
   const Piece ownRook = rook(side);
   if (pos.count(ownRook) == 0)
      return 0.;

   auto enemyKingSq = pos.kingLocation(!side);
   if (!enemyKingSq)
      return 0.;

   // Find min distance of any rook along file or rank to the enemy king.
   constexpr int MaxDist = 7;
   const int minDist = std::accumulate(pos.begin(ownRook), pos.end(ownRook), MaxDist,
                                       [enemyKingSq](int minDist, Square rookSq)
                                       {
                                          const int dist =
                                             minDistance(rookSq, *enemyKingSq);
                                          return dist < minDist ? dist : minDist;
                                       });

   // Higher bonus the closer the distance is.
   constexpr double EnemyKingDistanceBonus = 5.;
   return (MaxDist - minDist) * EnemyKingDistanceBonus;
}

static double calcRookSeventhRankBonus(Color side, const Position& pos)
{
   const Piece r = rook(side);
   const Rank seventhRank = side == Color::White ? r7 : r2;
   const bool on7th =
      std::any_of(pos.begin(r), pos.end(r),
                  [seventhRank](Square sq) { return rank(sq) == seventhRank; });

   constexpr double SeventhRankBonus = 20.;
   return on7th ? SeventhRankBonus : 0.;
}

static double calcRookSharedFileBonus(const std::vector<File>& sortedRookFiles)
{
   // Check if any consecutive files are the same, i.e. shared between those pieces.
   const auto last = std::end(sortedRookFiles);
   const bool onSameFile = std::adjacent_find(std::begin(sortedRookFiles), last) != last;

   constexpr double SharedFileBonus = 15.;
   return onSameFile ? SharedFileBonus : 0.;
}

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
   std::vector<File> sortedRookFiles = collectPieceFilesSorted(rook(m_side), m_pos);

   double score = calcPieceValueScore(m_pos, rook(m_side));
   score += calcRookKingClosenessBonus(m_side, m_pos);
   score += calcRookSeventhRankBonus(m_side, m_pos);
   score += calcRookSharedFileBonus(sortedRookFiles);
   score += calcRookPawnsOnFileBonus(m_side, m_pos, sortedRookFiles);
   return score;
}

static double calcQueenKingClosenessBonus(Color side, const Position& pos)
{
   const Piece q = queen(side);
   if (pos.count(q) == 0)
      return 0.;

   auto enemyKingSq = pos.kingLocation(!side);
   if (!enemyKingSq)
      return 0.;

   // Find min distance of any queen along file or rank to the enemy king.
   constexpr int MaxDist = 7;
   const int minDist = std::accumulate(pos.begin(q), pos.end(q), MaxDist,
                                       [enemyKingSq](int minDist, Square queenSq)
                                       {
                                          const int dist =
                                             minDistance(queenSq, *enemyKingSq);
                                          return dist < minDist ? dist : minDist;
                                       });

   // Higher bonus the closer the distance is.
   constexpr double EnemyKingDistanceBonus = 5.;
   return (MaxDist - minDist) * EnemyKingDistanceBonus;
}

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
   double score = calcPieceValueScore(m_pos, queen(m_side));
   score += calcQueenKingClosenessBonus(m_side, m_pos);
   score += calcQueenBishopDiagonalBonus(m_side, m_pos);
   return score;
}

double DailyChessScore::calcKingScore()
{
   double score = calcPieceValueScore(m_pos, king(m_side));
   return score;
}

///////////////////

double calcDailyChessScore(const Position& pos, Color side)
{
   DailyChessScore scoring{pos, side};
   return scoring.calc();
}

double calcDailyChessScore(const Position& pos)
{
   return calcDailyChessScore(pos, Color::White) - calcDailyChessScore(pos, Color::Black);
}

} // namespace matt2
