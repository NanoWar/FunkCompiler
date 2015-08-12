#include "Global.h"
#include "Register.h"

#include "Writer.h"

char *S_LOAD = "\tld\t%s, %s\n";
char *S_ADD_A = "\tadd\ta, %s\n";
char *S_DEFINE = "#define %s %s\n";


void write(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(output_file, format, args);
	va_end(args);
}

void WriteLoad(ERegister target, ERegister source)
{
	if (target == source) {
		return;
	}
	if (IS_BIG(target) && IS_BIG(source)) {
		if (target == ERegister::IX || target == ERegister::IY
			|| source == ERegister::IX || source == ERegister::IY) {
			write("\tpush\t%s\n\tpop\t%s\n",
				RSMx(source),
				RSMx(target));
		}
		else {
			write(S_LOAD,
				RSMx(HI_REG(target)),
				RSMx(HI_REG(source)));
			write(S_LOAD,
				RSMx(LO_REG(target)),
				RSMx(LO_REG(source)));
		}
	}
	else {
		write(S_LOAD,
			RSMx(target),
			RSMx(source));
	}
}

void WriteLoad(ERegister target, string value)
{
	write(S_LOAD,
		RSMx(target),
		value.c_str());
}

void WriteDefine(string target, string value)
{
	write(S_DEFINE,
		target.c_str(),
		value.c_str());
}

void WriteDefine(string target, ERegister source)
{
	write(S_DEFINE,
		target.c_str(),
		RSMx(source));
}
