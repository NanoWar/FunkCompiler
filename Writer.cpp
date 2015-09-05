#include "Global.h"
#include "Register.h"

#include "Writer.h"

char *S_LOAD = "\tld\t%s, %s";
char *S_ADD_A = "\tadd\ta, %s";
char *S_DEFINE = "#define %s %s";


void Write(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(output_file, format, args);
	va_end(args);
}

void WriteLn(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(output_file, format, args);
	va_end(args);
	fprintf(output_file, "\n");
}

// ld reg, reg
void WriteLoad(ERegister target, ERegister source)
{
	if (target == source) {
		return;
	}
	if (IS_BIG(target) && IS_BIG(source)) {
		if (target == ERegister::IX || target == ERegister::IY
			|| source == ERegister::IX || source == ERegister::IY) {
			WriteLn("\tpush\t%s\n\tpop\t%s",
				RSMx(source),
				RSMx(target));
		}
		else {
			WriteLn(S_LOAD,
				RSMx(HI_REG(target)),
				RSMx(HI_REG(source)));
			WriteLn(S_LOAD,
				RSMx(LO_REG(target)),
				RSMx(LO_REG(source)));
		}
	}
	else {
		WriteLn(S_LOAD,
			RSMx(target),
			RSMx(source));
	}
}

// ld reg, name
void WriteLoad(ERegister target, string value)
{
	WriteLn(S_LOAD,
		RSMx(target),
		value.c_str());
}

// ld reg, num
void WriteLoad(ERegister target, int value)
{
	WriteLn(S_LOAD,
		RSMx(target),
		to_string(value).c_str());
}

// #define name name
void WriteDefine(string target, string value)
{
	WriteLn(S_DEFINE,
		target.c_str(),
		value.c_str());
}

// Register alias: #define name reg
void WriteDefine(string target, ERegister source)
{
	WriteLn(S_DEFINE,
		target.c_str(),
		RSMx(source));
}

void WriteProgStart()
{
	WriteLn("\t.org 4000h");
}
