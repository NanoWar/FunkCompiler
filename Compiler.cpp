#include "Global.h"

#include "AST.h"
#include "StringMap.h"
#include "Register.h"
#include "Console.h"
#include "StringHelper.h"

#define GETOWNSTRING(n) (n->elems[0]->type)
#define NAMEOF(n) (get_name (n->elems[0]))
#define FRAMEDNAMEOF(n) (parse_ident (n->elems[0]))

extern FILE *output_file;

unordered_map<Node *, const char*> nodes_map;
unordered_map<string, Node*> identifier_map;
unordered_map<Node *, const char*> names_map;

int current_app_page;
vector<const char *> modules;
vector<Node *> frames;

int lbl_count = 0;

void write (const char* format, ...) {
	va_list args;
	va_start (args, format);
	vfprintf (output_file, format, args);
	va_end (args);
}

const char *combine_modules () {
	auto result = join (modules, '.');
	return result;
}

const char *get_name (Node *n)
{
	const char *name = NULL;
	if (name = names_map[n]) return name;

	// Name node
	if (strcmp (n->type, "Name") == 0) {
		// Type of Name node holds name
		auto buf = new char[strlen (GETOWNSTRING(n)) + 1];
		strcpy (buf, GETOWNSTRING(n));
		name = buf;
	}
	// Fields node (x.y.z)
	else if (strcmp (n->type, "Fields") == 0 || strcmp (n->type, "Ident") == 0)
	{
		vector<const char *> parts;
		for (int i = 0; i < n->n_elems; i++)
		{
			if (auto name = get_name (n->elems[i]))
				parts.push_back (name);
		}
		name = join (parts, '.');
	}
	else {
		// Error
	}

	if (name) {
		//if (!identifier_map[name]) identifier_map[name] = n;
		names_map[n] = name;
	}
	return name;
}

const char *parse_ident (Node *n)
{
	const char *name, *module;
	if (name = nodes_map[n]) return name;

	if (strcmp (n->type, "Ident") != 0) {
		// Error
	}

	// Ident node (x.y.z)
	vector<const char *> parts;

	// Add module prefix.
	module = join (modules, '.');
	if (module) parts.push_back (module);

	// Iterate over name and fields.
	for (int i = 0; i < n->n_elems; i++) {
		if (auto name = get_name (n->elems[i]))
			parts.push_back (name);
	}

	name = join (parts, '.');

	if (module) delete module;

	if (name) {
		if (!identifier_map[name]) {
			identifier_map[name] = n;
		}
		nodes_map[n] = name;
	}

	return name;
}

int evaluate (Node *n)
{
	int result = 0;
	n->format = "%i";

	// VALUE TYPES
	if (strcmp (n->type, "Integer") == 0) {
		auto str = GETOWNSTRING(n);
		result = (int) atoi(str);
		n->has_value = true;
	}
	else if (strcmp (n->type, "NegInteger") == 0) {
		auto str = GETOWNSTRING(n);
		result = (int) -atoi(str);
		n->has_value = true;
	}
	else if (strcmp (n->type, "Char") == 0) {
		auto str = GETOWNSTRING(n);
		result = (int) str[1];
		n->has_value = true;
	}

	// EXPRESSIONS
	else if (strcmp (n->type, "Indirection") == 0) {
		result = evaluate(n->elems[0]);
		n->has_value = n->elems[0]->has_value;
	}
	else if (strcmp (n->type, "Plus") == 0) {
		result = evaluate(n->elems[0]) + evaluate(n->elems[1]);
		n->has_value = n->elems[0]->has_value && n->elems[1]->has_value;
	}
	else if (strcmp (n->type, "Minus") == 0) {
		result = evaluate(n->elems[0]) - evaluate(n->elems[1]);
		n->has_value = n->elems[0]->has_value && n->elems[1]->has_value;
	}
	else if (strcmp (n->type, "Mul") == 0) {
		result = evaluate(n->elems[0]) * evaluate(n->elems[1]);
		n->has_value = n->elems[0]->has_value && n->elems[1]->has_value;
	}
	else if (strcmp (n->type, "Div") == 0) {
		auto rhs = evaluate(n->elems[1]);
		if(rhs == 0) {
			/* TODO: Error */
			result = 0;
		}
		else {
			result = evaluate(n->elems[0]) / rhs;
		}
		n->has_value = n->elems[0]->has_value && n->elems[1]->has_value;
	}

	// PROXIES
	else if (strcmp (n->type, "Ident") == 0) {
		auto name = parse_ident (n);
		auto node = identifier_map[name];
		if (node != n) {
			//write (" ; %s -> %s (%p)\n", get_name (n), name, node);
		}
		else {
			// TODO: Error
		}
		n->proxy = name;
		n->is_proxy = true;
		n->format = "%s";
	}
	else if (strcmp (n->type, "String") == 0) {
		auto str = get_string(GETOWNSTRING(n));
		//write (" ; string: %p -> %s\n", str, str);
		//TODO: Make pointer, eval later
		n->proxy = str;
		n->is_proxy = true;
		n->format = "str_%p";
	}

	n->value = result;
	return result;
}

