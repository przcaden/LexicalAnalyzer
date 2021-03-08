
#include <iostream>
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
	// pre: parameter is a character within a lexeme gotten from a source code file.
	// post: returns true if the character is a valid lexeme symbol, false if not.

	bool isInt(char l) {
		bool digit = false;
		char temp;
		for (int i=0; i<10; i++) {
			temp = '0' + i;
			if (l == temp)
				digit = true;
		}
		return digit;
	}
	// pre: pre: parameter is a character within a lexeme gotten from a source code file.
	// post: returns true if the character is a digit 0-9, false if not.

	bool isString(string lex, char l, int& index) {
		bool valid = false;
		if (l == '"' && index!=lex.length()-1) {
			for (int i=index; i<lex.length(); i++) {
				if (lex[i] == '"')
					valid = true;
			}
		}
		return valid;
	}
	// pre: 1st parameter is a lexeme gotten from the source code file, 2nd param is the current
	// iterated character of the lexeme, 3rd param is the index of the character.
	// post: returns true if there are two double quotation characters found in the current line
	// of the source code file, false if not.

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
	// pre: 1st parameter refers the index of a lexeme gotten from the source code being iterated over,
	// 2nd param refers to the current lexeme that will be pushed into the lexeme vector, 3rd param refers
	// to a lexeme gotten from the source code file.
	// post: returns true if the lexeme consists of only digits, otherwise the lexeme is labeled as an
	// identifier and returns false.

	void checkKeyword(int& index, string& currlex, string lex) {
		currlex.push_back(lex[index]);
		index++;
		for (int i=index; i<lex.length(); i++) {
			if ( isalpha(lex[i]) || isInt(lex[i]) ) {
				currlex.push_back(lex[i]);
				index++;
			}
			else i=lex.length();
		}
		// Checks if lexeme matches a token. If not, it must be an identifier.
		lexemes.push_back(currlex);
		if ( !contains(currlex) )
			tokenmap[currlex] = "t_id";
	}
	// pre: 1st parameter refers the index of a lexeme gotten from the source code being iterated over,
	// 2nd param refers to the current lexeme that will be pushed into the lexeme vector, 3rd param refers
	// to a lexeme gotten from the source code file.
	// post: the current lexeme is populated with characters until the lexeme reaches a character that is not
	// alphabetical. if the lexeme is not found in the tokenmap, a new key is added for an identifier.

	void checkSymbol(int& index, string& currlex, string lex) {
		currlex.push_back(lex[index]);
		index++;
		if ( (index!=lex.length()) && ((lex[index-1] == '=' && lex[index] == '=')
				|| ( (lex[index-1] == '<' || lex[index-1] == '>') && lex[index] == '=' ) || ( lex[index-1] == '!' && lex[index] == '=' )
				|| ( lex[index-1] == '|' && lex[index] == '|' ) || ( lex[index-1] == '&' && lex[index] == '&' )) ) {
			currlex.push_back(lex[index]);
			index++;
		}
		lexemes.push_back(currlex);
	}
	// pre: 1st parameter refers the index of a lexeme gotten from the source code being iterated over,
	// 2nd param refers to the current lexeme that will be pushed into the lexeme vector, 3rd param refers
	// to a lexeme gotten from the source code file.
	// post: pushes any possible symbols into the lexemes vector. if any lexeme contains an operator that
	// consists of more than one character, the entire operator is pushed into the vector instead.

	void checkString(string& lex, int& index, string& currlex, istream& infile) {
		index++;
		bool end = false;
		while ( !end ) {
			if (lex[index] == '"')
				end = true;
			else {
				currlex.push_back(lex[index]);
				index++;
			}
		}
		index++;
		lexemes.push_back(currlex);
		tokenmap[currlex] = "t_str";
	}
	// pre: 1st parameter refers to a lexeme received from the source code file, 2nd parameter refers
	// to the index of the lexeme, 3rd param refers to the current lexeme that will be pushed into the
	// lexeme vector, 4th param refers to source code file.
	// post: method iterates through all characters within double quotation marks, pushing each character
	// into the current lexeme.

	void checkValidity(ostream& outfile) {
		bool valid = true;
		// End of process: print to output file
		for (int i=0; i<lexemes.size(); i++) {
			string lex = lexemes[i];
			if (!contains(lex)) {
				outfile << lex << " >>> error: invalid token." << endl;
				valid = false;
			}
			else
				outfile << tokenmap[lex] << " : " << lex << endl;
		}
		if (valid)
			cout << "valid: source code was ran successfully." << endl;
		else cout << "error: source code contains an invalid token.";
	}
	// pre: parameter refers to an output file
	// post: lexeme vector is iterated through and the validity of each lexeme is written to the
	// output file. end result of the source code scan is outputted in the console.

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
		while ( getline(infile, lex) ) {
			int index = 0;
			string currlex; // Individual lexeme that will be extracted from each line.
			while ( index<lex.length() ) {

				// Performs checks if the current lexeme is a keyword or identifier.
				if ( isalpha(lex[index]) ) {
					checkKeyword(index, currlex, lex);
					currlex.clear();
				}

				// Performs checks if the current lexeme is an integer.
				else if ( isInt(lex[index]) ) {
					if (checkInt(index, currlex, lex))
						tokenmap[currlex] = "t_int";
					lexemes.push_back(currlex);
					currlex.clear();
				}

				// Performs checks if the current lexeme is a symbol.
				else if ( isSymbol(lex[index]) ) {
					checkSymbol(index, currlex, lex);
					currlex.clear();
				}

				// Performs checks if the current lexeme is a string held in double quotations.
				else if ( isString(lex, lex[index], index) ) {
					checkString(lex, index, currlex, infile);
					currlex.clear();
				}
				else index++;
			}
		}
		checkValidity(outfile);
	}
	// pre: 1st parameter refers to an open text file that contains source
	// code in the language, 2nd parameter refers to an open empty output file
	// post: If no error, the token and lexeme pairs for the given input
	// file have been written to the output file.  If there is an error,
	// the incomplete token/lexeme pairs, as well as an error message have
	// written to the output file.  A success or fail message has printed to the console.

};


int main() {
	string input;
	cout << "Enter the lexeme data file name: " << endl;
	cin >> input;
	ifstream tokenfile(input);
	LexAnalyzer analyzer(tokenfile);

	cout << "Enter name of source code file: " << endl;
	cin >> input;
	ifstream codefile(input);

	cout << "Enter the name of the data output file: " << endl;
	cin >> input;
	ofstream outfile(input);
	analyzer.scanFile(codefile, outfile);

	return 0;
}
