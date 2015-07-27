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
