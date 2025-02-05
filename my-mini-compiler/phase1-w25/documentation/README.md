# **Lexical Analyzer - Phase 1**

## **Project Overview**
This project is the first phase of a compiler construction process, focusing on **Lexical Analysis**. The lexical analyzer (lexer) scans input source code, identifies valid tokens, and reports lexical errors. This serves as the foundation for later phases, including syntax and semantic analysis.

## **Phase 1: Lexical Analysis**
The goal of this phase was to implement a lexer that:
- Reads input character by character.
- Identifies and classifies tokens.
- Handles comments and string literals.
- Reports lexical errors such as invalid identifiers and unterminated strings.

## Design Decisions

#### Distinguishing Between Keywords and Identifiers
- The lexer first checks if a sequence of characters matches a keyword.
- If not, it is treated as an identifier.
- Implemented using a keyword lookup function (`is_keyword()`).

#### Multi-Character Operators
- The lexer does not currently support `==`, `!=`, `<=`, `>=`, but they can be added by modifying the operator recognition logic.

#### Error Messages
- Errors are reported with line numbers and descriptions.
- Example: `Lexical Error at line 3: Unterminated string`.

#### Handling Comments
- The lexer skips over single-line (`//`) and multi-line (`/* */`) comments.
- If a multi-line comment is not closed, an error is generated.

## **Features Implemented**
### **Token Recognition**
The lexer correctly identifies the following token types:
- **Keywords:** `if`, `repeat`, `until`
- **Identifiers:** Variable names following the pattern `[a-zA-Z][a-zA-Z0-9_]*`
- **Numbers:** Integer values such as `123`, `4567`
- **Operators:** `+`, `-`, `*`, `/`, `=`
- **Delimiters:** `;`, `{`, `}`, `(`, `)`
- **String Literals:** Enclosed in `" "`, with escape sequence support (`\"`, `\\`)
- **Comments:** Single-line (`//`) and multi-line (`/* */`)

### **Error Handling**
- **Invalid characters:** Unrecognized symbols trigger an error.
- **Invalid identifiers:** Identifiers cannot start with a digit.
- **Unterminated string literals:** Missing closing quotes are flagged.
- **Unterminated comments:** Multi-line comments missing `*/` generate an error.
- **Consecutive operators:** The lexer prevents invalid sequences like `++`.


| Error Type                     | Description                                      |
|--------------------------------|--------------------------------------------------|
| ERROR_INVALID_CHAR            | Invalid character detected in input.            |
| ERROR_INVALID_NUMBER          | Incorrect number format (not applicable yet).   |
| ERROR_CONSECUTIVE_OPERATORS   | Consecutive operators detected (e.g., ++).      |
| ERROR_UNTERMINATED_STRING     | String is missing a closing quote (").          |
| ERROR_UNTERMINATED_COMMENT    | Multi-line comment is missing a closing */.  

## **How to Run the Lexer**
### **Using GCC and Command Line**
1. Compile the lexer:
   ```sh
   gcc src/lexer/lexer.c -o lexer
   ```
2. Run the lexer:
   ```sh
   ./lexer
   ```

### **Using Visual Studio Code**
1. Open the project in VS Code.
2. Use the integrated terminal to compile and run the lexer.
3. Modify `main()` in `lexer.c` to test different inputs.

## **Test Cases**
Example valid input:
```
"123 + 456 - 789\n"
"if repeat until x = 10;\n"
"someVariable another_var 123abc\n"
"\"Hello, world!\" \"Unclosed string\n"
"// This is a comment\n"
"/* Multi-line comment\nstill inside */\n"
"y = \"escaped \\\"quotes\\\" inside\";\n"
"/* Unterminated comment starts here..."
```
Expected Output:
```
Analyzing input:
123 + 456 - 789
if repeat until x = 10;
someVariable another_var 123abc
"Hello, world!" "Unclosed string
// This is a comment
/* Multi-line comment
still inside /
y = "escaped "quotes" inside";
/ Unterminated comment starts here...

Token: NUMBER | Lexeme: '123' | Line: 1
Token: OPERATOR | Lexeme: '+' | Line: 1
Token: NUMBER | Lexeme: '456' | Line: 1
Token: OPERATOR | Lexeme: '-' | Line: 1
Token: NUMBER | Lexeme: '789' | Line: 1
Token: KEYWORD | Lexeme: 'if' | Line: 1
Token: KEYWORD | Lexeme: 'repeat' | Line: 2
Token: KEYWORD | Lexeme: 'until' | Line: 2
Token: IDENTIFIER | Lexeme: 'x' | Line: 2
Token: OPERATOR | Lexeme: '=' | Line: 2
Token: NUMBER | Lexeme: '10' | Line: 2
Token: DELIMITER | Lexeme: ';' | Line: 2
Token: IDENTIFIER | Lexeme: 'someVariable' | Line: 2
Token: IDENTIFIER | Lexeme: 'another_var' | Line: 3
Lexical Error at line 3: Invalid character '123'
Token: IDENTIFIER | Lexeme: 'abc' | Line: 3
Token: STRING | Lexeme: 'Hello, world!' | Line: 3
Lexical Error at line 4: Invalid character 'Unterminated string'
Token: IDENTIFIER | Lexeme: 'y' | Line: 7
Token: OPERATOR | Lexeme: '=' | Line: 8
Token: STRING | Lexeme: 'escaped ' | Line: 8
Token: IDENTIFIER | Lexeme: 'quotes' | Line: 8
Lexical Error at line 8: Invalid character ''
Token: STRING | Lexeme: ' inside' | Line: 8
Token: DELIMITER | Lexeme: ';' | Line: 8
Lexical Error at line 8: Invalid character 'Unterminated string'
Token: EOF | Lexeme: 'EOF' | Line: 9
```
