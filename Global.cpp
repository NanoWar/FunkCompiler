#include "Global.h"

// Settings
int verbose = 0;
int quiet = 0;
int repl = 0;
int no_colors = 0;

// Input / output
string input_file_folder = "";
string input_file_name_full;
string input_file_name;
string input_file_ext;
FILE *input_file = NULL;
FILE *output_file = NULL;

int errors = 0;
