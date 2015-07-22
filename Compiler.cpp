#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"

extern FILE *output_file;

void write (const char* format, ...) {
	va_list args;
	va_start (args, format);
	vfprintf (output_file, format, args);
	va_end (args);
}

void FunctionDeclNode::Compile()
{
	write("fn_%s\n", this->GetIdentifier());
}

void ModuleNode::Compile()
{
	write("mod_%s\n", this->GetIdentifier());
	this->Statements->Compile();
}

int compile (Node *n)
{
	return 0;
}
