//
// essentutils
// Time utilities.
//
// Apr-2021, Michael Lindner
// MIT license
//
#pragma once
#include <chrono>


namespace esl
{
///////////////////

struct TimeMeasurement
{
   struct StartTag
   {
   };
   static const StartTag Start;

   TimeMeasurement() = default;
   explicit TimeMeasurement(StartTag);

   void start();
   TimeMeasurement& stop();
   template <typename Unit = std::chrono::milliseconds> int64_t length() const;

   using Clock = std::chrono::system_clock;
   Clock::time_point started;
   mutable Clock::time_point stopped;
};


inline TimeMeasurement::TimeMeasurement(StartTag)
{
   start();
}


inline void TimeMeasurement::start()
{
   started = Clock::now();
}


inline TimeMeasurement& TimeMeasurement::stop()
{
   stopped = Clock::now();
   return *this;
}


template <typename Unit> int64_t TimeMeasurement::length() const
{
   return static_cast<int64_t>(
      std::chrono::duration_cast<Unit>(stopped - started).count());
}

} // namespace esl
