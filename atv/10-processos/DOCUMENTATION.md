# Documentation for `atv/10-processos` - Using Processes for Parallel Search

This directory contains a C program (`finder.c`) that demonstrates how to use multiple processes to search for an integer within an array concurrently.

## Concepts

### Processes
In Unix-like operating systems, a process is an instance of a running program. Each process has its own independent memory space, program counter, and resources. Processes can create new processes (child processes) using system calls like `fork()`.

### `fork()` System Call
The `fork()` system call creates a new process, called a child process, which is an almost exact copy of the calling process (the parent process).
*   After `fork()`, both parent and child processes continue execution from the instruction following the `fork()` call.
*   `fork()` returns:
    *   `0` in the child process.
    *   The Process ID (PID) of the newly created child process in the parent process.
    *   `-1` if the fork failed.

### Inter-Process Communication (IPC) via Exit Status
One simple way for a child process to communicate a result back to its parent is through its exit status.
*   `exit(status)`: A child process terminates and returns an integer `status` (0-255) to the operating system.
*   `wait(&status_var)` or `waitpid(pid, &status_var, options)`: The parent process can wait for a child to terminate and retrieve its status.
    *   `WIFEXITED(status_var)`: Macro that returns true if the child terminated normally (via `exit()` or `return` from `main`).
    *   `WEXITSTATUS(status_var)`: Macro that returns the exit status of the child if `WIFEXITED` is true. The value is an 8-bit integer (0-255).

## `finder.c` Program Explained

The `finder.c` program searches for a specific integer `x` within a larger integer array `A`. It divides the search task among `k` child processes.

### Program Logic

1.  **Input**:
    *   The program first prompts the user for:
        *   `x`: The integer element to be searched.
        *   `n`: The total number of elements in the array.
        *   The `n` integer elements of the array.
        *   `k`: The number of child processes to create for the search.
    *   It's a requirement that `n` must be divisible by `k` (`n % k == 0`), so each child searches an equal portion of the array.

2.  **Process Creation**:
    *   The parent process creates `k` child processes using `fork()`.
    *   Each child process is assigned a specific region of the array to search. The region size is `r = n / k`.
        *   Child 0 searches indices `0` to `r-1`.
        *   Child 1 searches indices `r` to `2r-1`.
        *   And so on.

3.  **Child Process Behavior**:
    *   **Delay**: Each child process first waits for 5 seconds using `sleep(5)`. This is likely to simulate work or to make concurrent execution more observable.
    *   **Search**: The child searches its assigned region for the first occurrence of `x`.
    *   **Exit Status**:
        *   If `x` is found at a *local* index `j` within its region (where `0 <= j < r`), the child process exits with status `j`.
        *   If `x` is not found in its region, the child process exits with status `255` (defined as `NOT_FOUND`). The value 255 is chosen because exit statuses are 8-bit, and this is a common way to indicate a special value different from a valid index (assuming regions are smaller than 255 elements).

4.  **Parent Process Behavior**:
    *   After creating child processes, the parent waits for each child to terminate using `wait()`.
    *   For each terminated child, the parent:
        *   Retrieves the child's PID and exit status.
        *   Determines which child process it was (based on PID).
        *   Converts the child's exit status (local index or `NOT_FOUND`) back to a meaningful result:
            *   If the status is `NOT_FOUND` (255), the result for that child is -1.
            *   Otherwise, the status is a local index `j`. The parent converts this to a global index in the original array: `global_index = (child_process_number * region_size) + local_index`.
        *   Prints a message indicating which child finished and what local index (or -1) it returned.
    *   After all children have finished, the parent checks the results.
    *   If any child found the element, the parent reports the *first* such occurrence (i.e., the one found by the child process with the smallest ID, or the smallest global index if multiple children find it at different global indices but the problem asks for the one reported by the numerically first process). The example output suggests it prioritizes the result from the child with the lower process index (e.g., "process 0").
    *   If no child found the element, it prints "Elemento não encontrado!".
    *   The parent process must always `return 0;`.

### Key Functions Used

*   `scanf()`: To read user input.
*   `printf()`: To display messages.
*   `malloc()`: To allocate memory for the array and process ID storage.
*   `free()`: To release allocated memory.
*   `fork()`: To create child processes.
*   `sleep()`: To pause execution in child processes.
*   `exit()`: For child processes to terminate and return a status.
*   `wait()`: For the parent process to wait for child processes.
*   `WIFEXITED()`, `WEXITSTATUS()`: To interpret the child's exit status.

## Compilation and Execution

### Compilation
The program can be compiled using the provided `Makefile`:
```bash
make
```
This typically executes a command like:
```bash
gcc -Wall -Og -g finder.c -o finder
```

### Running the Program
Execute the compiled program:
```bash
./finder
```
The program will then prompt for input interactively.

### Using Input Redirection
For testing, it's convenient to use input files. The `in/` directory contains several test files (`in01.txt`, `in02.txt`, etc.).
```bash
./finder < in/in01.txt
```
This will feed the contents of `in01.txt` as standard input to the `finder` program.

### Example from `README.md`
Given input:
```
200  // Element to search
4    // Array size
100  // Array elements
200
300
400
2    // Number of processes
```
Expected output sequence (order of child completion might vary slightly, but final result line is key):
```
Qual o elemento a ser buscado? 200
Qual o tamanho do array? 4
Digite os 4 elementos do array:
100 200 300 200
Qual a quantidade de processos? 2
Criou filho 0 com PID=XXXXX, r=[0,2)  // XXXXX will be an actual PID
Criou filho 1 com PID=YYYYY, r=[2,4)  // YYYYY will be an actual PID
Processo pai esperando os filhos finalizarem...
Processo 1 com PID=YYYYY finalizou retornando -1 // Child 1 (searches [300, 400])
Processo 0 com PID=XXXXX finalizou retornando 1  // Child 0 (searches [100, 200], finds 200 at its local index 1)
Elemento 200 encontrado pelo processo 0 no indice 1! // Global index is (0 * 2) + 1 = 1
```

### `finder-gabarito`
A pre-compiled solution `finder-gabarito` is provided. You can compare your program's output with the expected output:
```bash
./finder < in/in02.txt
./finder-gabarito < in/in02.txt
```
The last line of output from both should match.

This program serves as a good introduction to multi-process programming, demonstrating process creation, basic synchronization with `wait()`, and simple inter-process communication using exit statuses.
```
