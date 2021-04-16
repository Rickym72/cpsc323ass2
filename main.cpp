
/*
- Use g++ -Wall -c -g main.cpp -o main.o
  followed by g++ main.o -o main

- Run the program with ./main
*/

// Include Libraries and header files
#include <iostream>
#include "FSM.h"
#include "syntax.h"
using namespace std;

int main(int argc, char *argv[])
{	
	//initalizing variables
	vector<string> codeVector;
	ofstream fout;
	ifstream fin;
	string file_name;
	string input;
	string line;

	//error message for incorrect command
  	if(argc < 2)
		{cout << "ERROR - format should be: ./main inputFile\n"; exit(1);}
  	else
    	input = argv[1];
	//Open our input
	fin.open(input);
	if (!fin.is_open())
		{cerr << "File Opening Error\n"; exit(-1);}

	while (getline(fin, line)) {codeVector.push_back(line);}
	fin.close();

	//LEXER
	cout << "Time for lexer to do its thing...." << endl;
	//vector data type creation
	vector<tokens> lexerStorage;
	FSM machine;
	
	//initializing
	int state = 0;
	int lexStart = 0;


		// These for loops need the integer type because of the return type of .size() and .length()
	for (long long unsigned int vecString = 0; vecString < codeVector.size(); vecString++) {
		for (long long unsigned int vecChar = 0; vecChar <= codeVector[vecString].length(); vecChar++) {
			if (state == 0) {
				lexStart = vecChar;
			}

			state = machine.check_input(state, machine.char_to_input(codeVector[vecString][vecChar]));
			if (machine.is_final_state(state)) {
				if (machine.should_back_up(state)) {
					vecChar--;
				}
				if (state != 7) {
					string lex = "";
					for (long long unsigned int i = lexStart; i <= vecChar; i++) {
						lex += codeVector[vecString][i];
					}
					if (machine.getTokenName(state, lex) != "OTHER") {
						lexerStorage.push_back(tokens(machine.getTokenName(state, lex), lex));
					}
				}//set state
				state = 0;
			}
		}
	}
	//output file stream (tokens/lexemes) to text file.
	fout.open("output.txt");
	if(!fout.is_open()){ cout << "Output File Error\n"; exit(1);}
	if (!syntaxAnalyze(lexerStorage, fout)) {
		cout << "Syntax error" << endl;
		fout << "ERROR: syntax error found in the source code" << endl;
	}

	fout.close();

	return 0;
}
