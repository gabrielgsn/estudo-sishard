// Leia o enunciado antes de iniciar este exercício!

// File-level overview:
// This program reads words from an input file specified via command-line argument.
// For each word, it creates a thread to classify the word into one of three categories (0, 1, or 2).
// A semaphore is used to limit the number of threads that can run the classification logic concurrently,
// with the maximum concurrency level also specified as a command-line argument.
// After all words are processed, the program prints a summary showing how many words
// were assigned to each category. The program dynamically allocates memory for thread IDs
// and their arguments, reallocating as needed if the number of words exceeds initial estimates.

// inclua libs aqui!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> // Para open() e flags
#include <string.h> // Para strlen()
#include <pthread.h> // Para pthreads
#include <semaphore.h> // Para semáforos

// Global semaphore:
// sem: This semaphore is used to limit the number of threads that can concurrently
// execute the classifier_thread function. Its initial count is set to the
// number of desired concurrent threads (n_threads from command line).
// Each thread calls sem_wait() before entering the critical classification section
// and sem_post() after leaving it.
sem_t sem;

// t_arg struct: Defines the structure for arguments passed to each classifier thread.
typedef struct
{
    char *palavra; // Field to hold the word (string) to be classified by the thread.
    int res;       // Field to store the classification result (0, 1, or 2) produced by the thread.
} t_arg;

// MAX_LINE_LENGTH: Defines the maximum expected length for a line (word) read from
// the input file, including space for the null terminator.
#define MAX_LINE_LENGTH 256

/*
 * Função que simula a classificação de uma palavra
 * classifier: Simulates a word classification task.
 *
 * Parameters:
 *   palavra (char *): The word string to be classified.
 *
 * Returns:
 *   int: An integer representing the classification category (0, 1, or 2).
 *
 * Behavior:
 *   - It introduces a random delay (sleep) to simulate processing time.
 *   - It calculates a sum based on the ASCII values of the characters in the word.
 *   - The final classification is derived from (sum % 3).
 *   - For the purpose of this program (q4.c), the internal logic of classifier()
 *     is treated as a black box; the main focus is on threading and synchronization.
 */
int classifier(char *palavra)
{
    // Simulate processing time with a random sleep duration between 4 and 4 seconds (inclusive).
    // Note: rand() % 1 always results in 0, so sleep_time is effectively always 4.
    // To achieve a range, it should be rand() % (max_val - min_val + 1) + min_val.
    // For example, rand() % 1 + 4 means rand() % (4-4+1) + 4 = rand() % 1 + 4.
    // If the intent was 1 to 4 seconds, it would be rand() % 4 + 1.
    // As it is, it's always 4 seconds.
    int sleep_time = rand() % 1 + 4;

    // Espera por uma qtde aleatória de segundos
    sleep(sleep_time);

    int soma = 157; // Initial base value for sum.

    // Iterate through the word, adding ASCII values of characters to 'soma'.
    for (int i = 0; palavra[i] != '\0'; i++)
    {
        soma += palavra[i];
    }

    // The classification result is 'soma' modulo 3.
    int res = (soma % 3);

    return res;
}

/*
 * Classificação em Threads!
 * classifier_thread: This is the function executed by each worker thread.
 *
 * Parameters:
 *   _arg (void *): A void pointer to a t_arg structure. This structure contains
 *                  the word to be classified and a field to store the result.
 *
 * Behavior:
 *   1. Waits on the global semaphore 'sem'. If the semaphore's count is greater
 *      than zero, it decrements the count and proceeds. If the count is zero,
 *      the thread blocks until another thread posts to the semaphore. This limits
 *      the number of threads concurrently executing the classifier.
 *   2. Calls the `classifier` function to perform the actual classification on
 *      the word stored in `arg->palavra`. The result is stored in `arg->res`.
 *   3. Posts to the global semaphore 'sem', incrementing its count. This allows
 *      another waiting thread (if any) to acquire the semaphore and proceed.
 *   4. Terminates the thread execution.
 */
void *classifier_thread(void *_arg)
{
    t_arg *arg = (t_arg *)_arg; // Cast the void pointer argument to the t_arg type.

    sem_wait(&sem); // Aguarda a liberação do semáforo (waits for the semaphore to be available).
                    // Decrements semaphore count or blocks if count is 0.

    arg->res = classifier(arg->palavra); // Perform classification.

    sem_post(&sem); // Libera o semáforo (releases the semaphore).
                    // Increments semaphore count, potentially unblocking another thread.

    pthread_exit(NULL); // Terminate the current thread.
    return NULL; // Added to satisfy some compilers, though pthread_exit stops execution.
}

