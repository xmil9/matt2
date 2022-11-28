//
// Sept-2021, Michael Lindner
// MIT license
//
#include "scoring.h"
#include "position.h"
#include <numeric>

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

 private:
   const Position& m_pos;
   Color m_side;
};

DailyChessScore::DailyChessScore(const Position& pos, Color side)
: m_pos{pos}, m_side{side}
{
}

///////////////////

double calcDailyChessScore(const Position& pos, Color side)
{
   // todo
   pos;
   side;
   return 0.;
}

double calcDailyChessScore(const Position& pos)
{
   return calcDailyChessScore(pos, Color::White) - calcDailyChessScore(pos, Color::Black);
}

} // namespace matt2