int compile (Node *n);

int compile_children (Node *n)
{
	int errors = 0;
	for (int i = 0; i < n->n_elems; ++i) {
		errors += compile (n->elems[i]);
	}
	return errors;
}

const char *get_type(Node *expr)
{
	const char *type = 0;
	if (strcmp (expr->type, "Ident") == 0) {
		auto proxy_node = identifier_map[expr->proxy];
		if (strcmp (proxy_node->parent->type, "DeclLocal") == 0) {
			type = get_name (proxy_node->parent->elems[1]);
		}
		if (strcmp (proxy_node->parent->type, "ParamDef") == 0) {
			type = get_name (proxy_node->parent->elems[0]);
		}
	}
	return type;
}

void write_expr_value(Node *expr)
{
	if (expr->has_value) {
		write ("%i", expr->value);
	}
	else if (strcmp (expr->type, "Ident") == 0) {
		auto type = get_type(expr);
		if (type)
			write ("%s", type);
		else 
			write ("%s", expr->proxy);


	}
	else if (strcmp (expr->type, "String") == 0) {
		write ("str_%p", expr->proxy);
	}
	else if (strcmp (expr->type, "Indirection") == 0) {
		write ("(");
		write_expr_value (expr->elems[0]);
		write (")");
	}
	else if (strcmp (expr->type, "Plus") == 0) {
		auto lhs_type = get_type(expr->elems[0]);
		auto rhs_type = get_type(expr->elems[1]);
		if (lhs_type) {
			write (" add %s, ", lhs_type);
			write_expr_value (expr->elems[1]);
			write ("\n");
		}
		//write_expr_value (expr->elems[0]);
		//write (" + ");
		//write_expr_value (expr->elems[1]);
	}
	else if (strcmp (expr->type, "Minus") == 0) {
		write_expr_value (expr->elems[0]);
		write (" - ");
		write_expr_value (expr->elems[1]);		
	}
	else {
		write ("%i", expr->value);
	}
}

void enter_frame(Node *n, const char *name)
{
	info (">> Entering frame '%s'\n", name);
	UnUseAllRegisters();

	frames.push_back (n);
	modules.push_back (name);
}

void exit_frame()
{
	auto name = modules.back();
	info ("<< Exiting frame '%s'\n", name);

	UnUseAllRegisters();

	modules.pop_back ();
	frames.pop_back ();
}

int compile (Node *n)
{
	int errors = 0;

	if (!n) { return ++errors; }

	if (strcmp (n->type, "Module") == 0) {
		enter_frame(n, NAMEOF(n));
		errors += compile_children (n);
		exit_frame();
	}
	else if (strcmp (n->type, "Function") == 0) {
		write ("%s\n", FRAMEDNAMEOF(n));
		enter_frame(n, NAMEOF(n));
		errors += compile_children (n);

		info ("Used registers: ");
		for (auto it = reg8_map.begin(); it != reg8_map.end(); ++it) {
			if (IsRegisterUsed(it->second))
				info ("%s ", it->first.c_str());
		}
		info ("\n");

		exit_frame();
		write (" ret\n");
	}
	else if (strcmp (n->type, "ParamDef") == 0) {
		auto type = get_name (n->elems[0]);
		auto bind = get_name (n->elems[1]);
		auto pre = n->elems[2]->type;
		auto name = parse_ident (n->elems[3]);
		if (!UseRegister(type)) {
			cerr << "Warning: Register '" << type << "' already in use." << endl;
		}
		info ("Var %s = %s\n", name, type); 
	}
	else if (strcmp (n->type, "DeclLocal") == 0) {
		auto name = parse_ident (n->elems[0]);
		auto type = get_name (n->elems[1]);
		auto expr = n->elems[2];
		auto val = evaluate (expr);

		if (type) {
			if (!UseRegister(type)) {
				cerr << "Warning: Register '" << type << "' already in use." << endl;
			}
			info ("Var %s = %s\n", name, type);
			write_expr_value (expr);
			write (" ld %s, ", type);
			write (" ???");
			write ("\n");
		}
		else {
			write("#define %s ", name);
			write_expr_value (expr);
			write ("\n");
		}
	}
	else if (strcmp (n->type, "Loop") == 0) {
		int loop_nr = lbl_count++;
		write (" ld\tb, %s\n_lbl%d\n", get_name (n->elems[0]), loop_nr);
		errors += compile_children (n);
		write (" djnz\t_lbl%d\n", loop_nr);
	}
	else {
		errors += compile_children (n);
	}
	return errors;
}

void export_data()
{
	int page = 0;
	for (auto map = paged_stringmaps.begin(); map != paged_stringmaps.end(); ++map, ++page) {
		write ("; DATA PAGE %i\n", page);
		for (auto str = (*map)->begin(); str != (*map)->end(); ++str) {
			write ("str_%p", str->second);
			write (" .db %s, 0\n", str->second);
		}
	}
}