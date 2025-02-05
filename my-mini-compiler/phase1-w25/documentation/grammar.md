## Token Grammar

The lexer implementation follows a structured token grammar, categorizing input into the following types:

- **Keywords**: Keywords such as `if`, `repeat`, and `until` are recognized using a lookup function.
- **Identifiers**: Identifiers must start with a letter and may include digits and underscores.
- **Numbers**: Numbers are processed as sequences of digits, with error handling to prevent invalid identifiers (`123abc`).
- **Operators**: Operators are categorized into:
  - Arithmetic: `+`, `-`, `*`, `/`
  - Assignment: `=`
  - Multi-character relational operators (`==`, `!=`, `<=`, `>=`) are not yet supported.
- **String Literals**: The lexer correctly processes string literals with escape sequences.
- **Delimiters**: Delimiters are handled appropriately.
- **Comments**: Comments are skipped during tokenization.

The lexer recognizes the following token types:

### 2.1 Keywords
- `if`
- `repeat`
- `until`

### 2.2 Identifiers
- A valid identifier starts with a letter (`a-z`, `A-Z`) and can contain letters, digits (`0-9`), and underscores (`_`).
- **Example:** `var_name`, `x1`, `someVar`

### 2.3 Numbers
- A number consists of one or more digits (`0-9`).
- **Example:** `123`, `4567`

### 2.4 Operators
- **Arithmetic Operators:** `+`, `-`, `*`, `/`
- **Assignment Operator:** `=`
- **Relational Operators** *(planned for future phases)*: `==`, `!=`, `<=`, `>=`

### 2.5 String Literals
- Strings are enclosed in double quotes (`" "`).
- Supports escape sequences: `\"`, `\\`, `\n`
- **Example:** `"Hello, world!"`

### 2.6 Delimiters
- `;`, `{`, `}`, `(`, `)`

### 2.7 Comments
- **Single-line comments:** `// This is a comment`
- **Multi-line comments:** `/* This is a multi-line comment */`
