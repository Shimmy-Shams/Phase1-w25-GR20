# **Lexical Analyzer - Error Handling and Design Decisions**

## **4. Error Codes and Handling**

The error handling in this lexer is designed to detect and report lexical issues while scanning the input. When an error occurs, such as an invalid character, consecutive operators, or an unterminated string, the function print_error is called to display a message and the line number. The lexer also dynamically allocates memory for error messages, ensuring that errors like unterminated strings or comments are identified. Line tracking is implemented to help locate errors accurately, updating the current_line variable whenever a newline character is encountered. Additionally, for invalid tokens, the lexer attempts to continue scanning rather than halting immediately, allowing it to detect multiple errors in a single pass.

The lexer reports the following lexical errors:
| Error Type                     | Description                                      |
|--------------------------------|--------------------------------------------------|
| ERROR_INVALID_CHAR            | Invalid character detected in input.            |
| ERROR_INVALID_NUMBER          | Incorrect number format (not applicable yet).   |
| ERROR_CONSECUTIVE_OPERATORS   | Consecutive operators detected (e.g., ++).      |
| ERROR_UNTERMINATED_STRING     | String is missing a closing quote (").          |
| ERROR_UNTERMINATED_COMMENT    | Multi-line comment is missing a closing */.    |
