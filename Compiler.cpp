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
	info("%d: Compiling function %s\n", SourceLine, GetIdentifier().c_str());
	//write("\n;------------------------------\n; Function: %s\n;------------------------------\n", Name.c_str());
	write("%s\n", GetIdentifier().c_str());
	if (Parameters->HasChildren()) {
		write(";Inputs:\n");
		Parameters->Compile();
	}
	Statements->Compile();
	write("\tret\n\n");
}

void ParameterNode::Compile()
{
	write(";  %s = %s\n", RegisterStringMap[Register].c_str(), Name.c_str());
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
	info("%d: Compiling module %s\n", SourceLine, GetIdentifier().c_str());
	write("\n;==============================\n; Module: %s\n;==============================\n\n", GetIdentifier().c_str());
	Statements->Compile();
}

void IdentExpr::Compile()
{
	auto id = GetIdentifier();
	auto node = StringToNode[id];
	if (dynamic_cast<ParameterNode*>(node))
	{
		TargetRegister = ((ParameterNode*)node)->Register;
		HasTargetRegister = true;
		//TODO: this should not be set:
		Target = RegisterStringMap[TargetRegister];
	}
	else {
		Target = Name;
	}
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
	Parameters->Compile();

	auto id = Ident->GetIdentifier();

	// Find declaration
	auto decl = StringToNode[id];
	if (dynamic_cast<FunctionDeclNode*>(decl)) {
		auto decl_params = ((FunctionDeclNode*)decl)->Parameters;
		if (decl_params->Children.size() != Parameters->Children.size()) {
			warn("Parameter mismatch.\n");
		}
		else {
			for (unsigned int i = 0; i < Parameters->Children.size(); i++) {
				auto assign = new AssignStmt(decl_params->Children[i]->Register, Parameters->Children[i]);
				assign->Compile();
			}
		}
	}
	write("\tcall\t%s\n", id.c_str());
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
