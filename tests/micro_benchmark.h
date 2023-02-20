//
// Feb-2023, Michael Lindner
// MIT license
//
#pragma once
#include <chrono>

template <typename TRes> struct MicroBenchmark
{
   using Clock = std::chrono::high_resolution_clock;

   MicroBenchmark(TRes& res) : result{res} { start = Clock::now(); }
   ~MicroBenchmark()
   {
      const auto end = Clock::now();
      const auto duration =
         std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
      result = duration.count();
   }

   TRes& result;
   Clock::time_point start;
};
