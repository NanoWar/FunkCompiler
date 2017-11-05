#include "Global.h"
#include <typeinfo>
#include <chrono>

#include "AST.h"
#include "Parser.h"
#include "Console.h"
#include "Writer.h"
#include "Definitions.h"
#include "StringHelper.h"

extern int compile(Node *n);

StatementsNode *Program;
char *tmp_file;

int main(int argc, char **argv)
{
	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
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
			input_file = yyin;
		}

		// -v Verbose
		else if (strcmp(argv[i], "-v") == 0) verbose = 1;

		// -q Quiet
		else if (strcmp(argv[i], "-q") == 0) quiet = 1;

		// -d Debug
		else if (strcmp(argv[i], "-d") == 0) yydebug = 1;

		// -a Interactive
		else if (strcmp(argv[i], "-a") == 0) repl = 1;

		// -x Execute
		else if (strcmp(argv[i], "-x") == 0) expect_input_string = 1;

		// -n No colors
		else if (strcmp(argv[i], "-n") == 0) no_colors = 1;

		// Input file
		else if (expect_input_file) {
			expect_input_file = 0;
			char *file_path = argv[i];
			yyin = fopen(file_path, "r");
			if (!yyin) { Error("Could not open input file <%s>", argv[i]); return -1; }

			char *file_name = SkipFolders(file_path);
			if (file_path != file_name) {
				*(file_name-1) = 0; // Cut
				input_file_folder.assign(file_path).append("\\");
			}
			input_file_name_full.assign(file_name);

			char *file_ext = SkipFileName(file_name);
			if (file_name != file_ext) *(file_ext-1) = 0; // Cut
			input_file_name.assign(file_name);
			input_file_ext.assign(file_ext);
			input_file = yyin;
		}

		// Output file
		else if (expect_output_file) {
			expect_output_file = 0;
			output_file = fopen(argv[i], "w+");
			if (!output_file) { Error("Could not create output file <%s>", argv[i]); return -1; }
		}
	}

	SetConsoleAttributes(Console::WHITE);
	Print("Funk Z80 ASM Transpiler 1.0");
	if (verbose) Print("-> verbose mode");
	if (yydebug) Print("-> debug mode");
	RestoreConsoleAttributes();

	if (argc == 1) {
		// Display usage
		Print("\nUsage:\n  [options] <input file> <output file>\n");
		Print("  -v         Verbose mode (show additional output)");
		Print("  -q         Quiet mode (supress all console output)");
		Print("  -x <code>  Execute code from command line");
		Print("  -n         No colored console output");
		Print("  -a         Interactive mode");
		Print("  -d         Debug mode (show debug messages)");
		Print("\nby Robert Kuhfss");
		return 0;
	}


	if (repl)
	{
		Trace("Reading from console input ...");
		AddConsoleCtrlHandler();
	}
	else
	{
		if (expect_input_string) { Error("No code specified"); return -1; }
		if (!input_file) { Error("No input file specified"); return -1; }
		if (input_file_name.empty())
		{
			// -x mode
			output_file = stdout;
		}
		else
		{
			// Interfere output file name from input file name
			if (!output_file)
			{
				Trace("Interfering output file name");
				string output_file_name(input_file_name);
				output_file_name.append(".asm");
				output_file = fopen(output_file_name.c_str(), "w+");
				if (!output_file) { Error("Could not create output file <%s>", output_file_name.c_str()); return -1; }
			}
			Print("Processing <%s>", input_file_name.c_str());
		}
		Print("Parsing ...");
	}


	// Parse
	int yyparse_ret = yyparse(input_file_name_full);

	if (!yyparse_ret)
	{
		// Evaluate
		Print("Analyzing ...");
		Program->Evaluate();

		// Compile
		Print("Emitting ...");
		WriteProgStart();
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
	for(auto it = buffers.begin(); it != buffers.end(); ++it) delete[] *it;
	
	Print("It took %.2f seconds", std::chrono::duration_cast<chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() / 1000.0);
	if (errors)
	{
		SetConsoleAttributes(Console::RED);
		Print("There were %d errors!", errors);
	}
	else
	{
		SetConsoleAttributes(Console::GREEN);
		Print("Success!");
	}
	RestoreConsoleAttributes();

	return yyparse_ret;
}

void yyerror(string file_name, char const *error)
{
	errors++;
	if (!quiet)
	{
		printf("%4d %s: ", yylineno, yylloc.file_name.c_str());
		Error(error);
	}
}
