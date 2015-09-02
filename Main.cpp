#include "Global.h"
#include "AST.h"
#include "Parser.h"
#include "Console.h"
#include <typeinfo>

extern int compile(Node *n);

StatementsNode *Program;

int verbose = 0;
int quiet = 0;
int tree = 0;
int repl = 0;
int no_colors = 0;

char *tmp_file;
FILE *output_file = stdout;
int errors = 0;

int main(int argc, char **argv)
{
	SaveConsoleAttributes();

	int expect_input_string = 0;
	int expect_input_file = 1;
	int expect_output_file = 1;

	for (int i = 1; i < argc; i++)
	{
		if (expect_input_string)
		{
			expect_input_string = 0;
			tmp_file = "__tmp";
			auto f = fopen(tmp_file, "w+");
			fwrite(argv[i], sizeof(char), strlen(argv[i]), f);
			fclose(f);
			yyin = fopen(tmp_file, "r");
		}

		// -v Verbose
		else if (strcmp(argv[i], "-v") == 0) verbose = 1;

		// -q Quiet
		else if (strcmp(argv[i], "-q") == 0) quiet = 1;

		// -d Debug
		else if (strcmp(argv[i], "-d") == 0) yydebug = 1;

		// -t Tree
		else if (strcmp(argv[i], "-t") == 0) tree = 1;

		// -a Interactive
		else if (strcmp(argv[i], "-a") == 0) repl = 1;

		// -x Execute
		else if (strcmp(argv[i], "-x") == 0) expect_input_string = 1;

		// -n No colors
		else if (strcmp(argv[i], "-n") == 0) no_colors = 1;

		// Input file
		else if (expect_input_file) {
			expect_input_file = 0;
			yyin = fopen(argv[i], "r");
			if (!yyin) { error("Could not open input file <%s>.\n", argv[i]); return -1; }
		}

		// Output file
		else if (expect_output_file) {
			expect_output_file = 0;
			output_file = fopen(argv[i], "w+");
			if (!output_file) { error("Could not create output file <%s>.\n", argv[i]); return -1; }
		}
	}
	
	SetConsoleAttributes(Console::WHITE);
	println("FunkCompiler by Robert Kuhfss");
	if (verbose) println("-> verbose mode");
	if (yydebug) println("-> debug mode");
	RestoreConsoleAttributes();

	if (argc == 1) {
		// Display usage
		println("\nUsage: [options] <input file> <output file>");
		println("  -v         Verbose mode (show additional output)");
		println("  -q         Quiet mode (supress all console output)");
		println("  -x <code>  Execute code directly");
		println("  -n         No colored console output");
		println("  -a         Interactive mode");
		println("  -d         Debug mode (show debug messages)");
		println("  -t         Show parse tree");
		return 0;
	}

	if (repl) {
		trace("Reading from console input ...\n");
		AddConsoleCtrlHandler();
	}
	else {
		trace("Parsing input ...\n");
	}


	// Parse
	int yyparse_ret = yyparse();
	trace("Parsing completed.\n");

	if (!yyparse_ret) {

		// Evaluate
		Program->Evaluate();

		// Compile
		Program->Compile();

		// Add strings
		CompileCurrentStrings();
	}


	// Safety flush
	fflush(output_file);

	// Clean up files
	fclose(yyin);
	fclose(output_file);
	if (tmp_file) {
		remove(tmp_file);
	}

	if (errors) {
		SetConsoleAttributes(Console::RED);
		print("There were errors.\n");
	}
	else {
		SetConsoleAttributes(Console::GREEN);
		print("Success!\n");
	}

	RestoreConsoleAttributes();
	return yyparse_ret;
}

void yyerror(char const *s) {
	errors++;
	error("%s in line %d\n", s, yylineno);
}
