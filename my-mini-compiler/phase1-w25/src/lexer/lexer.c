/* lexer.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../../include/tokens.h"

// Line tracking
static int current_line = 1;
static char last_token_type = 'x'; // For checking consecutive operators

/* Print error messages for lexical errors */
void print_error(ErrorType error, int line, const char *lexeme) {
    printf("Lexical Error at line %d: ", line);
    switch (error) {
        case ERROR_INVALID_CHAR:
            printf("Invalid character '%s'\n", lexeme);
            break;
        case ERROR_INVALID_NUMBER:
            printf("Invalid number format\n");
            break;
        case ERROR_CONSECUTIVE_OPERATORS:
            printf("Consecutive operators not allowed\n");
            break;
        default:
            printf("Unknown error\n");
    }
}

/* Print token information
 *
 *  TODO Update your printing function accordingly
 */

void print_token(Token token) {
    if (token.error != ERROR_NONE) {
        print_error(token.error, token.line, token.lexeme);
        return;
    }

    printf("Token: ");
    switch (token.type) {
        case TOKEN_NUMBER:
            printf("NUMBER");
            break;
        case TOKEN_OPERATOR:
            printf("OPERATOR");
            break;
        case TOKEN_KEYWORD:
            printf("KEYWORD");
            break;
        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case TOKEN_STRING:
            printf("STRING");
            break;
        case TOKEN_DELIMITER:
            printf("DELIMITER");
            break;
        case TOKEN_COMMENT:
            printf("COMMENT");
            break;
        case TOKEN_EOF:
            printf("EOF");
            break;
        default:
            printf("UNKNOWN");
    }
    printf(" | Lexeme: '%s' | Line: %d\n",
           token.lexeme, token.line);
}


// List of keywords
const char *keywords[] = {"if", "repeat", "until", NULL};

// Function to check if a word is a keyword
int is_keyword(const char *word) {
    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1; // It's a keyword
        }
    }
    return 0; // Not a keyword
}

// Get next token from input
Token get_next_token(const char *input, int *pos) {
    Token token = {TOKEN_ERROR, "", current_line, ERROR_NONE};
    char c;

    // Skip whitespace and track line numbers
    while ((c = input[*pos]) != '\0' && (c == ' ' || c == '\n' || c == '\t')) {
        if (c == '\n') {
            current_line++;
        }
        (*pos)++;
    }

    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }

    c = input[*pos];

    // Handle numbers (detect invalid identifiers like "123abc")
    if (isdigit(c)) {
        int i = 0;
        do {
            token.lexeme[i++] = c;
            (*pos)++;
            c = input[*pos];
        } while (isdigit(c) && i < sizeof(token.lexeme) - 1);

        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;

        // If next character is alphabetic, mark as error (invalid identifier)
        if (isalpha(input[*pos])) {
            token.error = ERROR_INVALID_CHAR;
        }
        return token;
    }

    // Handle keywords and identifiers
    if (isalpha(c)) {
        int i = 0;
        do {
            token.lexeme[i++] = c;
            (*pos)++;
            c = input[*pos];
        } while ((isalnum(c) || c == '_') && i < sizeof(token.lexeme) - 1);

        token.lexeme[i] = '\0';

        // Check if it's a keyword or identifier
        if (is_keyword(token.lexeme)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    // Handle operators including '='
    if (c == '+' || c == '-' || c == '=') {
        token.type = TOKEN_OPERATOR;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        (*pos)++;
        return token;
    }

    // Handle string literals
    if (c == '"') {
        int i = 0;
        token.type = TOKEN_STRING;
        (*pos)++; // Move past opening quote

        while (input[*pos] != '"' && input[*pos] != '\0' && i < sizeof(token.lexeme) - 2) {
            if (input[*pos] == '\\' && (input[*pos + 1] == '"' || input[*pos + 1] == '\\')) {
                // Handle escape sequences (e.g., \" or \\)
                token.lexeme[i++] = input[*pos];
                (*pos)++;
            }
            token.lexeme[i++] = input[*pos];
            (*pos)++;

            if (input[*pos] == '\n') {
                token.error = ERROR_INVALID_CHAR; // Strings shouldn't span multiple lines
                strcpy(token.lexeme, "Unterminated string");
                return token;
            }
        }

        if (input[*pos] == '"') {
            token.lexeme[i] = '\0';
            (*pos)++; // Move past closing quote
        } else {
            token.error = ERROR_INVALID_CHAR; // Unterminated string
            strcpy(token.lexeme, "Unterminated string");
        }
        return token;
    }


    // Handle delimiters (e.g., ';')
    if (c == ';') {
        token.type = TOKEN_DELIMITER;
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
        (*pos)++;
        return token;
    }

    // Handle single-line comments "//"
    if (c == '/' && input[*pos + 1] == '/') {
        (*pos) += 2; // Skip "//"
        while (input[*pos] != '\n' && input[*pos] != '\0') {
            (*pos)++;
        }
        return get_next_token(input, pos); // Skip comment and continue scanning
    }

    // Handle multi-line comments "/* ... */"
    if (c == '/' && input[*pos + 1] == '*') {
        (*pos) += 2; // Skip "/*"

        while (input[*pos] != '\0') {
            if (input[*pos] == '*' && input[*pos + 1] == '/') {
                (*pos) += 2; // Move past "*/"
                return get_next_token(input, pos); // Skip comment and continue scanning
            }

            if (input[*pos] == '\n') {
                current_line++;
            }
            (*pos)++;
        }

        // If we reach here, the comment was never closed
        token.error = ERROR_INVALID_CHAR;
        strcpy(token.lexeme, "Unterminated comment");
        return token;
    }

    // Handle invalid characters
    token.error = ERROR_INVALID_CHAR;
    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    (*pos)++;
    return token;
}

// This is a basic lexer that handles numbers (e.g., "123", "456"), basic operators (+ and -), consecutive operator errors, whitespace and newlines, with simple line tracking for error reporting.

int main() {
    const char *input = "123 + 456 - 789\n"
                        "if repeat until x = 10;\n"
                        "someVariable another_var 123abc\n"
                        "\"Hello, world!\" \"Unclosed string\n"
                        "// This is a comment\n"
                        "/* Multi-line comment\nstill inside */\n"
                        "y = \"escaped \\\"quotes\\\" inside\";\n"
                        "/* Unterminated comment starts here...";

    int position = 0;
    Token token;

    printf("Analyzing input:\n%s\n\n", input);

    do {
        token = get_next_token(input, &position);
        print_token(token);
    } while (token.type != TOKEN_EOF);

    return 0;
}