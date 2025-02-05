# **Lexical Analyzer - Error Handling and Design Decisions**

## **4. Error Codes and Handling**

The lexer reports the following lexical errors:
| Error Type                     | Description                                      |
|--------------------------------|--------------------------------------------------|
| ERROR_INVALID_CHAR            | Invalid character detected in input.            |
| ERROR_INVALID_NUMBER          | Incorrect number format (not applicable yet).   |
| ERROR_CONSECUTIVE_OPERATORS   | Consecutive operators detected (e.g., ++).      |
| ERROR_UNTERMINATED_STRING     | String is missing a closing quote (").          |
| ERROR_UNTERMINATED_COMMENT    | Multi-line comment is missing a closing */.    |
