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

int const indent_step = 2;

void RestoreConsoleAttributes() {
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), user_attributes);
#elif !defined(MACVER)
	printf("\x1b[0m");
#endif
}

void SaveConsoleAttributes() {
#ifdef WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbiScreenBufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbiScreenBufferInfo);
	user_attributes = csbiScreenBufferInfo.wAttributes;
#endif
}

void SetConsoleAttributes(unsigned short attr) {
	if(no_colors) return;
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)attr);
#elif !defined(MACVER)
	printf("\x1b[1;%dm", attr);
#endif
}

// Only in verbose mode
void trace(const char* format, ...) {
	if (!quiet && verbose) {
		SetConsoleAttributes(Console::DARKGRAY);
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		RestoreConsoleAttributes();
	}
}

void info(const char* format, ...) {
	if (!quiet) {
		SetConsoleAttributes(Console::WHITE);
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		RestoreConsoleAttributes();
	}
}

void warn(const char* format, ...) {
	if (!quiet) {
		SetConsoleAttributes(Console::YELLOW);
		printf("Warning: ");
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		RestoreConsoleAttributes();
	}
}

void error(const char* format, ...) {
	errors++;
	if (!quiet) {
		SetConsoleAttributes(Console::RED);
		printf("Error: ");
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		RestoreConsoleAttributes();
	}
}

void fatal(const char* format, ...) {
	errors++;
	if (!quiet) {
		SetConsoleAttributes(Console::MAGENTA);
		printf("Fatal: ");
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		RestoreConsoleAttributes();
	}
}

void print(const char* format, ...) {
	if (!quiet) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

void println(const char* format, ...) {
	if (!quiet) {
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

void print_indent(int depth) {
	while (depth) {
		if (depth-- % indent_step == 0) {
			print("|");
		}
		else {
			print(" ");
		}
	}
}

void print_node(Node *n, int depth) {
	int i = 0;
	print_indent(depth);
	print(n->GetIdentifier().c_str());
	print("\n");
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
