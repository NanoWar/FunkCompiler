#include "Global.h"

#include "Register.h"


bool RegisterUsage[ERegister8::MAX];

map<string, ERegister8, map_icmp> reg8_map = mk_reg8_map();
map<string, ERegister16, map_icmp> reg16_map = mk_reg16_map();


bool IsRegisterUsed(ERegister8 reg8)
{
	return RegisterUsage[reg8];
}

bool IsRegisterUsed(ERegister16 reg16)
{
	return RegisterUsage[reg16/2] && RegisterUsage[reg16/2+1];
}

bool UseRegister(ERegister8 reg8)
{
	if (IsRegisterUsed(reg8)) return false;
	return RegisterUsage[reg8] = true;
}

bool UseRegister(ERegister16 reg16)
{
	if (IsRegisterUsed(reg16)) return false;
	return RegisterUsage[reg16*2] = RegisterUsage[reg16*2+1] = true;
}

void UnUseRegister(ERegister8 reg8)
{
	RegisterUsage[reg8] = false;

}

void UnUseRegister(ERegister16 reg16)
{
	RegisterUsage[reg16*2] = RegisterUsage[reg16*2+1] = false;
}

void UnUseAllRegisters()
{
	for (int i = 0; i < ERegister8::MAX; ++i) RegisterUsage[i] = false;
}

map<string, ERegister16, map_icmp> mk_reg16_map()
{
	auto m = map<string, ERegister16, map_icmp>();
	m["AF"] = ERegister16::AF;
	m["BC"] = ERegister16::BC;
	m["DE"] = ERegister16::DE;
	m["HL"] = ERegister16::HL;
	m["IX"] = ERegister16::IX;
	m["IY"] = ERegister16::IY;
	m["IR"] = ERegister16::IR;
	return m;
};

map<string, ERegister8, map_icmp> mk_reg8_map()
{
	auto m = map<string, ERegister8, map_icmp>();
	m["A"] = ERegister8::A;
	m["F"] = ERegister8::F;
	m["B"] = ERegister8::B;
	m["C"] = ERegister8::C;
	m["D"] = ERegister8::D;
	m["E"] = ERegister8::E;
	m["H"] = ERegister8::H;
	m["L"] = ERegister8::L;
	m["IXH"] = ERegister8::IXH;
	m["IXL"] = ERegister8::IXL;
	m["IYH"] = ERegister8::IYH;
	m["IYL"] = ERegister8::IYL;
	m["I"] = ERegister8::I;
	m["R"] = ERegister8::R;
	return m;
};

bool UseRegister(const char *reg)
{
	if (reg8_map.end() != reg8_map.find(reg)) {
		return UseRegister(reg8_map[reg]);
	}
	else if (reg16_map.end() != reg16_map.find(reg)) {
		return UseRegister(reg16_map[reg]);
	}
}

void UnUseRegister(const char *reg)
{
	if (reg8_map.end() != reg8_map.find(reg)) {
		UnUseRegister(reg8_map[reg]);
	}
	else if (reg16_map.end() != reg16_map.find(reg)) {
		UnUseRegister(reg16_map[reg]);
	}
}
