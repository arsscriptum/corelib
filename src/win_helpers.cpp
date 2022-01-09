
//==============================================================================
//
//  win_helpers.cpp
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================


#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include "win_helpers.h"



std::string WindowsHelpers::wstringToString(const std::wstring& wstr)
{
  std::string rtn;
  for (auto ch : wstr)
    rtn += static_cast<char>(ch);
  rtn += '\0';
  return rtn;
}

std::wstring WindowsHelpers::stringToWideString(const std::string& str)
{
  std::wstring rtn;
  for (auto ch : str)
    rtn += ch;
  rtn += static_cast<wchar_t>('\0');
  return rtn;
}


std::string WindowsHelpers::GetLastMsg(bool WantSocketMsg) 
{

  DWORD errorCode = GetLastError();
  if (errorCode == 0)
    return "no error";

  // map errorCode into a system defined error string

  DWORD dwFlags =
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER;
  LPCVOID lpSource = NULL;
  DWORD dwMessageID = errorCode;
  DWORD dwLanguageId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US);
  LPSTR lpBuffer;
  DWORD nSize = 0;
  va_list *Arguments = NULL;

  FormatMessage(
    dwFlags, lpSource, dwMessageID, dwLanguageId,
    (LPTSTR)&lpBuffer, nSize, Arguments
    );

  std::string _msg(lpBuffer);
  LocalFree(lpBuffer);
  return _msg;
}