/* Função que lê uma palavra de um arquivo.
 * Considere que cada linha do arquivo contem apenas uma palavra.
 *
 * Entradas:
 *   int fd: descritor do arquivo (file descriptor for the input file).
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida (sem \n).
 *   Caso o arquivo termine (EOF) E nenhuma character foi lido para a linha atual, retorne NULL.
 *   Caso EOF seja atingido após alguns caracteres serem lidos para a linha atual, retorna a palavra lida até então.
 *
 * Implementation details:
 *   - Uses only the read() system call for I/O.
 *   - Reads character by character until a newline ('\n') is encountered,
 *     EOF is reached, or MAX_LINE_LENGTH is exceeded.
 *   - Dynamically allocates memory for the line buffer. The caller is responsible for freeing this memory.
 */
char *read_line(int fd)
{
    // Allocate buffer for the line. +1 for the null terminator.
    char *buffer = malloc(MAX_LINE_LENGTH + 1);
    if (!buffer) { // Check if malloc failed
        perror("malloc failed in read_line");
        return NULL;
    }

    int i = 0; // Index for storing characters in the buffer.
    char c;    // Character read from the file.
    ssize_t bytes_read; // Number of bytes read by read().

    // Loop to read characters one by one.
    while (i < MAX_LINE_LENGTH)
    {
        // Read a single character from the file.
        bytes_read = read(fd, &c, 1);

        if (bytes_read == -1) { // Error during read
            perror("read failed in read_line");
            free(buffer);
            return NULL;
        }

        if (bytes_read == 0) // EOF (End Of File) reached.
        {
            if (i == 0) { // EOF reached and no characters were read for this line.
                free(buffer);
                return NULL; // Signal that there are no more lines.
            }
            else { // EOF reached after some characters were read for this line.
                break; // Terminate loop, the partially read line will be returned.
            }
        }

        if (c == '\n') // Newline character encountered.
        {
            break; // End of the current line.
        }

        buffer[i++] = c; // Store the character and increment index.
    }

    buffer[i] = '\0'; // Null-terminate the string.
    
    return buffer; // Return the pointer to the line read.
}

