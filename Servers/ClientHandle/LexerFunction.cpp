//
// Created by tomer on 12/23/18.
//
#include "LexerFunction.h"
const char FORCE_SPLIT = ',';

vector<string> LexerFunction::lexer(string& line) {
	string word;
	// adding splitter character at the end
	// of 'line'
	line += " ";
	// traversing 'line' from left to right
	vector<string> args;
	//user entered a comma
	bool force_seperate = false;
	for (int i = 0; i < line.size(); i++) {
		// if line[i] is not equal to the splitter
		if (!is_a_separator(line[i])) {
			handle_expression(line , args , &i , word);

		} else {
			if(line[i]== FORCE_SPLIT){
				force_seperate =true;
			}
			if (!word.empty()) {
				int lastExpIndex = args.size()-1;
				//if this is not the first argument , and user did not enter
				//a comma separator , math expression might need to be validate
				if(lastExpIndex > 0 && !force_seperate){
					handle_math_expressions(args , lastExpIndex , word);
				}else{
					force_seperate = false;
					args.push_back(word);
					word = "";
				}
			}
		}
	}
	// return the split line
	return args;

}

//TODO TEST
void LexerFunction::handle_expression(string& line,vector<string>& args, int*
index , string& word){


	switch (line[*index]){
		case '=':
		case '<':
		case '>':
		case '!':
		case '{':
		case '}':
			handle_operators(args, word, line[*index], index, line);
			break;
		case '\'':
		case '\"':
			read_until(line, index, word, line[*index], false);
			break;
		case '(':
			read_until(line, index, word, ')', true);
			break;
		default:
			word += line[*index];
			break;
	}

}

void LexerFunction::read_until(string &line, int *index, string &word, char to_find,
							   bool ignore_seperators) {
	bool found = false;
	word += line[*index];
	*index = *index +1 ;
	//loop until another " is found
	while(*index < line.length()){
		if(ignore_seperators){
			if(!is_a_space(line[*index])){
				word += line[*index];
			}
		}else{
			word += line[*index];
		}
		if(line[*index] == to_find){
			found = true;
			break;
		}
		*index = *index +1 ;
	}
	if (!found){
		word = "";
		throw "Could not find matching appearance of " + string(1,to_find );
	}
}


void LexerFunction::handle_math_expressions(vector<string>& args , int
lastExpIndex ,string& word) {
	string lastExp = args[lastExpIndex];
	int lastExpLen = args[lastExpIndex].length();

	//if last expresion did not end properly , it needs to be merged
	if (!expressionEndedPropely(lastExp[lastExpLen - 1])) {
		args[lastExpIndex] += word;
		word = "";
		//if given expression did not start properly it needs to be merged with
		// previous
	} else if (!expressionStartedPropely(word[0])) {
		args[lastExpIndex] += word;
		word = "";
	} else {
		args.push_back(word);
		word = "";
	}
}

void LexerFunction::handle_operators(vector<string> &args, string &word,
									 char op, int *index, string &line) {

	int tempIndex = *index;
	char assignmentOp = '=';
	string to_push = string(1, op);
	switch (op) {
		case '<':
		case '>':
		case '!':
			// try to find a matching assigning operator
			//advance position post the current operator
			tempIndex++;
			//loop until a non-separator char is found
			while (tempIndex < line.length()) {
				char c = line[tempIndex];
				//if it is an '='
				if (line[tempIndex] == '=') {
					//set index to temp index
					*index = tempIndex;
					//push the given comparison operator
					to_push = (string(1, op) + string(1, '='));
					break;
				} else if (!(is_a_space(c))) {
					break;
				}
				tempIndex++;
			}
	}
	if(!word.empty()){
		args.push_back(word);
	}
	args.push_back(to_push);

	word = "";

}

bool LexerFunction::is_a_separator(char c){
	return (c== ' ' || c== '\t' || c==',' || c=='\n');
}

bool LexerFunction::is_a_space(char c){
	return (c== ' ' || c== '\t' );
}


