//
// Sept-2021, Michael Lindner
// MIT license
//
#include "scoring.h"
#include "position.h"
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
using FileStats_t = std::unordered_map<File, Ranks_t>;

static double collectPawnStats(Color side, const Position& pos, FileStats_t& stats)
{
   // Calc piece value of pawns while we are iterating the pawn.
   double pawnValue = 0.;

   stats = {{fa, {}}, {fb, {}}, {fc, {}}, {fd, {}},
            {fe, {}}, {ff, {}}, {fg, {}}, {fh, {}}};

   const Piece pawn = side == Color::White ? Pw : Pb;
   auto end = pos.end(pawn);
   for (auto it = pos.begin(pawn); it != end; ++it)
   {
      const Square sq = *it;
      const Rank r = rank(sq);
      stats[file(sq)].push_back(r);

      pawnValue += pieceValue(pawn);
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

double DailyChessScore::calcKnightScore()
{
   double score = calcPieceValueScore(m_pos, m_side == Color::White ? Nw : Nb);
   return score;
}

static double calcMultipleBishopBonus(Color side, const Position& pos)
{
   if (pos.count(side == Color::White ? Bw : Bb) < 2)
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
   const Piece bishop = side == Color::White ? Bw : Bb;
   return std::accumulate(pos.begin(bishop), pos.end(bishop), 0.,
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
   double score = calcPieceValueScore(m_pos, m_side == Color::White ? Bw : Bb);
   score += calcMultipleBishopBonus(m_side, m_pos);
   score -= calcAdjacentPawnBishopPenalty(m_side, m_pos);
   return score;
}

double DailyChessScore::calcRookScore()
{
   double score = calcPieceValueScore(m_pos, m_side == Color::White ? Rw : Rb);
   return score;
}

double DailyChessScore::calcQueenScore()
{
   double score = calcPieceValueScore(m_pos, m_side == Color::White ? Qw : Qb);
   return score;
}

double DailyChessScore::calcKingScore()
{
   double score = calcPieceValueScore(m_pos, m_side == Color::White ? Kw : Kb);
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
