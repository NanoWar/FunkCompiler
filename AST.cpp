#include "Global.h"
#include "AST.h"
#include "Console.h"

extern int verbose;

Node *nodes = NULL;
int n_nodes = 0;

Node *mk_node(char const *name, int n, ...) {
	va_list ap;
	int i = 0;
	unsigned sz = sizeof(Node) + (n * sizeof(Node *));
	Node *nn, *nd = (Node *)malloc(sz);

	info ("# New %d-ary node: %s = %p\n", n, name, nd);

	// Value
	nd->own_string = 0;
	nd->type = name;

	// Navigation
	nd->prev = NULL;
	nd->next = nodes;
	if (nodes) {
		nodes->prev = nd;
	}
	nodes = nd;

	// Real value
	nd->is_proxy = false;
	nd->proxy = NULL;
	nd->has_value = false;
	nd->value = 0;


	// Children
	nd->n_elems = n;
	va_start(ap, n);
	while (i < n) {
		nn = va_arg(ap, Node *);
		info ("#   arg[%d]: %p\n", i, nn);
		info ("#            (%s ...)\n", nn->type);
		nd->elems[i++] = nn;
		nn->parent = nd;
	}
	va_end(ap);
	n_nodes++;
	return nd;
}

Node *mk_atom(char *name) {
	Node *nd = mk_node((char const *)strdup(name), 0);
	nd->own_string = 1;
	return nd;
}

Node *mk_none() {
	return mk_atom("<none>");
}

Node *ext_node(Node *nd, int n, ...) {
	va_list ap;
	int i = 0, c = nd->n_elems + n;
	unsigned sz = sizeof(Node) + (c * sizeof(Node *));
	Node *nn;

	if (verbose)
		info ("# Extending %d-ary node by %d nodes: %s = %p",
		nd->n_elems, c, nd->type, nd);

	if (nd->next) {
		nd->next->prev = nd->prev;
	}
	if (nd->prev) {
		nd->prev->next = nd->next;
	}
	nd = (Node*) realloc(nd, sz);
	nd->prev = NULL;
	nd->next = nodes;
	nodes->prev = nd;
	nodes = nd;

	info (" ==> %p\n", nd);

	va_start(ap, n);
	while (i < n) {
		nn = va_arg(ap, Node *);
		info ("#   arg[%d]: %p\n", i, nn);
		info ("#            (%s ...)\n", nn->type);
		nd->elems[nd->n_elems++] = nn;
		nn->parent = nd;
		++i;
	}
	va_end(ap);
	return nd;
}