// main: Entry point of the program.
// Handles command-line arguments, file operations, thread creation,
// synchronization, result aggregation, and cleanup.
int main(int argc, char *argv[]){
    
    // Argument Parsing: Check if the correct number of command-line arguments is provided.
    if (argc != 3)
    {
        // Print usage message if arguments are incorrect.
        printf("USAGE:\n./q4 <n_threads> <file_name>\n");
        return 0; // Exit indicating incorrect usage.
    }

    // Convert the first command-line argument (string) to an integer for n_threads.
    // n_threads: Maximum number of threads allowed to run the classifier concurrently.
    int n_threads = atoi(argv[1]);
    if (n_threads <= 0) {
        fprintf(stderr, "Error: Number of threads must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    // Semaphore Initialization: Initialize the global semaphore 'sem'.
    // - &sem: Pointer to the semaphore object.
    // - 0: pshared argument; 0 indicates the semaphore is shared between threads of the same process.
    // - n_threads: Initial value of the semaphore, limiting concurrent access.
    sem_init(&sem, 0, n_threads);

    // Get the input filename from the second command-line argument.
    char *file_name = argv[2];

    // File Opening: Open the specified input file in read-only mode.
    int fd1 = open(file_name, O_RDONLY);

    // Error handling for file opening.
    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo! (Failed to open the file!)\n");
        perror("open"); // Print system error message for open
        exit(EXIT_FAILURE); // Exit if file cannot be opened.
    }

    // Seed the random number generator. Used by classifier() for its sleep duration.
    srand(time(NULL));

    // Thread Management Data Structures:
    // Initial allocation size for thread IDs and argument pointers.
    int threads_limit = 2; // Start by allocating space for 2 threads.
    // Dynamically allocate an array to store thread IDs (pthread_t).
    pthread_t *tids = malloc(sizeof(pthread_t) * threads_limit);
    // Dynamically allocate an array to store pointers to thread arguments (t_arg *).
    t_arg **args = malloc(sizeof(t_arg *) * threads_limit);

    if (!tids || !args) { // Check for malloc failure
        perror("malloc failed for tids or args");
        close(fd1);
        sem_destroy(&sem);
        exit(EXIT_FAILURE);
    }

    int threads_created = 0; // Counter for the number of threads actually created.

    // Word Reading and Thread Creation Loop:
    // This loop reads words from the file one by one and creates a thread for each word.
    // The loop counter 'i' also serves as an index for tids and args arrays.
    for (int i = 0;; i++)
    {
        // Attempt to read a word (line) from the file.
        char *palavra = read_line(fd1);

        // If read_line returns NULL, it means EOF was reached and no more words.
        if (palavra == NULL)
        {
            break; // Exit the loop.
        }

        // Allocate memory for the current thread's argument structure (t_arg).
        t_arg *arg = malloc(sizeof(t_arg));
        if (!arg) { // Check for malloc failure
            perror("malloc failed for t_arg");
            // Basic cleanup for already read word and previously allocated structures
            free(palavra);
            // More robust cleanup would involve joining already created threads and freeing all args
            break; 
        }
        // Assign the word read from file to the argument structure.
        arg->palavra = palavra;

        // Dynamic Reallocation: If the current number of created threads reaches the allocated limit,
        // double the size of the `tids` and `args` arrays.
        if (threads_limit == threads_created)
        {
            threads_limit *= 2;
            pthread_t *temp_tids = realloc(tids, sizeof(pthread_t) * threads_limit);
            t_arg **temp_args = realloc(args, sizeof(t_arg *) * threads_limit);

            if (!temp_tids || !temp_args) { // Check for realloc failure
                perror("realloc failed");
                // Free partially allocated memory and exit or handle error
                free(arg->palavra);
                free(arg);
                if (temp_tids) tids = temp_tids; // Keep whichever realloc might have succeeded
                if (temp_args) args = temp_args;
                // A more graceful exit would involve joining existing threads and freeing all data
                break; 
            }
            tids = temp_tids;
            args = temp_args;
        }

        // Store the pointer to the current thread's argument structure.
        args[i] = arg;

        // Create a new thread.
        // - &tids[i]: Pointer to store the ID of the new thread.
        // - NULL: Thread attributes (default attributes are used).
        // - classifier_thread: The function the new thread will execute.
        // - arg: The argument to be passed to classifier_thread.
        if (pthread_create(&tids[i], NULL, classifier_thread, arg) != 0) {
            perror("pthread_create failed");
            free(arg->palavra);
            free(arg);
            // Potentially reduce threads_created if we decide not to count this one
            break; // Stop creating threads if one fails
        }
        printf("CRIOU THREAD %02d PARA PROCESSAR A PALAVRA %s\n", i, arg->palavra);

        threads_created++; // Increment the count of successfully created threads.
    }
    // "Complete a main!" comment is addressed by the following sections.

    // Waiting for Threads and Collecting Results:
    // Array to store the counts of words in each of the 3 classification categories.
    int classes[3] = {0, 0, 0}; // classes[0], classes[1], classes[2]

    // Loop through all created threads.
    for(int i=0; i<threads_created; i++){
        // Wait for the thread with ID tids[i] to complete its execution.
        // NULL as the second argument means we are not interested in the thread's return value (via pthread_exit).
        pthread_join(tids[i], NULL);

        // Print message indicating thread completion and its classification result.
        printf("THREAD %02d TERMINOU COM O RESULTADO %d\n", i, args[i]->res);
        
        // Increment the count for the respective classification category.
        // args[i]->res will be 0, 1, or 2.
        if (args[i]->res >= 0 && args[i]->res < 3) { // Basic validation
            classes[args[i]->res]++;
        } else {
            fprintf(stderr, "Warning: Thread %02d returned invalid class %d for word %s\n", i, args[i]->res, args[i]->palavra);
        }


        // Free the memory allocated for the word string (by read_line).
        free(args[i]->palavra);
        // Free the memory allocated for the thread's argument structure.
        free(args[i]);
    }

    // Printing Results: Display the total number of words in each class.
    printf("RESULTADO:\n");
    printf("TOTAL CLASSE 0: %d\n", classes[0]);
    printf("TOTAL CLASSE 1: %d\n", classes[1]);
    printf("TOTAL CLASSE 2: %d\n", classes[2]);

    // Cleanup:
    // Free the dynamically allocated array for thread IDs.
    free(tids);
    // Free the dynamically allocated array for thread argument pointers.
    free(args);

    sem_destroy(&sem); // Destroy the semaphore, freeing resources it used.
    close(fd1);        // Close the input file descriptor.

    return 0; // Indicate successful program execution.
}
