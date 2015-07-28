#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include "Writer.h"

void FunctionDeclNode::Compile()
{
	info("%d: Compiling Function %s\n", SourceLine, GetIdentifier().c_str());
	//write("\n;------------------------------\n; Function: %s\n;------------------------------\n", Name.c_str());
	write("%s\n", GetIdentifier().c_str());
	if (Parameters->HasChildren()) {
		write(";Inputs:\n");
		Parameters->Compile();
	}
	Statements->Compile();
}

void ParameterNode::Compile()
{
	write(";  %s (%s)\n", Name.c_str(), GetIdentifier().c_str());
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
	if (Lhs->HasStaticValue && Rhs->HasStaticValue) {
		HasStaticValue = true;
		Value = Lhs->Value + Rhs->Value;
		target << Value;
		Target = target.str();
		Size = max(Lhs->Size, Rhs->Size);
		return;
	}
	else {
		if (Lhs->Size == Rhs->Size) {
			//write("\tpush\taf\n");
			if (Lhs->HasTargetRegister) {
				write("\tadd\t%s, %s\n", RegisterStringMap[Lhs->TargetRegister].c_str(), Rhs->Target.c_str());
				TargetRegister = Lhs->TargetRegister;
				HasTargetRegister = true;
			}
			else {
				write(S_LOAD, RSM(A), Lhs->Target.c_str());
				write(S_ADD_A, Rhs->Target.c_str());
				TargetRegister = ERegister::A;
				HasTargetRegister = true;
			}
			//write("\tpop\taf\n");
			target << RegisterStringMap[TargetRegister];
			Target = target.str();
			Size = Lhs->Size;
			return;
		}
		else {
			target << Lhs->Target << " + " << Rhs->Target;
			Target = target.str();
			return;
		}
	}
}

void FunctionCallStmt::Compile()
{
	Ident->Compile();
	Parameters->Compile();
	write("\tcall\t%s\n", Ident->GetIdentifier().c_str());
}

void AssignStmt::Compile()
{
	Expr->Compile();
	if (HasTargetRegister)
	{
		if (Expr->HasTargetRegister) {
			WriteLoad(TargetRegister, Expr->TargetRegister);
		}
		else {
			WriteLoad(TargetRegister, Expr->Target);
		}
	}
	else {
		// TODO: Look up
		WriteDefine(Target, Expr->Target);
	}
}
