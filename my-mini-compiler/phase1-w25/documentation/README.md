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
if x = 10;
y = "Hello, world!";
// This is a comment
/* Multi-line comment */
```
Expected Output:
```
Token: KEYWORD | Lexeme: 'if'
Token: IDENTIFIER | Lexeme: 'x'
Token: OPERATOR | Lexeme: '='
Token: NUMBER | Lexeme: '10'
Token: DELIMITER | Lexeme: ';'
Token: IDENTIFIER | Lexeme: 'y'
Token: OPERATOR | Lexeme: '='
Token: STRING | Lexeme: 'Hello, world!'
Token: DELIMITER | Lexeme: ';'
Token: EOF | Lexeme: 'EOF'
```
