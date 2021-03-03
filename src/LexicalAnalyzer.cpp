

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
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

	LexAnalyzer(istream& infile) {
		string token;
		string lexeme;
		infile >> token >> lexeme;
		while (!infile.eof()) {
			//m[lexeme] = token;
			tokenmap.insert(lexeme, token);
			tokens.push_back(token);
			lexemes.push_back(lexeme);

			infile >> token >> lexeme;
		}
	}
	// pre: parameter refers to open data file consisting of token and
	// lexeme pairs i.e.  s_and and t_begin begin t_int 27.  Each pair    // appears on its own input line.
	// post: tokenmap has been populated

	bool contains(vector<string>& v, string lex) {
		vector<string>::iterator vitr;
		bool exists = false;
		for (vitr=v.begin(); vitr!=v.end(); vitr++) { // if you are using an iterator to iterate through a data structure, do not change the size.
			if (*vitr == lex)
				exists = true;
		}
		return exists;
	}
	// pre: 1st parameter refers to a vector that contains all lexemes
	// gotten from tokenlexemedata.txt, 2nd parameter refers to a lexeme
	// received from a source code file.
	// post: returns true if the vector contains the lexeme string, false if not.

	bool isValid(string lex) { // still unsure if this method is needed yet.
		bool valid = true;
		// check string for being an integer.
		if (!isalpha(lex[0])) {

		}
		return valid;
	}

	void scanFile(istream& infile, ostream& outfile) {
		string lex;
		infile >> lex;
		vector<string> usedlexemes;

		bool loop = !infile.eof();
		while (loop) {
			usedlexemes.push_back(lex);
			if (contains(lexemes, lex)) {
				if (isValid(lex)) {
					loop = !infile.eof();
				}
				else {
					loop = false;

				}
			}
		}

		for (int i=0; i<usedlexemes.size(); i++) {
			lex = usedlexemes[i];

			if (i==usedlexemes.size()-1 && !loop)
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
	//ostream outfile;
	//analyzer.scanFile(codefile, outfile);

	map<string, int> m; // standard template data structure.
	m["hello"] = 12;
	m["today"] = 15;
	cout << m["hello"] << endl;

	vector<int>::iterator vitr;
	vector<int> v;
	for (vitr=v.begin(); vitr!=v.end(); vitr++) {
		cout << *vitr << endl; // print value vitr is on
	}

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
