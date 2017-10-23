#ifndef Register_h
#define Register_h

#include "Node.h"
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
#define REG_SIZE(reg) (IS_SMALL(reg) ? 1 : 2)

extern map<ERegister, string> RegisterStringMap;

#define RSM(reg) RegisterStringMap[ERegister::##reg].c_str()
#define RSMx(expr) RegisterStringMap[expr].c_str()

struct RegisterUsageInfo
{
	ERegisterUsage Usage;
	int Value;

	RegisterUsageInfo() : Usage(ERegisterUsage::FREE), Value(0) { }

	RegisterUsageInfo Combine(RegisterUsageInfo other, bool otherIsLow = true);
};

class RegisterUsage
{
private:
	map<ERegister, RegisterUsageInfo> RegisterUsageInfoMap;

public:
	ERegister AnyFIXED_Value(int value, bool isSmall);
	RegisterUsageInfo GetUsageInfo(ERegister reg);
	void SetUsageInfo(ERegister reg, RegisterUsageInfo info);
	void SetUsage(const Node* node, ERegister reg, ERegisterUsage usage, int value = 0);
};

#endif Register_h