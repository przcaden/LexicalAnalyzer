# LexicalAnalyzer

This program serves as a pseudo-compiler, checking for valid symbols and lexemes. Lexemes are keywords that are associated with specific tasks or meanings in your code, while tokens are the lexemes' associated phrases the compiler will use. For example, the lexeme "output" will have a corresponding token "t_output". The lexical analyzer will iterate through a source code file and determine if it contains valid lexemes that have a certain token associated with them. This program was written in C++, though the lexical analyzer will scan a hypothetical programming language shown later in this readme.

## Process

The program requires three text files to operate properly: a file containing token and lexeme values, a source code file, and a data output file to print the results of each lexeme
found in the source code. The user of this program will be prompted for the name of all three files (including the .txt file extension). The lexical analyzer will then iterate 
through each line of the source code and check if each character is equivalent to a symbol (such as parentheses and operators), an integer, an alphabetical letter, or a double 
quotation mark used for strings. After each lexeme is found, the lexemes and their corresponding token value will be written to the output file. A message will be printed to the
console which tells the user if the source code runs successfully or not.

Source code written like this, for example:
```
main()
	begin
		integer var=5;
		if (var == 5 || var>=10)
			output("hello world!")
		end if
	end
  ```
  Will produce an output like this:
  ```
t_main : main
s_lparen : (
s_rparen : )
t_begin : begin
t_integer : integer
t_var : var
s_assign : =
t_int : 5
s_semi : ;
t_if : if
s_lparen : (
t_var : var
s_eq : ==
t_int : 5
s_or : ||
t_var : var
s_ge : >=
t_int : 10
s_rparen : )
t_output : output
s_lparen : (
t_str : hello world!
s_rparen : )
t_end : end
t_if : if
t_end : end
```

## Known issues (needs to be fixed)
- Does not check for invalid symbols (such as $, @, ~)
- The program will not run successfully when only a single quotation mark is found in a line. It will continually search for a second one.
