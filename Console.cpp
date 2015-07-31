#include "Global.h"

#include "Console.h"
#include "AST.h"

extern int quiet;
extern int verbose;

int const indent_step = 2;

#ifdef WIN32
#include <WinBase.h>
//saved console attributes, to be restored on exit
WORD user_attributes;
#endif

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
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)attr);
#elif !defined(MACVER)
	printf("\x1b[1;%dm", attr);
#endif
}




void info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	if (!quiet && verbose) {
#ifdef WIN32
		HANDLE hConsole = GetStdHandle(stdout);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		vprintf(format, args);
	}
	va_end(args);
}

void print(const char* format, ...) {
	va_list args;
	va_start(args, format);
	if (!quiet) {
		vprintf(format, args);
	}
	va_end(args);
}

void print_indent(int depth) {
	while (depth) {
		if (depth-- % indent_step == 0) {
			print ("|");
		} else {
			print (" ");
		}
	}
}

void print_node(Node *n, int depth) {
	int i = 0;
	print_indent(depth);
	print(n->GetIdentifier().c_str());
	print ("\n");
}


#ifdef WIN32
#include <windows.h>

BOOL CtrlHandler (DWORD fdwCtrlType) 
{
	switch( fdwCtrlType ) 
	{
	case CTRL_C_EVENT:
		//cout << "Ctrl-C event" << endl;
		return TRUE;

	/*case CTRL_CLOSE_EVENT:
		cout << "Ctrl-Close event" << endl;
		return TRUE;
	*/
	}
	return FALSE;
}

void AddConsoleCtrlHandler ()
{
	SetConsoleCtrlHandler ((PHANDLER_ROUTINE) CtrlHandler, TRUE);
}

#else

void AddConsoleCtrlHandler ()
{
}

#endif WIN32
