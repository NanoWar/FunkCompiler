#include "Global.h"

#include "Console.h"
#include "AST.h"

#ifdef WIN32

//saved console attributes, to be restored on exit
WORD user_attributes;

#endif WIN32

extern int verbose;
extern int quiet;
extern int errors;
extern int no_colors;

void RestoreConsoleAttributes()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), user_attributes);
#elif !defined(MACVER)
	printf("\x1b[0m");
#endif
}

void SaveConsoleAttributes() 
{
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbiScreenBufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiScreenBufferInfo);
	user_attributes = csbiScreenBufferInfo.wAttributes;
#endif
}

void SetConsoleAttributes(unsigned short attr)
{
	if(no_colors) return;
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)attr);
#elif !defined(MACVER)
	printf("\x1b[1;%dm", attr);
#endif
}

#define VPRINTF_ARGS(format) va_list args; va_start(args, format); vprintf(format, args); va_end(args);

void TraceN(const char* format, ...) 
{
	if (!quiet && verbose) {
		SetConsoleAttributes(Console::DARKGRAY);
		VPRINTF_ARGS(format);
		RestoreConsoleAttributes();
	}
}

void Trace(const char* format, ...) {
	if (!quiet && verbose) {
		SetConsoleAttributes(Console::DARKGRAY);
		VPRINTF_ARGS(format);
		printf("\n");
		RestoreConsoleAttributes();
	}
}
void InfoN(const char* format, ...) {
	if (!quiet) {
		SetConsoleAttributes(Console::WHITE);
		VPRINTF_ARGS(format);
		RestoreConsoleAttributes();
	}
}

void Info(const char* format, ...)
{
	if (!quiet) {
		SetConsoleAttributes(Console::WHITE);
		VPRINTF_ARGS(format);
		printf("\n");
		RestoreConsoleAttributes();
	}
}

void WarnN(const char* format, ...)
{
	if (!quiet) {
		SetConsoleAttributes(Console::YELLOW);
		printf("Warning: ");
		VPRINTF_ARGS(format);
		RestoreConsoleAttributes();
	}
}

void Warn(const char* format, ...) {
	if (!quiet) {
		SetConsoleAttributes(Console::YELLOW);
		printf("Warning: ");
		VPRINTF_ARGS(format);
		printf("\n");
		RestoreConsoleAttributes();
	}
}

void ErrorN(const char* format, ...) 
{
	errors++;
	if (!quiet) {
		SetConsoleAttributes(Console::RED);
		printf("Error: ");
		VPRINTF_ARGS(format);
		RestoreConsoleAttributes();
	}
}

void Error(const char* format, ...) 
{
	errors++;
	if (!quiet) {
		SetConsoleAttributes(Console::RED);
		printf("Error: ");
		VPRINTF_ARGS(format);
		printf("\n");
		RestoreConsoleAttributes();
	}
}

void FatalN(const char* format, ...)
{
	errors++;
	if (!quiet) {
		SetConsoleAttributes(Console::MAGENTA);
		printf("Fatal: ");
		VPRINTF_ARGS(format);
		RestoreConsoleAttributes();
	}
}

void Fatal(const char* format, ...)
{
	errors++;
	if (!quiet) {
		SetConsoleAttributes(Console::MAGENTA);
		printf("Fatal: ");
		VPRINTF_ARGS(format);
		printf("\n");
		RestoreConsoleAttributes();
	}
}

void PrintN(const char* format, ...)
{
	if (!quiet) {
		VPRINTF_ARGS(format);
	}
}

void Print(const char* format, ...)
{
	if (!quiet) {
		VPRINTF_ARGS(format);
		printf("\n");
	}
}

#ifdef WIN32
#include <windows.h>

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType)
	{
	case CTRL_C_EVENT:
		return TRUE;
	default:
		return FALSE;
	}
}

void AddConsoleCtrlHandler()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
}

#else

void AddConsoleCtrlHandler ()
{
}

#endif WIN32
