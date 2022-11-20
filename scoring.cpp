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
      return 1.;
   case Nw:
   case Nb:
   case Bw:
   case Bb:
      return 3.;
   case Rw:
   case Rb:
      return 5.;
   case Qw:
   case Qb:
      return 9.;
   case Kw:
   case Kb:
      return 100.;
   default:
      throw std::runtime_error("Unknown piece.");
   }
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

} // namespace matt2
