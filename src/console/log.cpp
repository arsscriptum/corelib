
//==============================================================================
//
//  log.cpp
//
//==============================================================================
//  cybercastor - made in quebec 2020 <cybercastor@icloud.com>
//==============================================================================



#include "console\log.h"
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <wtypes.h>
#include "terminal_formatting.h"
#include "console_colors_define.h"

#pragma todo( log.cpp compiled )

//==============================================================================
// ConsoleOut
// Used by the ServiceTerminal
//==============================================================================
void __cdecl ConsoleOut(std::string color, const char *format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';


	EndOfLineEscapeTag Format{ color, ANSI_TEXT_COLOR_RESET };
	std::clog << Format << buf;
}
 
void __cdecl ConsoleWarning(const char* format, ...)
{
	char    buf[4096], * p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf && isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	EndOfLineEscapeTag FormatTitle{ console::RED_UNDERLINED, ANSI_TEXT_COLOR_RESET };
	EndOfLineEscapeTag FormatText{ console::YELLOW_BRIGHT_UNDERLINED, ANSI_TEXT_COLOR_RESET };
	std::clog << FormatTitle << "[WARN] ";
	std::clog << FormatText << buf;
}

void __cdecl ConsoleTrace(const char* format, ...)
{
	char    buf[4096], * p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf && isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	EndOfLineEscapeTag FormatTitle{ console::BLUE_BOLD, ANSI_TEXT_COLOR_RESET };
	EndOfLineEscapeTag FormatText{ console::WHITE_BOLD, ANSI_TEXT_COLOR_RESET };
	std::clog << FormatTitle << "[app] ";
	std::clog << FormatText << buf;
}

void __cdecl ConsoleLog(const char *format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	EndOfLineEscapeTag FormatTitle{ console::BLUE_BACKGROUND_BRIGHT, console::WHITE_BOLD };
	EndOfLineEscapeTag FormatText{ console::WHITE_BOLD, ANSI_TEXT_COLOR_RESET };
	std::clog << FormatTitle << "[trace] ";
	std::clog << FormatText << buf;
}

void __cdecl ConsoleInfo(const char *format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	EndOfLineEscapeTag FormatTitle{ console::GREEN_UNDERLINED, ANSI_TEXT_COLOR_RESET };
	EndOfLineEscapeTag FormatText{ console::GREEN_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET };
	std::clog << FormatTitle << "[info] ";
	std::clog << FormatText << buf;
}
void __cdecl ConsoleError(const char *format, ...)
{
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	EndOfLineEscapeTag FormatTitle{ CONSOLE_COLOR_RED_BRIGHT, ANSI_TEXT_COLOR_RESET };
	EndOfLineEscapeTag FormatText{ CONSOLE_COLOR_YELLOW_BRIGHT, ANSI_TEXT_COLOR_RESET };
	std::clog << FormatTitle << "[error] ";
	std::clog << FormatText << buf;
}
//==============================================================================
// SystemDebugOutput
// Kernel-mode and Win32 debug output
//   - Win32 OutputDebugString
//   - Kernel - mode DbgPrint
// You can monitor this stream using Debugview from SysInternals
// https://docs.microsoft.com/en-us/sysinternals/downloads/debugview
//==============================================================================
void __cdecl SystemDebugOutput(const wchar_t *channel, const char *format, ...)
{
#ifndef FINAL
	char    buf[4096], *p = buf;
	va_list args;
	int     n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf  &&  isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	OutputDebugStringA(buf);
#ifdef KERNEL_DEBUG
	DbgPrint(buf);
#endif // KERNEL_DEBUG

#endif // FINAL
}


void __cdecl ConsoleDemo()
{

	EndOfLineEscapeTag *FormatText = new EndOfLineEscapeTag( console::WHITE_BOLD, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::CYAN_BOLD, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	std::cout << std::endl << std::endl << "---------------------------------------------" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::PURPLE_BOLD, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::YELLOW_BOLD, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::GREEN_BOLD, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::RED_BOLD, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLACK_BOLD, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	std::cout << std::endl << std::endl << "---------------------------------------------" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::WHITE, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::CYAN, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLUE, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::PURPLE, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::YELLOW, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::GREEN, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::RED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLACK, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	std::cout << std::endl << std::endl << "---------------------------------------------" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::WHITE_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::CYAN_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLUE_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::PURPLE_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::YELLOW_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::GREEN_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::RED_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLACK_UNDERLINED, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;

	std::cout << std::endl << std::endl << "---------------------------------------------" << std::endl;;
		FormatText = new EndOfLineEscapeTag( console::WHITE_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::CYAN_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLUE_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::PURPLE_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::YELLOW_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::GREEN_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::RED_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLACK_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << *FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	std::cout << std::endl << std::endl << "---------------------------------------------" << std::endl;;

	FormatText = new EndOfLineEscapeTag( console::WHITE_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::CYAN_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLUE_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::PURPLE_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::YELLOW_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::GREEN_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::RED_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;
	FormatText = new EndOfLineEscapeTag( console::BLACK_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET ); std::clog << FormatText << "ABCDEFGHIJKLMNOPQRSTUVWXYZ YES THIS IA A TEST 1234567890" << std::endl;;


	FormatText = new EndOfLineEscapeTag(console::RED, ANSI_TEXT_COLOR_RESET); std::clog << FormatText << "ceci est un test" << std::endl;;
	FormatText = new EndOfLineEscapeTag(console::RED_BOLD, ANSI_TEXT_COLOR_RESET); std::clog << FormatText << "ceci est un test" << std::endl;;
	FormatText = new EndOfLineEscapeTag(console::RED_BACKGROUND_BRIGHT, ANSI_TEXT_COLOR_RESET); std::clog << FormatText << "ceci est un test" << std::endl;;
	FormatText = new EndOfLineEscapeTag(console::RED_BOLD_BRIGHT, ANSI_TEXT_COLOR_RESET); std::clog << FormatText << "ceci est un test" << std::endl;;
	FormatText = new EndOfLineEscapeTag(console::RED_UNDERLINED, ANSI_TEXT_COLOR_RESET); std::clog << FormatText << "ceci est un test" << std::endl;;
	
}