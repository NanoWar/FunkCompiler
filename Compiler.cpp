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

int ObjectCounter = 0;
int NextObjectCounter() { return ObjectCounter++; }

void FunctionDeclNode::Compile()
{
	//write("\n;------------------------------\n; Function: %s\n;------------------------------\n", this->Name.c_str());
	write("%s\n", this->GetIdentifier().c_str());
	if(this->Parameters->HasChildren()) {
		write(";Inputs:\n");
		this->Parameters->Compile();
	}
	this->Statements->Compile();
}

void ParameterNode::Compile()
{
	write(";  reg = %s\n", this->Name.c_str());
}

void ModuleNode::Compile()
{
	write("\n;==============================\n; Module: %s\n;==============================\n\n", this->GetIdentifier().c_str());
	this->Statements->Compile();
}

void AssignNode::Compile()
{
	write("\n\tld\t%s, ", this->TargetRegister.c_str());
}

int compile (Node *n)
{
	return 0;
}
