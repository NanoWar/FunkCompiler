#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"
#include "StringBuffer.h"
#include "Writer.h"
#include "Definitions.h"

void AsmNode::Compile()
{
	WriteLn(Text.c_str());
}

void FunctionDeclNode::Compile()
{
	Trace("Line %d: Compiling function <%s>", SourceLine, GetIdentifier().c_str());
	//WriteLn("\n;------------------------------\n; Function: %s\n;------------------------------", Name.c_str());
	WriteLn("\n%s", GetIdentifier().c_str());
	if (Parameters->HasChildren())
	{
		WriteLn(";Inputs:");
		Parameters->Compile();
	}
	Statements->Compile();
	WriteLn("\tret");
}

void IfStmt::Compile()
{
	Expr->Compile();
	if (Expr->HasStaticValue && Expr->Value > 0)
	{
		// Always true => skip else
	}
	else if (Expr->HasTargetRegister)
	{
		if (Expr->Size == 1)
		{
			// Assign to a
			WriteLoad(ERegister::A, Expr->TargetRegister);
			WriteLn("\tor\ta");
		}
		else if (Expr->Size == 2)
		{
			// Assign to hl
			WriteLoad(ERegister::HL, Expr->TargetRegister);
			WriteLn("\tld\ta, h");
			WriteLn("\tor\tl");
		}
	}

	if (FalseStmts->HasChildren())
	{
		WriteLn("\tjp\tnz, __if_else");
		TrueStmts->Compile();
		WriteLn("\tjp\t__if_end");
		WriteLn("__if_else");
		FalseStmts->Compile();
		WriteLn("__if_end");
	}
	else
	{
		WriteLn("\tjp\tnz, __if_end");
		TrueStmts->Compile();
		WriteLn("__if_end");
	}
}

void CompareExpr::Compile()
{
	if (Lhs->HasStaticValue && Rhs->HasStaticValue)
	{
		if (Lhs->Value == Rhs->Value)
		{
			Warn("Comparison is always true");
			HasStaticValue = true;
			Value = 1;
		}
		else
		{
			Warn("Comparison is always false");
			HasStaticValue = true;
			Value = 0;
		}
	}
	else
	{
		Lhs->Compile();
		if (Lhs->HasTargetRegister && Lhs->TargetRegister == ERegister::HL) {
			WriteLn("\tpush\thl");
		}
		Rhs->Compile();
		// TODO

		if (Lhs->HasTargetRegister)
		{
			// Assign LHS to a
			WriteLoad(ERegister::A, Lhs->TargetRegister);
			WriteLn("\tcp\t%s", Rhs->Target.c_str());
		}
	}
}

void ParameterNode::Compile()
{
	WriteLn(";  %s = %s", RegisterStringMap[Register].c_str(), Name.c_str());
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
	Trace("Line %d: Compiling module <%s>", SourceLine, GetIdentifier().c_str());
	WriteLn("\n;==============================\n; Module: %s\n;==============================", GetIdentifier().c_str());
	Statements->Compile();
}

