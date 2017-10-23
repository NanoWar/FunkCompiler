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
	if (Usage == ERegisterUsage::FREE && other.Usage == ERegisterUsage::FREE)
	{
		result.Usage = ERegisterUsage::FREE;
	}
	else if (Usage == ERegisterUsage::FIXED_ && other.Usage == ERegisterUsage::FIXED_)
	{
		result.Usage = ERegisterUsage::FIXED_;
		result.Value = otherIsLow ? other.Value + Value * 256 : Value + other.Value * 256;
	}
	else
	{
		result.Usage = ERegisterUsage::USED;
	}
	return result;
}


ERegister RegisterUsage::AnyFIXED_Value(int value, bool isSmall)
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
		if (info.Usage == ERegisterUsage::FIXED_ && info.Value == value)
		{
			return reg;
		}
	}
	return ERegister::NONE;
}

RegisterUsageInfo RegisterUsage::GetUsageInfo(ERegister reg)
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

void RegisterUsage::SetUsageInfo(ERegister reg, RegisterUsageInfo info)
{
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
		RegisterUsageInfoMap[HI_REG(reg)] = hi;
		RegisterUsageInfoMap[LO_REG(reg)] = lo;
	}
}

void RegisterUsage::SetUsage(const Node* node, ERegister reg, ERegisterUsage usage, int value)
{
	if (GetUsageInfo(reg).Usage != ERegisterUsage::FREE)
	{
		Warn(node, "Register <%s> is already in use", RSMx(reg));
	}
	RegisterUsageInfo info;
	info.Usage = usage;
	info.Value = value;
	SetUsageInfo(reg, info);
}