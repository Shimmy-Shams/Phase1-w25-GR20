//
// Created by Youssef
//

/* tokens.h */
#ifndef TOKENS_H
#define TOKENS_H

/* Token types that need to be recognized by the lexer
 * TODO: Add more token types as per requirements:
 * - Keywords or reserved words (if, repeat, until)
 * - Identifiers
 * - String literals
 * - More operators
 * - Delimiters
 */
typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,     // e.g., "123", "456"
    TOKEN_OPERATOR,   // e.g., "+", "-"
    TOKEN_KEYWORD,    // e.g., "if", "repeat", "until"
    TOKEN_IDENTIFIER, // e.g., variable names: "x", "y", "someVar"
    TOKEN_STRING,     // e.g., "hello world"
    TOKEN_DELIMITER,  // e.g., ";", "(", ")"
    TOKEN_COMMENT,    // e.g., "// comment"
    TOKEN_ERROR
} TokenType;


/* Error types for lexical analysis
 * TODO: Add more error types as needed for your language - as much as you like !!
 */
typedef enum {
    ERROR_NONE,
    ERROR_INVALID_CHAR,
    ERROR_INVALID_NUMBER,
    ERROR_CONSECUTIVE_OPERATORS
} ErrorType;

/* Token structure to store token information
 * TODO: Add more fields if needed for your implementation
 * Hint: You might want to consider adding line and column tracking if you want to debug your lexer properly.
 * Don't forget to update the token fields in lexer.c as well
 */
typedef struct {
    TokenType type;     // Token type, defines what behaviors should be considered during AST generation
    char* lexeme;       // Actual text of the token
    unsigned int size;  // Size of lexeme, used in dynamic allocation
    int line;           // Line number in source file
    ErrorType error;    // Error type if any
} Token;

#endif /* TOKENS_H */
