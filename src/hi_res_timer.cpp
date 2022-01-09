
//==============================================================================
//
//   HiResTimer.cpp - High Resolution Timer   
//   Uses Win32 Performance Counters
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#include <exception>
#include "hi_res_timer.h"


  HiResTimer::HiResTimer()
  {
    a = b = 0UL;
    if ( QueryPerformanceFrequency((LARGE_INTEGER*)&f) == 0) 
      throw std::exception("no high resolution counter on this platform");
  }


  __int64 HiResTimer::ElapsedTicks()
  {
    return (b-a);
  }


  __int64 HiResTimer::ElapsedMicroseconds()
  {
    // pick more precise time calculation

    __int64 d = (b-a); 
    if (d < 0x10c6f7a0b5edUL) // 2^64 / 1e6
      return (d*1000000UL)/f; 
    else
      return (d/f)*1000000UL;
  }


  __int64 HiResTimer::Frequency()
  {
    return f;
  }



  void HiResTimer::Start()
  {
    ::Sleep(0);
    QueryPerformanceCounter((LARGE_INTEGER*)&a);
  }


  __int64 HiResTimer::Stop()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)&b);
    return ElapsedTicks();
  }

