
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
		bool exists = false;
		for (int i=0; i<tokenmap.size(); i++) { // if you are using an iterator to iterate through a data structure, do not change the size.
			if (exists)
				exists = true;
		}
		return exists;
	}
	// pre: parameter refers to a string holding a lexeme received from a source code file.
	// post: returns true if the tokenmap contains the lexeme string, false if not.

	bool isVariable(string lex) { // still unsure if this method is needed yet.

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

	bool isString(string lex, char l, int index) {
		bool valid = false;;
		if (l == '"' && index!=lex.length()-1) {
			for (int i=index; i<lex.length(); i++) {
				if (lex[i] == '"')
					valid = true;
			}
		}
		return valid;
	}

	void scanFile(istream& infile, ostream& outfile) {
		string lex;
		infile >> lex;

		while ( !infile.eof() ) {

			int t = 0;
			string currlex;
			while ( t<lex.length() ) {
				// Checks if the current lexeme is an integer.
				if ( isInt(lex[t]) ) {
					cout << "in int " << lex[t] << endl;
					currlex.push_back(lex[t]);
					for (int i=t; i<lex.length(); i++) { // checks for any numbers after the first.
						if (isInt(lex[i])) {
							currlex.push_back(lex[i]);
							t++;
						}
						else i=lex.length();
					}
					lexemes.push_back(currlex);
					tokenmap[currlex] = "t_int";
				}
				// Checks if the current lexeme is a keyword or identifier.
				else if ( isalpha(lex[t])) {
					cout << "in alpha " << lex[t] << endl;
					currlex.push_back(lex[t]);
					for (int i=t; i<lex.length(); i++) {
						if (isalpha(lex[i])) {
							currlex.push_back(lex[i]);
							t++;
						}
						else i=lex.length();
					}
					// Checks if lexeme matches a token. If not, it must be an identifier.
					if ( contains(currlex) )
						lexemes.push_back(currlex);
					else tokenmap[currlex] = "t_id";
				}
				// Checks if the current lexeme is a symbol.
				else if ( isSymbol(lex[t])) {
					cout << "in symbol " << lex[t] << endl;
					currlex.push_back(lex[t]);
					t++;
					if ( lex[t] == '=' && t!=lex.length()-1 && lex[t+1] == '=' ) {
						cout << "in equal" << endl;
						currlex.push_back(lex[t+1]);
						t++;
					}
					if ( (lex[t] == '<' || lex[t] == '>') && t!=lex.length()-1 && lex[t+1] == '=' ) {
						cout << "in less than/gt" << endl;
						currlex.push_back(lex[t+1]);
						t++;
					}
					if ( lex[t] == '!' && t!=lex.length()-1 && lex[t+1] == '=') {
						cout << "in negator" << endl;
						currlex.push_back(lex[t+1]);
						t++;
					}
					else lexemes.push_back(currlex);
				}
				else if ( isString(lex, lex[t], t) ) {
					cout << "in string " << lex[t] << endl;
					for (int i=t; i<lex.length(); i++) {
						if (lex[i] != '"') {
							currlex.push_back(lex[i]);
							t = i;
						}
					}
					lexemes.push_back(currlex);
					tokenmap[currlex] = "t_str";
				}

				else t++;
			}

			infile >> lex;
		}




		// End of process: print to output file
		for (int i=0; i<lexemes.size(); i++) {
			lex = lexemes[i];

			if (!contains(lex))
				outfile << lex << " >>>> error: invalid token." << endl;
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
	cout << "Enter the lexeme data file name: " << endl;
	cin >> input;
	ifstream tokenfile(input);
	LexAnalyzer analyzer(tokenfile);

	string codename;
	cout << "Enter name of source code file: " << endl;
	cin >> codename;

	ifstream codefile(codename);
	ofstream outfile("lexemeoutput.txt");
	analyzer.scanFile(codefile, outfile);

	return 0;
}

/*
	x = 3; valid --> x is not a keyword, must be a variable. = is a symbol. 3 is a number (t_int). ; is a symbol (s_semi)
	3;=x valid
	3=;x valid
	x3;= valid
	3x;= NOT VALID:

	x3main ---> x is no other token, must be a variable. 3 and main are also part of the variable.
*/
