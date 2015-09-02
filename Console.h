#ifndef Console_h
#define Console_h

#include "AST.h"

#ifdef WIN32
#include <Windows.h>
#include <Wincon.h>

namespace Console
{
	enum Color {
		BLACK = 0,
		DARKBLUE = FOREGROUND_BLUE,
		DARKGREEN = FOREGROUND_GREEN,
		DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DARKRED = FOREGROUND_RED,
		DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		GRAY = FOREGROUND_INTENSITY,
		BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};
}
#endif WIN32

void trace(const char* format, ...);
void info(const char* format, ...);
void warn(const char* format, ...);
void error(const char* format, ...);
void fatal(const char* format, ...);

void print(const char* format, ...);
void println(const char* format, ...);

void SaveConsoleAttributes();
void RestoreConsoleAttributes();
void SetConsoleAttributes(unsigned short attr);

void AddConsoleCtrlHandler();

#endif Console_h