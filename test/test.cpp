
//==============================================================================
//
//  win_helpers.cpp
//
//==============================================================================
//  Copyright (C) Guilaume Plante 2020 <cybercastor@icloud.com>
//==============================================================================


#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <winsock2.h>
#include "win_helpers.h"

using namespace WindowsHelpers;

//#pragma comment(lib, "Ws2_32.lib")

int main()
{
  std::string res = GetLastMsg(false);
  return 0;
}

