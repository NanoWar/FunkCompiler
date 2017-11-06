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
	auto results = GetFunctionScope()->Results;
	if(results->Children.size() != Exprs->Children.size())
	{
		Error(this, "Return does not match function results");
		return;
	}
	for(int i = 0; i < results->Children.size(); i++)
	{
		auto reg = results->Children[i]->Register;
		auto expr = Exprs->Children[i];
		auto assign = new AssignStmt(SourceLoc, reg, expr);
		assign->Parent = this;
		assign->Compile();
	}
	WriteLn("\tret");
}

void FunctionDeclNode::Compile()
{
	Trace(this, "Compiling function <%s>", GetIdentifier().c_str());
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
	if(!(Statements->HasChildren() && dynamic_cast<ReturnNode*>(Statements->Children.back())))
	{
		WriteLn("\tret");
	}
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

	auto scope = GetFunctionScope();
	auto s_counter = scope->Counter++;
	auto s_name = scope->GetIdentifier();

	if (FalseStmts->HasChildren())
	{
		WriteLn("\tjp\tnz, %s.if%i_else", s_name.c_str(), s_counter);
		TrueStmts->Compile();
		WriteLn("\tjp\t%s.if%i_end", s_name.c_str(), s_counter);
		WriteLn("%s.if%i_else", s_name.c_str(), s_counter);
		FalseStmts->Compile();
		WriteLn("%s.if%i_end", s_name.c_str(), s_counter);
	}
	else
	{
		WriteLn("\tjp\tnz, %s.if%i_end", s_name.c_str(), s_counter);
		TrueStmts->Compile();
		WriteLn("%s.if%i_end", s_name.c_str(), s_counter);
	}
}

void CompareExpr::Compile()
{
	Compile_Setup();
	if (!HasStaticValue && Lhs->HasTargetRegister)
	{
		WriteLn("\tcp\t%s", Rhs->Target.c_str());
	}
}

void ParameterNode::Compile()
{
	if (HasType) {
		WriteLn(";  %s = %s [%s]", RSMx(Register), Name.c_str(), Type->GetName().c_str());
	}
	else {
		WriteLn(";  %s = %s", RSMx(Register), Name.c_str());
	}
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
	Trace(this, "Compiling module <%s>", GetIdentifier().c_str());
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
				ERegister reg = Lhs->Size == 1 ? ERegister::A : ERegister::HL;
				auto reg_s = RSMx(reg);
				WriteLn(S_LOAD, reg_s, Lhs->Target.c_str());
				if (Rhs->HasStaticValue && Rhs->Value == 1)
				{
					WriteLn("\tinc\t%s", reg_s);
				}
				else if (Rhs->HasStaticValue && Rhs->Value == 2)
				{
					WriteLn("\tinc\t%s", reg_s);
					WriteLn("\tinc\t%s", reg_s);
				}
				else
				{
					WriteLn(S_ADD, reg_s, Rhs->Target.c_str());
				}
				TargetRegister = reg;
				HasTargetRegister = true;
			}
			//WriteLn("\tpop\taf");
			// Copy
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
		Error(this, "Could not find function <%s>", Ident->GetName().c_str());
		WriteLn("\tcall\t%s", Ident->GetName().c_str());
		return;
	}

	auto decl_params = decl->Parameters;
	if (decl_params->Children.size() != Parameters->Children.size())
	{
		Error(this, "Parameter mismatch of function <%s>", decl->GetIdentifier().c_str());
		WriteLn("\tcall\t%s", decl->GetIdentifier().c_str());
		return;
	}

	// Parameters are compiled here (inside assign)
	for (unsigned int i = 0; i < Parameters->Children.size(); i++)
	{
		// TODO: Place assign statement during evaluation into SETUP_NODES
		auto assign = new AssignStmt(SourceLoc, decl_params->Children[i]->Register, Parameters->Children[i]);
		assign->Parent = this;
		assign->Evaluate();
		assign->Compile();
	}

	auto returns = decl->Results->Children;
	if(returns.size() == 1)
	{
		HasTargetRegister = true;
		TargetRegister = returns[0]->Register;
		Size = REG_SIZE(TargetRegister);
	}

	WriteLn("\tcall\t%s", decl->GetIdentifier().c_str());
}

bool FindDirectReplacement(ExpressionNode *node, string name)
{
	// Look up direct replacement
	auto direct = Definitions[name];
	if (!direct.empty())
	{
		Trace(node, "Found direct replacement <%s> => <%s>", name.c_str(), direct.c_str());
		node->Name = ""; // Name gets replaced
		node->Target = direct;
		return true;
	}
	return false;
}

bool FindReplacement(IdentExpr *node)
{
	// Look up replacement
	auto identifier = node->GetIdentifier();
	auto replacement = Definitions[identifier];
	if (!replacement.empty())
	{
		Trace(node, "Found replacement <%s> => <%s>", identifier.c_str(), replacement.c_str());
		node->Name = ""; // Name gets replaced
		node->Target = replacement;
		return true;
	}
	return false;
}

