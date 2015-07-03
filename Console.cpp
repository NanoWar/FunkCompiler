#include "Global.h"

#include "Console.h"
#include "AST.h"

extern int quiet;
extern int verbose;

int const indent_step = 2;

void info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	if (!quiet && verbose) {
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
	if (n->n_elems == 0) {
		print ("%s\n", n->type);
	} else {
		print ("(%s\n", n->type);
		for (i = 0; i < n->n_elems; ++i) {
			print_node(n->elems[i], depth + indent_step);
		}
		print_indent(depth);
		print (")\n");
	}
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
