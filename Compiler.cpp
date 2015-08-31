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

void IfStmt::Compile()
{
	Expr->Compile();
	if (Expr->HasStaticValue && Expr->Value > 0) {
		// Always true => skip else
	}
	else if (Expr->HasTargetRegister) {
		if (Expr->Size == 1) {
			// Assign to a
			WriteLoad(ERegister::A, Expr->TargetRegister);
			write("\tor\ta\n");
		}
		else if (Expr->Size == 2){
			// Assign to hl
			WriteLoad(ERegister::HL, Expr->TargetRegister);
			write("\tld\ta, h\n");
			write("\tor\tl\n");
		}
	}

	if (FalseStmts->HasChildren()) {
		write("\tjp\tnz, __if_else\n");
		TrueStmts->Compile();
		write("\tjp\t__if_end\n");
		write("__if_else\n");
		FalseStmts->Compile();
		write("__if_end\n");
	}
	else {
		write("\tjp\tnz, __if_end\n");
		TrueStmts->Compile();
		write("__if_end\n");
	}
}

void CompareExpr::Compile()
{
	if (Lhs->HasStaticValue && Rhs->HasStaticValue) {
		if (Lhs->Value == Rhs->Value) {
			warn("Comparison is always true.\n");
			HasStaticValue = true;
			Value = 1;
		}
		else {
			warn("Comparison is always false.\n");
			HasStaticValue = true;
			Value = 0;
		}
	}
	else {
		Lhs->Compile();
		if (Lhs->HasTargetRegister && Lhs->TargetRegister == ERegister::HL) {
			write("\tpush\thl\n");
		}
		Rhs->Compile();
		// TODO

		if (Lhs->HasTargetRegister) {
			// Assign LHS to a
			WriteLoad(ERegister::A, Lhs->TargetRegister);
			write("\tcp\t%s\n", Rhs->Target.c_str());
		}
	}
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

	// Find declaration
	auto decl = Ident->GetReferenced();
	if (dynamic_cast<FunctionDeclNode*>(decl)) {
		auto decl_params = ((FunctionDeclNode*)decl)->Parameters;
		if (decl_params->Children.size() != Parameters->Children.size()) {
			warn("Parameter mismatch of function %s.\n", decl->GetIdentifier().c_str());
		}
		else {
			for (unsigned int i = 0; i < Parameters->Children.size(); i++) {
				auto assign = new AssignStmt(decl_params->Children[i]->Register, Parameters->Children[i]);
				assign->Parent = this;
				assign->Compile();
			}
		}
		write("\tcall\t%s\n", decl->GetIdentifier().c_str());
	}
	else {
		write("\tcall\t%s\n", Ident->GetName().c_str());
	}
}

void IdentExpr::Compile()
{
	auto name = GetName();
	auto ref = GetReferenced();
	if (NULL == ref) {
		warn("Could not find id '%s'.\n", name.c_str());
		Target = name;
	}
	else {
		if (dynamic_cast<ParameterNode*>(ref)) {
			TargetRegister = ((ParameterNode*)ref)->Register;
			HasTargetRegister = true;
			//TODO: this should not be set:
			Target = RegisterStringMap[TargetRegister];
		}
		else {
			Target = name;
		}
	}
}

void AssignStmt::Compile()
{
	Lhs->Compile();
	Rhs->Compile();

	if (Lhs->HasTargetRegister) {
		if (Rhs->HasTargetRegister) {
			WriteLoad(Lhs->TargetRegister, Rhs->TargetRegister);
		}
		else {
			// Optimize "ld a, 0" => "xor a"
			if (Lhs->TargetRegister == ERegister::A && Rhs->Value == 0) {
				write("\txor\ta\n");
			} else {
				if(Rhs->Name.empty()) {
					WriteLoad(Lhs->TargetRegister, Rhs->Target);
				} else {
					WriteLoad(Lhs->TargetRegister, Rhs->GetIdentifier());
				}
			}
		}
	}
	else {
		WriteDefine(Lhs->GetIdentifier(), Rhs->Target);
	}
}