void PlusExpr::Compile()
{
	Rhs->Compile();
	Lhs->Compile();
	stringstream target;
	if (Lhs->HasStaticValue && Rhs->HasStaticValue)
	{
		HasStaticValue = true;
		Value = Lhs->Value + Rhs->Value;
		target << Value;
		Target = target.str();
		Size = max(Lhs->Size, Rhs->Size);
		return;
	}
	else
	{
		if (Lhs->Size == Rhs->Size)
		{
			//WriteLn("\tpush\taf");
			if (Lhs->HasTargetRegister) {
				if (Rhs->HasStaticValue && Rhs->Value == 1) 
				{
					WriteLn("\tinc\t%s", RSMx(Lhs->TargetRegister));
				}
				else
				{
					WriteLn("\tadd\t%s, %s", RSMx(Lhs->TargetRegister), Rhs->Target.c_str());
				}
				TargetRegister = Lhs->TargetRegister;
				HasTargetRegister = true;
			}
			else 
			{
				WriteLn(S_LOAD, RSM(A), Lhs->Target.c_str());
				if (Rhs->HasStaticValue && Rhs->Value == 1) 
				{
					WriteLn("\tinc\ta");
				}
				else
				{
					WriteLn(S_ADD_A, Rhs->Target.c_str());
				}
				TargetRegister = ERegister::A;
				HasTargetRegister = true;
			}
			//WriteLn("\tpop\taf");
			target << RegisterStringMap[TargetRegister];
			Target = target.str();
			Size = Lhs->Size;
			return;
		}
		else
		{
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
	if (dynamic_cast<FunctionDeclNode*>(decl))
	{
		auto decl_params = ((FunctionDeclNode*)decl)->Parameters;
		if (decl_params->Children.size() != Parameters->Children.size())
		{
			Warn("Parameter mismatch of function <%s> in line %d", decl->GetIdentifier().c_str(), SourceLine);
		}
		else
		{
			for (unsigned int i = 0; i < Parameters->Children.size(); i++)
			{
				auto assign = new AssignStmt(decl_params->Children[i]->Register, Parameters->Children[i]);
				assign->Parent = this;
				assign->Compile();
			}
		}
		WriteLn("\tcall\t%s", decl->GetIdentifier().c_str());
	}
	else
	{
		WriteLn("\tcall\t%s", Ident->GetName().c_str());
	}
}

void IdentExpr::Compile()
{
	auto name = GetName();

	// Look up direct replacement
	auto direct = Definitions[name];
	if (!direct.empty())
	{
		Trace("Found direct replacement <%s> => <%s>", name.c_str(), direct.c_str());
		Name = ""; // Name gets replaced
		Target = direct;
		return;
	}

	// Look up replacement
	auto identifier = GetIdentifier();
	auto replacement = Definitions[identifier];
	if (!replacement.empty())
	{
		Trace("Found replacement <%s> => <%s>", identifier.c_str(), replacement.c_str());
		Name = ""; // Name gets replaced
		Target = replacement;
		return;
	}

	auto ref = GetReferenced();
	if (ref == NULL)
	{
		if (auto def = DefinitionsHashed[name.c_str()])
		{
			Trace("Found definition <%s> => <%s>", name.c_str(), def);
			int num = ParseNumber(def);
			if (num == -1) Size = 0;
			else Size = num > 255 ? 2 : 1;
		}
		else
		{
			Warn("Cannot resolve id <%s>", name.c_str());
		}
		Target = name;
	}
	else 
	{
		if (dynamic_cast<ParameterNode*>(ref))
		{
			TargetRegister = ((ParameterNode*)ref)->Register;
			HasTargetRegister = true;
			//TODO: this should not be set:
			Target = RegisterStringMap[TargetRegister];
		}
		else
		{
			Target = name;
		}
	}
}

void AssignStmt::Compile()
{
	Lhs->Compile();
	Rhs->Compile();

	if (Lhs->HasTargetRegister)
	{
		if (Rhs->HasTargetRegister)
		{
			if (Lhs->Size != Rhs->Size)
			{
				Warn("Incompatible operation <ld %s, %s>", RSMx(Lhs->TargetRegister), RSMx(Rhs->TargetRegister));
			}
			WriteLoad(Lhs->TargetRegister, Rhs->TargetRegister);
		}
		else if (Rhs->HasStaticValue)
		{
			if (Lhs->TargetRegister == ERegister::A && Rhs->Value == 0)
			{
				WriteLn("\txor\ta");
			}
			else
			{
				WriteLoad(Lhs->TargetRegister, Rhs->Value);
			}
		}
		else
		{
			if (Rhs->Name.empty())
			{
				// TODO: check for "ld de, (hl)"
				Trace("Interesting expression <%s> of size <%d>", Rhs->Target.c_str(), Rhs->Size);
				// Combined expression like indirection or replacement from definitions file
				WriteLoad(Lhs->TargetRegister, Rhs->Target);
			}
			else if (auto identExpr = dynamic_cast<IdentExpr*>(Rhs))
			{
				auto ref = identExpr->GetReferenced();
				if (ref)
				{
					WriteLoad(Lhs->TargetRegister, ref->GetIdentifier());
				}
				else
				{
					// Reference unknown (earlier warning)
					WriteLoad(Lhs->TargetRegister, Rhs->Name);
				}
			}
			else
			{
				Fatal("Cannot resolve <%s>", Rhs->Name);
			}
		}
	}
	else
	{
		Definitions[Lhs->GetIdentifier()] = Rhs->Target;
	}
}
