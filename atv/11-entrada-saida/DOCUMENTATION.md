# Documentation for `atv/11-entrada-saida` - File I/O and Comment Removal

This directory contains a C program (`tirar_comentarios.c`) that demonstrates file input/output operations by reading a C source file and writing a new file with all comments (both single-line and multi-line) removed.

## Concepts

### File Input/Output (I/O) in C
Standard C provides several ways to interact with files. This program uses low-level POSIX file I/O, which involves:
*   **File Descriptors**: Integer values that uniquely identify an open file within a process.
*   `open(pathname, flags, mode)`: System call to open or create a file.
    *   `pathname`: Path to the file.
    *   `flags`: Specify access mode (read-only, write-only, read-write) and other options (create if not exists, truncate, etc.). Examples: `O_RDONLY`, `O_WRONLY`, `O_CREAT`, `O_TRUNC`.
    *   `mode`: Permissions for the new file if `O_CREAT` is used (e.g., `0700` for read/write/execute for owner).
    *   Returns a file descriptor on success, -1 on error.
*   `read(fd, buf, count)`: System call to read up to `count` bytes from the file descriptor `fd` into the buffer `buf`.
    *   Returns the number of bytes read (can be less than `count` if end-of-file is reached or an error occurs). Returns 0 at end-of-file, -1 on error.
*   `write(fd, buf, count)`: System call to write `count` bytes from the buffer `buf` to the file descriptor `fd`.
    *   Returns the number of bytes written (can be less than `count` if an error occurs). Returns -1 on error.
*   `close(fd)`: System call to close an open file descriptor, releasing it for reuse.

### State Machine
A state machine is a computational model consisting of a set of states, transitions between those states, and actions performed based on the current state and input. It's a useful way to design parsers and lexical analyzers.

## `tirar_comentarios.c` Program Explained

The `tirar_comentarios.c` program reads an input C source file character by character, processes it using a state machine to identify and discard comments, and writes the non-comment content to an output file.

### Command-Line Arguments
The program expects two command-line arguments:
1.  `argv[1]`: The path to the input C source file (e.g., `caso1.c`).
2.  `argv[2]`: The path to the output file where the comment-free code will be written (e.g., `resp_caso1.c`).

If the wrong number of arguments is provided, the program exits.

### State Machine Logic
The core of the program is a state machine implemented with a `switch` statement inside a `while` loop that reads the input file one byte at a time. The `estado` variable holds the current state.

The states are (as inferred from the code and the diagram in `README.md`):

*   **State 0: Normal (Initial/Default)**
    *   This is the default state where characters are considered part of the code.
    *   If the character `c` is `/`, it might be the start of a comment, so transition to **State 1**.
    *   Otherwise, write `c` to the output file and remain in **State 0**.

*   **State 1: Encountered `/`**
    *   The previous character was `/`.
    *   If `c` is `/`, it's a single-line comment (`//`). Transition to **State 2**.
    *   If `c` is `*`, it's the start of a multi-line comment (`/*`). Transition to **State 3**.
    *   Otherwise, the previous `/` was not part of a comment (e.g., division operator). Write the stored `/` and the current character `c` to the output file. Transition back to **State 0**.

*   **State 2: Inside Single-Line Comment (`// ...`)**
    *   Characters are ignored until a newline (`\n`) is found.
    *   If `c` is `\n`, write `\n` to the output file (to preserve line structure) and transition back to **State 0**.
    *   Otherwise, stay in **State 2** (discarding the character).

*   **State 3: Inside Multi-Line Comment (`/* ... */`)**
    *   Characters are ignored until a `*` is found.
    *   If `c` is `*`, it might be the end of the comment. Transition to **State 4**.
    *   Otherwise, stay in **State 3** (discarding the character).

*   **State 4: Encountered `*` inside Multi-Line Comment**
    *   The previous character inside a multi-line comment was `*`.
    *   If `c` is `/`, the multi-line comment (`*/`) has ended. Transition back to **State 0**.
    *   If `c` is `*` again, remain in **State 4** (e.g., `/** ... */`).
    *   Otherwise (any other character), the previous `*` was not part of `*/`. Transition back to **State 3** (still inside the multi-line comment).

### End of File Handling
*   After the `while` loop finishes (either due to end-of-file or a read error):
    *   If `nread == -1`, an error occurred during reading, so an error message is printed.
    *   A special check: if the loop ended while in `estado == 1` (meaning a `/` was the last character of the file), that `/` is written to the output. This handles cases like a file ending with a single `/`.

### File Operations
1.  **Open Input File**: `fd1 = open(argv[1], O_RDONLY);`
    *   Opens the source file in read-only mode.
2.  **Open Output File**: `fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0700);`
    *   Opens the destination file in write-only mode.
    *   `O_CREAT`: Creates the file if it doesn't exist.
    *   `O_TRUNC`: Truncates the file to zero length if it already exists (overwrites).
    *   `0700`: Sets file permissions (read, write, execute for owner; none for group/others).
3.  **Read Loop**: `while ((nread = read(fd1, buf, 1)) > 0)`
    *   Reads one byte at a time into `buf`.
4.  **Write**: `write(fd2, buf, 1);`
    *   Writes the processed byte (if not part of a comment) to the output file.
5.  **Close Files**: `close(fd1); close(fd2);`
    *   Closes both file descriptors after processing.

## Compilation and Testing

### Compilation
Compile the program using GCC (no special libraries needed beyond standard C and POSIX):
```bash
gcc tirar_comentarios.c -o tirar_comentarios -Wall -g
```

### Running the Program
To remove comments from `input.c` and save the result to `output.c`:
```bash
./tirar_comentarios input.c output.c
```
The directory includes test cases like `caso1.c`, `caso2.c`, etc.
For example:
```bash
./tirar_comentarios caso1.c resp_caso1.c
```
You can then inspect `resp_caso1.c` to see if comments were removed correctly.

### `teste.sh` Script
The `teste.sh` script is provided to automate testing with the given `caso*.c` files. It likely compiles `tirar_comentarios.c` and then runs it against each test case, possibly comparing the output to expected results (though the comparison logic isn't detailed in the README, it might just generate the files).
To run the script:
```bash
./teste.sh
```
Make sure it has execute permissions (`chmod +x teste.sh`).

### `gabarito.c`
The `gabarito.c` file seems to be a simple C file, possibly used as one of the test cases or as an example of a file that would be processed. The content `printf("Alô, mundo!! %d \n", 10/2);` is a simple print statement.

This program is a practical example of character-by-character file processing and implementing a simple parser using a state machine.
```
