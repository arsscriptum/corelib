

//==============================================================================
//
//  win_helpers.h - exported_h
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#ifndef WINDOWSHELPERS_H
#define WINDOWSHELPERS_H


#include <string>
#include <vector>

namespace WindowsHelpers
{
  std::string wstringToString(const std::wstring& wstr);
  std::wstring stringToWideString(const std::string& str);
  std::string GetLastMsg(bool wantSocketMessage);
}
 
#endif
