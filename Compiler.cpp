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
	info("%d: Compiling Function %s\n", SourceLine, GetIdentifier().c_str());
	//write("\n;------------------------------\n; Function: %s\n;------------------------------\n", Name.c_str());
	write("%s\n", GetIdentifier().c_str());
	if(Parameters->HasChildren()) {
		write(";Inputs:\n");
		Parameters->Compile();
	}
	Statements->Compile();
}

void ParameterNode::Compile()
{
	write(";  reg = %s\n", GetIdentifier().c_str());
}

void IndirectionExpr::Compile()
{
	Expr->Compile();
	stringstream target;
	target << "(" << Expr->Target << ")";
	Target = target.str();
};

void ModuleNode::Compile()
{
	info("%d: Compiling Module %s\n", SourceLine, GetIdentifier().c_str());
	write("\n;==============================\n; Module: %s\n;==============================\n\n", GetIdentifier().c_str());
	Statements->Compile();
}

void IdentExpr::Compile()
{
	// TODO: Look up
	Target = Ident;
}

void PlusExpr::Compile()
{
	Rhs->Compile();
	Lhs->Compile();
	stringstream target;
	if(Lhs->HasStaticValue && Rhs->HasStaticValue) {
		HasStaticValue = true;
		Value = Lhs->Value + Rhs->Value;
		target << Value;
	} else {
		// TODO: Add logic
		target << Lhs->Target << " + " << Rhs->Target;
	}
	Target = target.str();
}

void AssignStmt::Compile()
{
	Expr->Compile();
	if(Expr->HasTargetRegister) {
		if(Expr->TargetRegister == TargetRegister) return;
		if(IS_BIG(Expr->TargetRegister) && IS_BIG(TargetRegister)) {
			write("\tld\t%s, %s\n",
				RegisterStringMap[HI_REG(TargetRegister)].c_str(),
				RegisterStringMap[HI_REG(Expr->TargetRegister)].c_str());
			write("\tld\t%s, %s\n",
				RegisterStringMap[LO_REG(TargetRegister)].c_str(),
				RegisterStringMap[LO_REG(Expr->TargetRegister)].c_str());
		}
		else {
			write("\tld\t%s, %s\n", Target.c_str(), Expr->Target.c_str());
		}
	}
	else {
		write("\tld\t%s, %s\n", Target.c_str(), Expr->Target.c_str());
	}
}

int compile (Node *n)
{
	return 0;
}
