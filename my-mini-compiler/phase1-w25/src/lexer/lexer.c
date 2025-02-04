/* lexer.c */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../../include/tokens.h"

// Line tracking
static int current_line = 1;
static char last_token_type = 'x'; // For checking consecutive operators

// TODO: we need a better handler, tokenizers/lexers should have their own class tbh
// This way it would be easier to manage like creating a simple iterator for tokens

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
    Token token;
    token.type = TOKEN_ERROR;
    token.lexeme = NULL;
    token.size = 0;
    token.line = current_line;
    token.error = ERROR_NONE;

    char c;

    // Skip whitespace and track line numbers
    while ((c = input[*pos]) != '\0' && (c == ' ' || c == '\n' || c == '\t')) {
        if (c == '\n') {
            current_line++;
        }
        (*pos)++;
    }

    // "EOF" + null terminator, so size is "EOF" + 1
    if (input[*pos] == '\0') {
        token.type = TOKEN_EOF;
        token.size = 4;
        token.lexeme = malloc(token.size);
        if (token.lexeme) strcpy(token.lexeme, "EOF");
        return token;
    }

    c = input[*pos];

    // Handle numbers (detect invalid identifiers like "123abc")
    // I made it so that it dynamic allocates large numbers as well
    // We can define later on during symantics if the numbers are too large for a datatype
    if (isdigit(c)) {
        int start = *pos;
        while (isdigit(input[*pos])) (*pos)++;

        token.size = (*pos - start) + 1;
        token.lexeme = malloc(token.size);
        if (token.lexeme) strncpy(token.lexeme, &input[start], token.size - 1);
        token.lexeme[token.size - 1] = '\0';
        token.type = TOKEN_NUMBER;

        // If next character is alphabetic, mark as error (invalid identifier)
        if (isalpha(input[*pos])) {
            token.error = ERROR_INVALID_CHAR;
        }
        
        return token;
    }

    // Handle keywords and identifiers
    if (isalpha(c)) {
        int start = *pos;
        while (isalnum(input[*pos]) || input[*pos] == '_') (*pos)++;

        token.size = (*pos - start) + 1;
        token.lexeme = malloc(token.size);
        if (token.lexeme) strncpy(token.lexeme, &input[start], token.size - 1);
        token.lexeme[token.size - 1] = '\0';

        token.type = is_keyword(token.lexeme) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;
        return token;
    }

    // Handle operators including '='
    if (c == '+' || c == '-' || c == '=') {
        token.size = 2;
        token.lexeme = malloc(token.size);
        if (token.lexeme) {
            token.lexeme[0] = c;
            token.lexeme[1] = '\0';
        }
        token.type = TOKEN_OPERATOR;
        (*pos)++;
        return token;
    }

    // Handle string literals
    if (c == '"') {
        (*pos)++;
        int start = *pos;
        while (input[*pos] != '"' && input[*pos] != '\0' && input[*pos] != '\n') (*pos)++;

        if (input[*pos] == '"') {
            token.size = (*pos - start) + 1;
            token.lexeme = malloc(token.size);
            if (token.lexeme) strncpy(token.lexeme, &input[start], token.size - 1);
            token.lexeme[token.size - 1] = '\0';
            (*pos)++;
            token.type = TOKEN_STRING;
        } else {
            token.error = ERROR_INVALID_CHAR;
            token.size = 20;
            token.lexeme = malloc(token.size);
            if (token.lexeme) strcpy(token.lexeme, "Unterminated string");
        }
        return token;
    }


    // Handle delimiters (e.g., ';')
    if (c == ';') {
        token.type = TOKEN_DELIMITER;
        token.size = 2;
        token.lexeme = malloc(token.size);
        if (token.lexeme) {
            token.lexeme[0] = c;
            token.lexeme[1] = '\0';
        }
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
        token.size = 20;
        token.lexeme = malloc(token.size);
        if (token.lexeme) strcpy(token.lexeme, "Unterminated string");
        return token;
    }

    // Handle invalid characters
    token.error = ERROR_INVALID_CHAR;
    token.size = 2;
    token.lexeme = malloc(token.size);
    if (token.lexeme) {
        token.lexeme[0] = c;
        token.lexeme[1] = '\0';
    }
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