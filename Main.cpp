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
int errors = 0;
FILE *output_file = stdout;

int main(int argc, char **argv)
{
	int expect_input_string = 0;
	int expect_input_file = 0;
	int expect_output_file = 0;

	for(int i = 1; i < argc; i++) {
		if (expect_input_string) {
			expect_input_string = 0;
			tmp_file = "_tmp";
			auto f = fopen(tmp_file, "w+");
			fwrite(argv[i], sizeof(char), strlen(argv[i]), f);
			fclose(f);
			yyin = fopen(tmp_file, "r");
		}

		if (expect_input_file) {
			expect_input_file = 0;
			yyin = fopen(argv[i], "r");
			if(!yyin) { cerr << "Error: Could not open input file <" << argv[i] << ">" << endl; return -1; }
		}

		if (expect_output_file) {
			expect_output_file = 0;
			output_file = fopen(argv[i], "w+");
			if(!output_file) { cerr << "Error: Could not create output file <" << argv[i] << ">" << endl; return -1; }
		}

		// -v Verbose
		if(strcmp(argv[i], "-v") == 0) verbose = 1;

		// -q Quiet
		if(strcmp(argv[i], "-q") == 0) quiet = 1;

		// -d Debug
		if(strcmp(argv[i], "-d") == 0) yydebug = 1;

		// -t Tree
		if(strcmp(argv[i], "-t") == 0) tree = 1;

		// -x Execute
		if(strcmp(argv[i], "-x") == 0) expect_input_string = 1;

		// -i Input file
		if(strcmp(argv[i], "-i") == 0) expect_input_file = 1;

		// -o Output file
		if(strcmp(argv[i], "-o") == 0) expect_output_file = 1;

	}

	if(!quiet) {
		cout << "FunkCompiler by Robert Kuhfss" << endl;
		if(verbose) cout << "-> verbose mode" << endl;
		if(yydebug) cout << "-> debug mode" << endl;
	}

	if(argc == 1) {
		// Display usage
		cout << endl << "Usage:" << endl;
		cout << "  -v          " << "Verbose mode (shows additional output)" << endl;
		cout << "  -q          " << "Quiet mode (supresses all console output)" << endl;
		cout << "  -d          " << "Debug mode (shows debug messages)" << endl;
		cout << "  -t          " << "Tree (shows parse tree)" << endl;
		cout << "  -x <string> " << "Execute string" << endl;
		cout << "  -i <file>   " << "Input file" << endl;
		cout << "  -o <file>   " << "Output file" << endl;
		return 0;
	}


	if(yyin == stdin) {
		print("Reading from console input...");
		AddConsoleCtrlHandler();
	}
	else {
		print("Parsing input ...\n");
	}


	// Parse
	int yyparse_ret = yyparse();
	info("--- PARSE COMPLETE: ret:%d ---\n", yyparse_ret);

	// Evaluate
	Program->Evaluate();
	
	// Compile
	Program->Compile();

	// Add strings
	CompileCurrentStrings();



	// Safety flush
	fflush(output_file);

	// Clean up files
	fclose(yyin);
	fclose(output_file);
	if(tmp_file) {
		remove(tmp_file);
	}

	if(errors) print("There where errors\n");
	else print("Success!\n");

	return yyparse_ret;
}

void yyerror(char const *s) {
	errors++;
	if(!quiet) fprintf(stderr, "%s in line %d\n", s, yylineno);
}
