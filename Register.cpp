#include "Global.h"

#include "Register.h"

map<ERegister, string> mk_reg_map()
{
	auto m = map<ERegister, string>();
	m[ERegister::A] = "a";
	m[ERegister::F] = "f";
	m[ERegister::B] = "b";
	m[ERegister::C] = "c";
	m[ERegister::D] = "d";
	m[ERegister::E] = "e";
	m[ERegister::H] = "h";
	m[ERegister::L] = "l";
	m[ERegister::I] = "i";
	m[ERegister::R] = "r";
	m[ERegister::AF] = "af";
	m[ERegister::BC] = "bc";
	m[ERegister::DE] = "de";
	m[ERegister::HL] = "hl";
	m[ERegister::IX] = "ix";
	m[ERegister::IY] = "iy";
	m[ERegister::AFS] = "af'";
	return m;
};

map<ERegister, string> RegisterStringMap = mk_reg_map();


RegisterUsageInfo RegisterUsageInfo::Combine(RegisterUsageInfo other, bool otherIsLow)
{
	RegisterUsageInfo result;

	// Calculate usage
	if (Usage == ERegisterUsage::FREE && other.Usage == ERegisterUsage::FREE)
	{
		result.Usage = ERegisterUsage::FREE;
	}
	else if (Usage == ERegisterUsage::FIX && other.Usage == ERegisterUsage::FIX)
	{
		result.Usage = ERegisterUsage::FIX;
		result.Value = otherIsLow ? other.Value + Value * 256 : Value + other.Value * 256;
	}
	else
	{
		result.Usage = ERegisterUsage::USED;
	}

	// Calculate mutability
	result.IsMutable = IsMutable && other.IsMutable;

	return result;
}


ERegister RegisterPool::AnyFixValue(int value, bool isSmall)
{
	ERegister start, end;
	if (isSmall)
	{
		start = REGISTER_SMALL;
		end = REGISTER_BIG;
	}
	else
	{
		start = REGISTER_BIG;
		end = ERegister::MAX;
	}

	for (int i = start; i < end; i++)
	{
		auto reg = (ERegister)i;
		auto info = RegisterUsageInfoMap[reg];
		if (info.Usage == ERegisterUsage::FIX && info.Value == value)
		{
			return reg;
		}
	}
	return ERegister::NONE;
}

RegisterUsageInfo RegisterPool::GetUsageInfo(ERegister reg)
{
	if (IS_SMALL(reg))
	{
		return RegisterUsageInfoMap[reg];
	}
	else
	{
		return RegisterUsageInfoMap[HI_REG(reg)].Combine(RegisterUsageInfoMap[LO_REG(reg)]);
	}
}

void RegisterPool::SetUsage(const Node* node, ERegister reg, ERegisterUsage usage, int value)
{
	auto info = GetUsageInfo(reg);
	if (usage != ERegisterUsage::FREE && info.Usage != ERegisterUsage::FREE)
	{
		Warn(node, "Register <%s> is already in use", RSMx(reg));
	}
	info.Usage = usage;
	info.Value = value;
	SetUsageInfo(node, reg, info);
}

void RegisterPool::SetUsageInfo(const Node *node, ERegister reg, RegisterUsageInfo info)
{
	if (reg == ERegister::NONE)
	{
		Fatal(node, "Register was not read properly");
	}

	if (IS_SMALL(reg))
	{
		RegisterUsageInfoMap[reg] = info;
	}
	else
	{
		auto hi = RegisterUsageInfoMap[HI_REG(reg)];
		auto lo = RegisterUsageInfoMap[LO_REG(reg)];
		hi.Usage = info.Usage;
		lo.Usage = info.Usage;
		hi.Value = info.Value / 256;
		lo.Value = info.Value % 256;
		hi.IsMutable = info.IsMutable;
		lo.IsMutable = info.IsMutable;
		RegisterUsageInfoMap[HI_REG(reg)] = hi;
		RegisterUsageInfoMap[LO_REG(reg)] = lo;
	}
}

void RegisterPool::SetMutability(const Node* node, ERegister reg, bool isMutable)
{
	auto info = GetUsageInfo(reg);

	info.IsMutable = isMutable;

	SetUsageInfo(node, reg, info);
}