#include "Global.h"
#include "AST.h"
#include "Parser.h"
#include "Console.h"

extern int compile (Node *n);
extern void export_data();

extern Node *nodes;
extern int n_nodes;

extern unordered_map<Node *, const char*> nodes_map;
extern unordered_map<string, Node *> identifier_map;
char *tmp_file;

int verbose = 0;
int quiet = 0;
int tree = 0;
int errors = 0;
FILE *output_file = stdout;

int main (int argc, char **argv) {
	int expect_input = 0;
	int expect_input_file = 0;
	int expect_output_file = 0;

	for (int i = 1; i < argc; i++)
	{
		if (expect_input)
		{
			expect_input = 0;
			tmp_file = "_tmp";
			auto f = fopen (tmp_file, "w+");
			fwrite(argv[i], sizeof(char), strlen(argv[i]), f);
			fclose(f);
			yyin = fopen (tmp_file, "r");
		}

		if (expect_input_file)
		{
			expect_input_file = 0;
			yyin = fopen (argv[i], "r");
			if (!yyin) { cerr << "Error: Could not open input file <" << argv[i] << ">" << endl; return -1; }
		}

		if (expect_output_file)
		{
			expect_output_file = 0;
			output_file = fopen (argv[i], "w+");
			if (!output_file) { cerr << "Error: Could not create output file <" << argv[i] << ">" << endl; return -1; }
		}

		// -v Verbose
		if (strcmp (argv[i], "-v") == 0) verbose = 1;

		// -q Quiet
		if (strcmp (argv[i], "-q") == 0) quiet = 1;

		// -d Debug
		if (strcmp (argv[i], "-d") == 0) yydebug = 1;

		// -t Tree
		if (strcmp (argv[i], "-t") == 0) tree = 1;

		// -x Execute
		if (strcmp (argv[i], "-x") == 0) expect_input = 1;

		// -i Input file
		if (strcmp (argv[i], "-i") == 0) expect_input_file = 1;

		// -o Output file
		if (strcmp (argv[i], "-o") == 0) expect_output_file = 1;

	}

	if (!quiet) {
		cout << "FunkCompiler by Robert Kuhfss" << endl;
		if (verbose) cout << "-> verbose mode" << endl;
		if (yydebug) cout << "-> debug mode" << endl;
	}

	if (argc == 1)
	{
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
		print ("Reading from console input...");
		AddConsoleCtrlHandler ();
	}
	else {
		print ("Parsing input ...\n");
	}



	auto p = new ProgramNode();
	auto m1 = new ModuleNode(new NameNode("Module1"));
	auto m2 = new ModuleNode(new NameNode("Module2"));
	p->Extend(m1);
	m1->Extend(m2);
	auto fp1 = new ParametersNode();
	auto fp1p1 = new ParameterNode(new NameNode("Parameter1"));
	fp1->Extend(fp1p1);
	auto f1 = new FunctionNode(new NameNode("Function1"), fp1);
	m2->Extend(f1);
	auto test = fp1p1->GetIdentifier();
	auto test2 = fp1p1->GetIdentifier();


	// Parse
	int ret = 0;
	ret = yyparse ();
	info ("--- PARSE COMPLETE: ret:%d, n_nodes:%d ---\n", ret, n_nodes);
	if (tree && nodes) {
		print ("Tree:\n");
		print_node (nodes, 0);
	}

	// Compile
	compile (nodes);

	// Data
	export_data ();

	// Safety flush
	fflush(output_file);

	// Clean up names
	info ("Found %d nodes:\n", nodes_map.size());
	for (auto map = nodes_map.begin(); map != nodes_map.end(); ++map)
	{
		info ("  %p -> %s\n", map->first, map->second);
		delete map->second;
	}
	info ("Found %d identifiers:\n", identifier_map.size());
	for (auto map = identifier_map.begin(); map != identifier_map.end(); ++map)
	{
		info ("  %s -> %p \n", map->first.c_str(), map->second);
		//delete &(map->first);
	}

	// Clean up nodes
	Node *tmp;
	while (nodes) {
		tmp = nodes;
		nodes = tmp->next;
		if (tmp->own_string) {
			free ((void*)tmp->type);
		}
		free (tmp);
	}

	// Clean up files
	fclose (yyin);
	fclose (output_file);
	if (tmp_file) {
		remove (tmp_file);
	}


	if (errors) print ("There where errors\n");
	else print ("Success!\n");

	return ret;
}

void yyerror (char const *s) {
	errors++;
	if (!quiet) fprintf (stderr, "%s in line %d\n", s, yylineno);
}
