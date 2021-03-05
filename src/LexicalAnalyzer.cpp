
#include<iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
using namespace std;

class LexAnalyzer {
private:
	vector<string> lexemes;  // source code file lexemes
	vector<string> tokens;   // source code file tokens
	map<string, string> tokenmap;  // valid lexeme/token pairs
	// other private methods
public:

	LexAnalyzer(ifstream& infile) {
		string token;
		string lexeme;
		infile >> token >> lexeme;
		while (!infile.eof()) {
			tokenmap[lexeme] = token;
			infile >> token >> lexeme;
		}
	}
	// pre: parameter refers to open data file consisting of token and
	// lexeme pairs i.e.  s_and and t_begin begin t_int 27.  Each pair    // appears on its own input line.
	// post: tokenmap has been populated

	bool contains(string lex) {
		map<string, string>::iterator mitr;
		bool exists = false;
		for (mitr=tokenmap.begin(); mitr!=tokenmap.end(); mitr++) { // if you are using an iterator to iterate through a data structure, do not change the size.
			if (exists)
				exists = true;
		}
		return exists;
	}
	// pre: parameter refers to a string holding a lexeme received from a source code file.
	// post: returns true if the vector contains the lexeme string, false if not.

	bool isVariable(string lex) { // still unsure if this method is needed yet.
		bool var = true;
		string temp;
		for (int i=0; i<lex.length(); i++) {
			if ( isalpha(lex[i]) || isdigit(lex[i]) ) {
				temp.push_back(lex[i]);
			}
			else var = false;
		}

		return var;
	}

	bool isSymbol(char l) {
		return (l == '=' || l == ',' || l == '(' || l == ')' || l == ';' || l == '<' || l == '>' || l == '|'
			|| l == '!' || l == '+' || l == '-' || l == '*' || l == '/' || l == '%' || l == '&');
	}

	bool isInt(char l) {
		bool digit = false;
		char temp;
		for (int i=1; i<10; i++) {
			temp = (char)i;
			if (l == temp)
				digit = true;
		}
		return digit;
	}

	bool isString(string lex) {

	}

	void scanFile(istream& infile, ostream& outfile) {
		string lex;
		infile >> lex;

		while ( !infile.eof() ) {
			if ( isInt(lex[0]) ) { // following must be symbol or another number
				for (int i=1; i<lex.length(); i++) {
					if ( isSymbol(lex[i]) ) {
						lexemes.push_back( lex.substr(i, i+1) );
					}
					else {

					}
				}
			}
			else if ( isSymbol(lex[0]) ) {

			}
		}




		// End of process: print to output file
		for (int i=0; i<lexemes.size(); i++) {
			lex = lexemes[i];

			if (!contains(lex))
				outfile << lex << " : error: invalid token." << endl;
			else
				outfile << tokenmap[lex] << " : " << lex << endl;
		}
	}
	// pre: 1st parameter refers to an open text file that contains source
	// code in the language, 2nd parameter refers to an open empty output
	// file
	// post: If no error, the token and lexeme pairs for the given input
	// file have been written to the output file.  If there is an error,
	// the incomplete token/lexeme pairs, as well as an error message have // written to the output file.  A success or fail message has printed // to the console.
};


int main() {
	string input;
	cout << "Enter the input file name: " << endl;
	cin >> input;
	ifstream tokenfile(input);
	LexAnalyzer analyzer(tokenfile);

	string codename;
	cout << "Enter name of source code file: " << endl;
	cin >> codename;

	ifstream codefile(codename);
	ofstream outfile("lexemeoutput.txt");
	analyzer.scanFile(codefile, outfile);

}

/*
	x = 3; valid --> x is not a keyword, must be a variable. = is a symbol. 3 is a number (t_int). ; is a symbol (s_semi)
	3;=x valid
	3=;x valid
	x3;= valid
	3x;= NOT VALID:

	x3main ---> x is no other token, must be a variable. 3 and main are also part of the variable.
*/
