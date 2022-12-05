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
   auto it = pos.begin(side, pawn);
   auto end = pos.end(side, pawn);
   for (; it != end; ++it)
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

double DailyChessScore::calcPawnScore()
{
   // Calc piece value and collect pawn stats.
   FileStats_t pawnStats;
   double score = collectPawnStats(m_side, m_pos, pawnStats);

   FileStats_t opponentStats;
   collectPawnStats(!m_side, m_pos, opponentStats);

   score -= calcDoublePawnPenalty(pawnStats);
   score -= calcIsolatedPawnPenalty(pawnStats);
   score += calcPassedPawnBonus(m_side, pawnStats, opponentStats);

   return score;
}

double DailyChessScore::calcKnightScore()
{
   return 0.;
}

double DailyChessScore::calcBishopScore()
{
   return 0.;
}

double DailyChessScore::calcRookScore()
{
   return 0.;
}

double DailyChessScore::calcQueenScore()
{
   return 0.;
}

double DailyChessScore::calcKingScore()
{
   return 0.;
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
