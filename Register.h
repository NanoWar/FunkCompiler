#ifndef Register_h
#define Register_h

enum ERegister
{
	NONE,
	A, F, B, C, D, E, H, L, IXH, IXL, IYH, IYL, I, R,
	AF, BC, DE, HL, IX, IY, IR,
	AFS,
	MAX
};

#define REGISTER_BIG ERegister::AF
#define SPLIT_REGISTER(reg, index) ((ERegister) ((reg-REGISTER_BIG)*2+index+1))
#define HI_REG(reg) SPLIT_REGISTER(reg, 0)
#define LO_REG(reg) SPLIT_REGISTER(reg, 1)
#define IS_SMALL(reg) (reg > ERegister::NONE && reg < REGISTER_BIG)
#define IS_BIG(reg) !IS_SMALL(reg)

extern map<ERegister, string> RegisterStringMap;

#endif Register_h