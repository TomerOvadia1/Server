//
// Created by tomer on 12/18/18.
//

#ifndef INTERPRETER_LEXERFUNCTION_H
#define INTERPRETER_LEXERFUNCTION_H

#include <string>
#include <vector>

using namespace std;

class LexerFunction{

	//TODO TEST
	static void handle_expression(string& line,vector<string>& args, int*
	index , string& word);

	static void read_until(string &line, int *index, string &word, char to_find,
							   bool ignore_seperators);

	static void handle_math_expressions(vector<string>& args , int
	lastExpIndex ,string& word);

	static void handle_operators(vector<string> &args, string &word,
								 char op, int *index, string &line);

	static bool expressionEndedPropely(char c){
		return ( c != '-' && c!='/' && c!='*' &&c !='+' && c != '(' );
	}

	static bool expressionStartedPropely(char c){
		return ( c!='/' && c!='*' &&c !='+' && c != ')' );
	}

	static bool is_a_separator(char c);

	static bool is_a_space(char c);

public:
	static vector<string> lexer(string& line);


};
#endif //INTERPRETER_LEXERFUNCTION_H
