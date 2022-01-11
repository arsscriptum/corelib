

//==============================================================================
//
//   HiResTimer.h  - exported_h
//   High Resolution Timer   
//   Uses Win32 Performance Counters
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#ifndef __HIRESTIMER_H__
#define __HIRESTIMER_H__


/* 
     Module Operations:
     ==================
     This module provides a high resolution timer that can resolve time
     differences to the microsecond level, about 4 orders of magnitude
     more precise than the windows default timers.  You use the timer
     in your code by declaring an instance, starting it, stopping it, 
     and extracting the elapsed time in microseconds.

     Public Interface:
     =================
     HRTimer::HiResTimer hrt;
     hrt.Start();
     // code you want to time here
     hrt.Stop();
     cout << "\n\tTime is: " << hrt.ElapsedMicroseconds() << "\n\n";

 */


#include <windows.h>


class HiResTimer
{
protected:
	__int64 a, b, f;

public:
	HiResTimer();
	__int64 ElapsedTicks();
	__int64 ElapsedMicroseconds();
	__int64 Frequency();
	void Start();
	__int64 Stop();
};


#endif
