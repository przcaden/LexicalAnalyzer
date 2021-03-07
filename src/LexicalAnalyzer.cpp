
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

	bool contains(string lex) {
		return (tokenmap.count(lex) > 0);
	}
	// pre: parameter refers to a string containing a lexeme received from a source code file.
	// post: returns true if the tokenmap contains the lexeme string, false if not.

	bool isSymbol(char l) {
		return (l == '=' || l == ',' || l == '(' || l == ')' || l == ';' || l == '<' || l == '>' || l == '|'
			|| l == '!' || l == '+' || l == '-' || l == '*' || l == '/' || l == '%' || l == '&');
	}
	// pre: parameter

	bool isInt(char l) {
		bool digit = false;
		char temp;
		for (int i=1; i<10; i++) {
			temp = '0' + i;
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

	bool checkInt(int& index, string& currlex, string lex) {
		currlex.push_back(lex[index]);
		index++;
		bool valid = true;
		for (int i=index; i<lex.length(); i++) { // checks for any numbers after the first.
			if ( isInt(lex[i]) ) {
				currlex.push_back(lex[i]);
				index++;
			}
			else if (isalpha(lex[i])) {
				currlex.push_back(lex[i]);
				valid = false;
			}
			else i=lex.length();
		}
		return valid;
	}

	void checkKeyword(int& index, string& currlex, string lex) {
		currlex.push_back(lex[index]);
		index++;
		for (int i=index; i<lex.length(); i++) {
			if ( isalpha(lex[i]) || isInt(lex[i] && !isSymbol(lex[i])) ) {
				currlex.push_back(lex[i]);
				index++;
			}
			else i=lex.length();
		}
	}

	void checkSymbol(int& index, string& currlex, string lex) {
		currlex.push_back(lex[index]);
		index++;
		if ( lex[index] == '=' && index!=lex.length()-1 && lex[index+1] == '=' ) {
			cout << "in equal" << endl;
			currlex.push_back(lex[index+1]);
			index++;
		}
		if ( (lex[index] == '<' || lex[index] == '>') && index!=lex.length()-1 && lex[index+1] == '=' ) {
			cout << "in less than/gt" << endl;
			currlex.push_back(lex[index+1]);
			index++;
		}
		if ( lex[index] == '!' && index!=lex.length()-1 && lex[index+1] == '=') {
			cout << "in negator" << endl;
			currlex.push_back(lex[index+1]);
			index++;
		}
		else lexemes.push_back(currlex);
	}

	void checkString(int& index, string& currlex, string lex) {
		index++;
		for (int i=index; i<lex.length(); i++) {
			if (lex[i] != '"') {
				currlex.push_back(lex[i]);
				index = i;
			}
		}
	}

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
	// lexeme pairs i.e.  s_and and t_begin begin t_int 27.  Each pair appears on its own input line.
	// post: tokenmap has been populated

	void scanFile(istream& infile, ostream& outfile) {
		string lex;
		infile >> lex;

		while ( !infile.eof() ) {
			int index = 0; // holds the index of the current lexeme.
			string currlex;
			while ( index<lex.length() ) {
				currlex.clear();
				// Checks if the current lexeme is an integer.
				if ( isInt(lex[index]) ) {
					if (checkInt(index, currlex, lex))
						tokenmap[currlex] = "t_int";
					lexemes.push_back(currlex);
				}
				currlex.clear();

				// Checks if the current lexeme is a keyword or identifier.
				if ( isalpha(lex[index]) ) {
					checkKeyword(index, currlex, lex);
					// Checks if lexeme matches a token. If not, it must be an identifier.
					if ( contains(currlex) )
						lexemes.push_back(currlex);
					else tokenmap[currlex] = "t_id";
				}
				currlex.clear();

				// Checks if the current lexeme is a symbol.
				if ( isSymbol(lex[index]) )
					checkSymbol(index, currlex, lex);
				currlex.clear();

				// Checks if the current lexeme is a string held in double quotations.
				if ( isString(lex, lex[index], index) ) {
					checkString(index, currlex, lex);
					lexemes.push_back(currlex);
					tokenmap[currlex] = "t_str";
				}
				//else index++;
			}
			infile >> lex;
		}

		bool valid = true;
		// End of process: print to output file
		for (int i=0; i<lexemes.size(); i++) {
			lex = lexemes[i];
			if (!contains(lex)) {
				outfile << lex << " >>> error: invalid token." << endl;
				valid = false;
			}
			else
				outfile << tokenmap[lex] << " : " << lex << endl;
		}
		if (valid)
			cout << "Valid: Source code was ran successfully." << endl;
		else cout << "Error: Code was ran unsuccessfully. Source code contains an invalid token.";
	}
	// pre: 1st parameter refers to an open text file that contains source
	// code in the language, 2nd parameter refers to an open empty output
	// file
	// post: If no error, the token and lexeme pairs for the given input
	// file have been written to the output file.  If there is an error,
	// the incomplete token/lexeme pairs, as well as an error message have
	// written to the output file.  A success or fail message has printed to the console.

};


int main() {
	string input;
	cout << "Enter the lexeme data file name: " << endl;
	//cin >> input;
	ifstream tokenfile("tokenlexemedata.txt");
	LexAnalyzer analyzer(tokenfile);

	cout << "Enter name of source code file: " << endl;
	//cin >> input;
	ifstream codefile("sourcecode.txt");

	cout << "Enter the name of the data output file: " << endl;
	//cin >> input;
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
