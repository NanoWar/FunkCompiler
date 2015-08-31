#include "Global.h"
#include "AST.h"
#include "Parser.h"
#include "Console.h"
#include <typeinfo>

extern int compile(Node *n);

StatementsNode *Program;

char *tmp_file;

int verbose = 0;
int quiet = 0;
int tree = 0;
int repl = 0;
int errors = 0;
FILE *output_file = stdout;

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

		// -r REPL
		else if (strcmp(argv[i], "-r") == 0) repl = 1;

		// -x Execute
		else if (strcmp(argv[i], "-x") == 0) expect_input_string = 1;

		// Input file
		else if (expect_input_file) {
			expect_input_file = 0;
			yyin = fopen(argv[i], "r");
			if (!yyin) { cerr << "Error: Could not open input file <" << argv[i] << ">" << endl; return -1; }
		}

		// Output file
		else if (expect_output_file) {
			expect_output_file = 0;
			output_file = fopen(argv[i], "w+");
			if (!output_file) { cerr << "Error: Could not create output file <" << argv[i] << ">" << endl; return -1; }
		}
	}

	if (!quiet) {
		cout << "FunkCompiler by Robert Kuhfss" << endl;
		if (verbose) cout << "-> verbose mode" << endl;
		if (yydebug) cout << "-> debug mode" << endl;
	}

	if (argc == 1) {
		// Display usage
		cout << endl << "Usage: [options] <input file> <output file>" << endl;
		cout << "  -v          " << "Verbose mode (shows additional output)" << endl;
		cout << "  -q          " << "Quiet mode (supresses all console output)" << endl;
		cout << "  -d          " << "Debug mode (shows debug messages)" << endl;
		cout << "  -t          " << "Tree (shows parse tree)" << endl;
		cout << "  -r          " << "REPL mode" << endl;
		cout << "  -x <string> " << "Execute string" << endl;
		return 0;
	}

	if (repl) {
		print("Reading from console input ...\n");
		AddConsoleCtrlHandler();
	}
	else {
		print("Parsing input ...\n");
	}


	// Parse
	int yyparse_ret = yyparse();
	info("Parsing completed.\n");

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

	if (errors) print("There where errors.\n");
	else print("Success!\n");

	RestoreConsoleAttributes();
	return yyparse_ret;
}

void yyerror(char const *s) {
	errors++;
	error("%s in line %d\n", s, yylineno);
}
