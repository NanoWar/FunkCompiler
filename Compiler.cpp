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
	fputs(Text.c_str(), output_file);
	fputs("\n", output_file);
}

void ReturnNode::Compile()
{
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
	if (Results->HasChildren())
	{
		WriteLn(";Outputs:");
		Results->Compile();
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
	WriteLn(";  %s = %s", RSMx(Register), Name.c_str());
}

void ResultNode::Compile()
{
	if(Name.empty())
	{
		WriteLn(";  %s", RSMx(Register));
	}
	else
	{
		WriteLn(";  %s = %s", RSMx(Register), Name.c_str());
	}
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

void FunctionCallExpr::Compile()
{
	Ident->Compile();

	// Find declaration
	auto decl = dynamic_cast<FunctionDeclNode*>(Ident->GetReferenced());
	if (!decl)
	{
		// Fall back
		WriteLn("\tcall\t%s", Ident->GetName().c_str());
		return;
	}

	auto decl_params = decl->Parameters;
	if (decl_params->Children.size() != Parameters->Children.size())
	{
		Warn("Parameter mismatch of function <%s> in line %d", decl->GetIdentifier().c_str(), SourceLine);
	}
	else
	{
		// Parameters are compiled here (inside assign)
		for (unsigned int i = 0; i < Parameters->Children.size(); i++)
		{
			auto assign = new AssignStmt(decl_params->Children[i]->Register, Parameters->Children[i]);
			assign->Parent = this;
			assign->Compile();
		}

		auto returns = decl->Results->Children;
		if(returns.size() == 1)
		{
			HasTargetRegister = true;
			TargetRegister = returns[0]->Register;
			Size = REG_SIZE(TargetRegister);
		}
	}
	WriteLn("\tcall\t%s", decl->GetIdentifier().c_str());
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
		if (auto parameter = dynamic_cast<ParameterNode *>(ref))
		{
			SetTargetRegister(parameter->Register);
		}
		else if( auto result = dynamic_cast<ResultNode *>(ref))
		{
			SetTargetRegister(result->Register);
		}
		else if( auto ident_reg = dynamic_cast<IdentRegExpr *>(ref))
		{
			SetTargetRegister(ident_reg->TargetRegister);
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

void SaveStmt::Compile()
{
	vector<ERegister> saved_registers;
	for (auto ident = SaveList->Idents.begin(); ident != SaveList->Idents.end(); ++ident)
	{
		auto ref = (*ident)->GetReferenced();
		if (auto assign = dynamic_cast<AssignStmt *>(ref))
		{
			if (assign->Rhs->HasTargetRegister)
			{
				saved_registers.push_back(assign->Rhs->TargetRegister);
				WriteLn("\tpush\t%s", RSMx(assign->Rhs->TargetRegister));
			}
			else
			{
				Error("Cannot save <%s> because it has no attached register", (*ident)->GetName().c_str());
			}
		}
		else if (auto parameter = dynamic_cast<ParameterNode *>(ref))
		{
			saved_registers.push_back(parameter->Register);
			WriteLn("\tpush\t%s", RSMx(parameter->Register));
		}
		else if (auto result = dynamic_cast<ResultNode *>(ref))
		{
			saved_registers.push_back(result->Register);
			WriteLn("\tpush\t%s", RSMx(result->Register));
		}
		else if (auto new_ident = dynamic_cast<IdentRegExpr *>(ref))
		{
			saved_registers.push_back(new_ident->TargetRegister);
			WriteLn("\tpush\t%s", RSMx(new_ident->TargetRegister));
		}
		else
		{
			Warn("Cannot resolve reference <%s>", (*ident)->GetName().c_str());
		}
	}
	for (auto reg = SaveList->Registers.begin(); reg != SaveList->Registers.end(); ++reg)
	{
		saved_registers.push_back(*reg);
		WriteLn("\tpush\t%s", RSMx(*reg));
	}

	//TODO: Open new register scope

	// Compile inner statements
	Statements->Compile();

	for (auto reg = saved_registers.crbegin(); reg != saved_registers.crend(); ++reg)
	{
		WriteLn("\tpop\t%s", RSMx(*reg));
	}
}
