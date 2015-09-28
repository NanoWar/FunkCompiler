#ifndef Register_h
#define Register_h

#include "Console.h"

enum ERegister
{
	NONE,
	A, F, B, C, D, E, H, L, IXH, IXL, IYH, IYL, I, R,
	AF, BC, DE, HL, IX, IY, IR,
	MAX,
	AFS
};

enum ERegisterUsage
{
	FREE,
	FIXED_,
	USED
};

#define REGISTER_SMALL ERegister::A
#define REGISTER_BIG ERegister::AF
#define SPLIT_REGISTER(reg, index) ((ERegister) ((reg-REGISTER_BIG)*2+index+1))
#define HI_REG(reg) SPLIT_REGISTER(reg, 0)
#define LO_REG(reg) SPLIT_REGISTER(reg, 1)
#define IS_SMALL(reg) (reg > ERegister::NONE && reg < REGISTER_BIG)
#define IS_BIG(reg) (reg >= REGISTER_BIG && reg < ERegister::MAX)

extern map<ERegister, string> RegisterStringMap;

#define RSM(reg) RegisterStringMap[ERegister::##reg].c_str()
#define RSMx(expr) RegisterStringMap[expr].c_str()

struct RegisterUsageInfo
{
	ERegisterUsage Usage;
	int Value;

	RegisterUsageInfo() : Usage(ERegisterUsage::FREE), Value(0) { }

	RegisterUsageInfo Combine(RegisterUsageInfo other, bool otherIsLow = true)
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
};

class RegisterUsage
{
private:
	map<ERegister, RegisterUsageInfo> RegisterUsageInfoMap;

public:

	ERegister AnyFIXED_Value(int value, bool isSmall)
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

	RegisterUsageInfo GetUsageInfo(ERegister reg)
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

	void SetUsageInfo(ERegister reg, RegisterUsageInfo info)
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

	void SetUsage(ERegister reg, ERegisterUsage usage, int value = 0)
	{
		if (GetUsageInfo(reg).Usage != ERegisterUsage::FREE)
		{
			Warn("Register <%s> is already in use", RSMx(reg));
		}
		RegisterUsageInfo info;
		info.Usage = usage;
		info.Value = value;
		SetUsageInfo(reg, info);
	}
};

#endif Register_h