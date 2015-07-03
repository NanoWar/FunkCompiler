#ifndef Register_h
#define Register_h

enum ERegister16
{
	AF, BC, DE, HL, IX, IY, IR
};

enum ERegister8
{
	A, F, B, C, D, E, H, L, IXH, IXL, IYH, IYL, I, R, MAX
};

struct map_icmp { 
    bool operator() (const std::string& lhs, const std::string& rhs) const {
        return stricmp(lhs.c_str(), rhs.c_str()) < 0;
    }
};

extern bool RegisterUsage[ERegister8::MAX];
extern map<string, ERegister8, map_icmp> reg8_map;
extern map<string, ERegister16, map_icmp> reg16_map;

map<string, ERegister8, map_icmp> mk_reg8_map();
map<string, ERegister16, map_icmp> mk_reg16_map();



bool IsRegisterUsed(ERegister8 reg8);
bool IsRegisterUsed(ERegister16 reg16);
bool UseRegister(ERegister8 reg8);
bool UseRegister(ERegister16 reg16);
bool UseRegister(const char *reg);
void UnUseRegister(ERegister8 reg8);
void UnUseRegister(ERegister16 reg16);
void UnUseRegister(const char *reg);
void UnUseAllRegisters();


#endif Register_h