#ifndef Lexer_h
#define Lexer_h

void before_rule();
void after_rule();

void yypush_location_state(const char *new_file_name);
void yypop_location_state();

#endif Lexer_h