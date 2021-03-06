#ifndef Console_h
#define Console_h

#include "Node.h"

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

void Trace(const char* format, ...);
void Trace(const Node* node, const char* format, ...);
void Info(const char* format, ...);
void Warn(const char* format, ...);
void Warn(const Node* node, const char* format, ...);
void Error(const char* format, ...);
void Error(const Node* node, const char* format, ...);
void Fatal(const char* format, ...);
void Fatal(const Node* node, const char* format, ...);
void Print(const char* format, ...);

void SaveConsoleAttributes();
void RestoreConsoleAttributes();
void SetConsoleAttributes(unsigned short attr);

void AddConsoleCtrlHandler();

#endif Console_h