#ifndef Register_h
#define Register_h

enum ERegister
{
	NONE,
	A, F, B, C, D, E, H, L, IXH, IXL, IYH, IYL, I, R,
	AF, BC, DE, HL, IX, IY, IR,
	MAX
};

enum ERegisterUsage
{
	FREE,
	FIXED,
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
	ERegisterUsage Usage = ERegisterUsage::FREE;
	int Value = 0;

	RegisterUsageInfo Combine(RegisterUsageInfo other, bool otherIsLow = true)
	{
		RegisterUsageInfo result;
		if (Usage == ERegisterUsage::FREE && other.Usage == ERegisterUsage::FREE)
		{
			result.Usage = ERegisterUsage::FREE;
		}
		else if (Usage == ERegisterUsage::FIXED && other.Usage == ERegisterUsage::FIXED)
		{
			result.Usage = ERegisterUsage::FIXED;
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

	ERegister AnyFixedValue(int value, bool isSmall)
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
			if (info.Usage == ERegisterUsage::FIXED && info.Value == value)
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
};

#endif Register_h