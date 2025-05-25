# Documentation for `atv/12-threads` - Using Threads for Parallel Search

This directory contains a C program (`finderThread.c`) that demonstrates how to use multiple threads to search for an integer within an array concurrently. This is similar to the problem in `atv/10-processos` but uses threads instead of processes.

## Concepts

### Threads
Threads (or POSIX threads, pthreads) are the smallest sequence of programmed instructions that can be managed independently by a scheduler. Multiple threads can exist within the same process and share the same memory space (unlike processes, which have separate memory spaces). This allows for easier data sharing but requires careful synchronization to avoid race conditions.

### Key Pthread Functions
*   `pthread_create(thread, attr, start_routine, arg)`: Creates a new thread.
    *   `thread`: Pointer to a `pthread_t` variable that will store the ID of the new thread.
    *   `attr`: Attributes for the new thread (e.g., stack size, scheduling policy). `NULL` for default attributes.
    *   `start_routine`: A pointer to the function that the new thread will execute. This function must take a `void*` argument and return a `void*`.
    *   `arg`: The argument to be passed to the `start_routine`.
*   `pthread_join(thread, retval)`: Waits for a specific thread to terminate.
    *   `thread`: The ID of the thread to wait for.
    *   `retval`: A pointer to a `void*` where the return value (from `pthread_exit` or `return` in the thread function) will be stored.
*   `pthread_exit(retval)`: Terminates the calling thread and makes `retval` available to any thread joining it.

### Global Variables and Thread Safety
Since threads within the same process share memory, global variables can be used to share data between them.
In `finderThread.c`:
*   `vetor_principal_global`: Stores the main array to be searched.
*   `valor_buscado_global`: Stores the value to look for.
*   `tamanho_vetor_global`: Size of the main array.
*   `num_total_threads_global`: Number of threads to use.

For this specific problem, where threads only *read* from these shared variables after initialization and the array content doesn't change, this usage is relatively safe. However, if threads were modifying shared data, proper synchronization mechanisms (like mutexes) would be crucial to prevent race conditions.

## `finderThread.c` Program Explained

The `finderThread.c` program searches for a specific integer `x` within a global integer array `A`. It divides the search task among `k` threads.

### Program Logic

1.  **Input**:
    *   The program first prompts the user for:
        *   The integer element to be searched (`valor_a_buscar_local`).
        *   The total number of elements in the array (`tamanho_vetor_local`).
        *   The `n` integer elements of the array (`vetor_A_local`).
        *   The number of threads to create for the search (`k_threads_local`).
    *   It's a requirement that the array size must be divisible by the number of threads so each thread searches an equal portion.

2.  **Global Variable Initialization**:
    *   The local input values are assigned to the global variables (`vetor_principal_global`, `valor_buscado_global`, etc.) to make them accessible to the thread function.

3.  **Thread Creation**:
    *   The `main` function creates `k_threads_local` (now `num_total_threads_global`) threads using `pthread_create()`.
    *   Each thread is passed its logical ID (0, 1, 2, ...) as an argument to its start routine (`thread_busca`). This ID is used to determine its assigned search region.
    *   The region size per thread is `tamanho_regiao_por_thread = tamanho_vetor_global / num_total_threads_global`.
        *   Thread 0 searches indices `0` to `tamanho_regiao_por_thread - 1`.
        *   Thread 1 searches indices `tamanho_regiao_por_thread` to `2 * tamanho_regiao_por_thread - 1`.
        *   And so on.

4.  **Thread Behavior (`thread_busca` function)**:
    *   **Argument**: Receives its logical thread ID (`thread_id`) as a `void*`, which is cast to `long`.
    *   **Delay**: Each thread first waits for 5 seconds using `sleep(5)`.
    *   **Search**: The thread calculates its start and end indices based on its `thread_id` and `tamanho_regiao_por_thread`. It then searches its assigned region for the first occurrence of `valor_buscado_global`.
    *   **Return Value**:
        *   If the value is found at a global index `i` within the array, the thread terminates by calling `pthread_exit((void*)(long)i)`. The index `i` is cast to `void*`.
        *   If the value is not found in its region, the thread terminates by calling `pthread_exit((void*)-1L)`. The value `-1` (cast to `void*`) indicates "not found."

5.  **Main Function (`main`) Behavior After Thread Creation**:
    *   After creating all threads, the `main` function waits for each thread to complete using `pthread_join()`.
    *   For each terminated thread:
        *   It retrieves the thread's return value (which was passed to `pthread_exit`). This `void*` is cast back to a `long` to get the index or -1.
        *   Prints a message indicating which thread finished and what value it returned.
    *   The `main` function keeps track of the lowest index found across all threads that successfully found the element.
    *   After all threads have been joined:
        *   If an element was found, it prints a message like "Elemento X encontrado pela thread Y no indice Z!".
        *   If no thread found the element, it prints "Elemento nao encontrado!".
    *   Finally, it frees allocated memory (for the array and thread IDs).

## Compilation and Execution

### Compilation
To compile the program, you need to link against the pthreads library (`-pthread`):
```bash
gcc finderThread.c -o finderThread -pthread -Wall -Og -g
```
(The `README.md` provides: `gcc finderThread.c -o finderThread -pthread`)

### Running the Program
Execute the compiled program:
```bash
./finderThread
```
The program will then prompt for input interactively.

### Using Input Redirection
For testing, you can use the input files provided in the `in/` directory:
```bash
./finderThread < in/in01.txt
```

### Example from `README.md`
Given input (e.g., from `in/in01.txt`):
```
200  // Element to search
4    // Array size
100  // Array elements
200
300
400  // Note: The README example showed "200" as the last element, but the code's output implies it was 400 for child 1 to not find 200. Assuming array is [100, 200, 300, 400] for this example.
2    // Number of threads
```
Expected output sequence (order of thread completion might vary slightly, but final result line is key):
```
Qual o elemento a ser buscado? 200
Qual o tamanho do array? 4
Digite os 4 elementos do array:
100 200 300 400
Qual a quantidade de threads? 2
Criou thread 0, r=[0,2]
Criou thread 1, r=[2,4]
Funcao main() espera as threads finalizarem...
Thread 1 finalizou retornando -1          // Thread 1 (searches [300, 400])
Thread 0 finalizou retornando 1          // Thread 0 (searches [100, 200], finds 200 at global index 1)
Elemento 200 encontrado pela thread 0 no indice 1!
```

### `finderThread-gabarito`
A pre-compiled solution `finderThread-gabarito` is provided. You can compare your program's output with the expected output:
```bash
./finderThread < in/in02.txt
./finderThread-gabarito < in/in02.txt
```
The last line of output from both should match.

This program illustrates the use of threads for parallelizing tasks, data sharing via global variables (with caution), and basic thread management with `pthread_create`, `pthread_join`, and `pthread_exit`.
```