bool FindReferenced(IdentExpr *node, string name)
{
	auto ref = node->GetReferenced();
	if (ref)
	{
		if (auto parameter = dynamic_cast<ParameterNode *>(ref))
		{
			node->SetTargetRegister(parameter->Register);
		}
		else if( auto result = dynamic_cast<ResultNode *>(ref))
		{
			node->SetTargetRegister(result->Register);
		}
		else if( auto ident_reg = dynamic_cast<IdentRegExpr *>(ref))
		{
			node->SetTargetRegister(ident_reg->TargetRegister);
		}
		else
		{
			node->Target = name;
		}
		return true;
	}
	return false;
}

bool FindDefinition(ExpressionNode *node, string name)
{
	// Search in z80 include files
	if (auto def = DefinitionsHashed[name.c_str()])
	{
		Trace(node, "Found definition <%s> => <%s>", name.c_str(), def);
		int num = ParseNumber(def);
		if (num == -1) node->Size = 0;
		else node->Size = num > 255 ? 2 : 1;
		node->Target = name;
		return true;
	}
	return false;
}

bool FindStruct(IdentExpr *node)
{
	auto ref = node->GetReferenced(false);
	if (ref)
	{
		if (auto parameter = dynamic_cast<ParameterNode *>(ref))
		{
			if (parameter->HasType)
			{
				auto typeRef = parameter->Type->GetReferenced();
				if (typeRef)
				{
					if (auto structNode = dynamic_cast<StructNode *>(typeRef))
					{
						for (auto structDef : structNode->Definitions->Children)
						{
							auto defName = node->Children.back();
							if (strcmp(structDef->Name.c_str(), defName->c_str()) == 0)
							{
								stringstream ss;
								ss << "(ix + " << structDef->Offset << ")";
								node->Target = ss.str();
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void IdentExpr::Compile()
{
	auto name = GetName();
	if (FindDirectReplacement(this, name)) return;
	if (FindReplacement(this)) return;
	if (FindReferenced(this, name)) return;
	if (FindDefinition(this, name)) return;
	if (FindStruct(this)) return;

	Error(this, "Cannot resolve id <%s>", name.c_str());
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
				Error(this, "Incompatible operation <ld %s, %s>", RSMx(Lhs->TargetRegister), RSMx(Rhs->TargetRegister));
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
				Trace(this, "Found interesting expression <%s> of size <%d>", Rhs->Target.c_str(), Rhs->Size);
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
				else if (!Rhs->Target.empty())
				{
					WriteLoad(Lhs->TargetRegister, Rhs->Target);
				}
				else
				{
					// Reference unknown (earlier warning)
					WriteLoad(Lhs->TargetRegister, Rhs->Name);
				}
			}
			else
			{
				Error(this, "Cannot resolve <%s>", Rhs->Name);
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
	auto pool = GetRegisterPool();
	vector<tuple<ERegister, RegisterUsageInfo>> saved_registers;
	for (auto ident = SaveList->Idents.begin(); ident != SaveList->Idents.end(); ++ident)
	{
		ERegister saved_register = ERegister::NONE;
		auto ref = (*ident)->GetReferenced();
		if (auto assign = dynamic_cast<AssignStmt *>(ref))
		{
			if (assign->Rhs->HasTargetRegister)
			{
				saved_register = assign->Rhs->TargetRegister;
			}
			else
			{
				Error(this, "Cannot save <%s> because it has no attached register", (*ident)->GetName().c_str());
				continue;
			}
		}
		else if (auto parameter = dynamic_cast<ParameterNode *>(ref))
		{
			saved_register = parameter->Register;
		}
		else if (auto result = dynamic_cast<ResultNode *>(ref))
		{
			saved_register = result->Register;
		}
		else if (auto new_ident = dynamic_cast<IdentRegExpr *>(ref))
		{
			saved_register = new_ident->TargetRegister;
		}
		else
		{
			Error(this, "Cannot resolve reference <%s>", (*ident)->GetName().c_str());
			continue;
		}

		saved_registers.emplace_back(saved_register, pool->GetUsageInfo(saved_register));
	}

	// Add simple registers
	for (auto reg : SaveList->Registers) {
		saved_registers.emplace_back(reg, pool->GetUsageInfo(reg));
	}

	// Push everything
	for (auto reg : saved_registers) {
		WriteLn("\tpush\t%s", RSMx(get<0>(reg)));
		pool->SetUsage(this, get<0>(reg), ERegisterUsage::FREE);
	}

	// Compile inner statements
	Statements->Compile();

	// Pop everything
	for (auto reg : saved_registers) {
		WriteLn("\tpop\t%s", RSMx(get<0>(reg)));
		pool->SetUsageInfo(this, get<0>(reg), get<1>(reg));
	}
}