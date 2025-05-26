// Leia o PDF ou MD antes de iniciar este exercício!

// File-level overview:
// This program continuously approximates the value of Pi (π) using the Monte Carlo method.
// The approximation is stored in a global variable `pi`.
// The program also demonstrates signal handling. When a SIGINT signal (typically generated
// by pressing Ctrl+C in the terminal) is received, a custom signal handler function is invoked.
// This handler prints the current approximation of Pi to the console, saves this value
// to a file named "pi.txt", and then terminates the program.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> // Para open() e flags
#include <string.h> // Para strlen()
#include <signal.h> // Para sinal

// Variável GLOBAL para armazenar o valor de PI aproximado
// This global variable holds the most current approximation of Pi.
// It is continuously updated by the `aproxima_pi` function.
double pi = 0.0;

// Variável GLOBAL para armazenar o PID do processo
// This global variable stores the Process ID (PID) of the main process.
// It is used by the signal handler (`handler`) to send a SIGKILL signal
// to the process itself, ensuring termination upon receiving SIGINT.
pid_t pid;

// Função que gera um numero aleatorio uniformemente entre 0.0 e 1.0
// Você não deve alterar esta função
// random_zero_one: Generates and returns a pseudo-random double value
// uniformly distributed between 0.0 (inclusive) and 1.0 (inclusive).
// This function is a utility used by `aproxima_pi` for the Monte Carlo simulation.
double random_zero_one() {
    return (double)random()/(double)RAND_MAX;
}

// Função que calcula o valor de pi por simulação (Monte Carlo)
// Você não deve alterar esta função
// aproxima_pi: Implements the Monte Carlo simulation method to approximate the value of Pi.
//
// Logic:
// 1. It generates pairs of random (x, y) coordinates, where x and y are between 0 and 1.
//    These points fall within a 1x1 square in the first quadrant.
// 2. It calculates the distance 'd' of each point from the origin (0,0) using d = x*x + y*y.
// 3. If d <= 1, the point lies within a quarter circle of radius 1 centered at the origin.
// 4. The ratio of points falling 'dentro' (inside) the quarter circle to the 'total_pontos'
//    generated approximates the area of the quarter circle (π/4).
// 5. Therefore, Pi is approximated as 4 * (dentro / total_pontos).
//
// This function runs in an infinite `while(1)` loop, continuously refining the
// approximation of the global `pi` variable.
// It also includes a `sleep(1)` call that is triggered randomly with a low probability
// (0.8%), causing the simulation to pause for 1 second occasionally.
double aproxima_pi() {
    long dentro = 0;        // Counter for points inside the quarter circle
    long total_pontos = 0;  // Counter for total points generated

    double x,y,d; // Variables for coordinates and distance squared
    while (1) { // Infinite loop for continuous approximation
        x = random_zero_one(); // Generate random x coordinate
        y = random_zero_one(); // Generate random y coordinate
        d = x*x + y*y;         // Calculate squared distance from origin

        if (d<=1) { // Check if the point is inside the quarter circle (radius 1)
            dentro++;
        }
        total_pontos++;
        // Update the global pi variable with the new approximation
        pi = 4.0 * ((double)dentro/(double)total_pontos);

        // Occasionally sleep for 1 second to simulate some workload or I/O
        if (random_zero_one() < 0.008) {
            sleep(1);
        }
    }
    // This part of the function is unreachable due to the infinite loop,
    // but typically a function returning double would return a value.
    // However, in this design, it updates a global variable instead.
    return 0.0; // Should not be reached
}

// Crie AQUI a função que exporta o valor de pi para um arquivo chamado pi.txt
// export_pi: Writes the provided string representation of Pi to a specified file.
//
// Parameters:
//   file_name[]: A string representing the name of the file to write to (e.g., "pi.txt").
//   buffer_write[]: A string buffer containing the value of Pi, formatted as text.
void export_pi(char file_name[], char buffer_write[]) {
    // Abre o arquivo para escrita.
    // Flags used:
    //   O_WRONLY: Open the file in write-only mode.
    //   O_CREAT: If the file does not exist, create it.
    //   O_TRUNC: If the file already exists, truncate its length to 0 (overwrite).
    // Permissions (mode) 0644:
    //   Owner: read and write (rw-)
    //   Group: read-only (r--)
    //   Others: read-only (r--)
    int fd_escrita = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    // Error checking for open() is good practice, e.g., if (fd_escrita < 0) { perror("open"); return; }
    // The current code prints the file descriptor, which can be useful for debugging.
    printf("fd_escrita: %d\n", fd_escrita);

    // Escrever no arquivo
    // Attempts to write 'strlen(buffer_write)' bytes from 'buffer_write'
    // to the file associated with the file descriptor 'fd_escrita'.
    // Returns the number of bytes actually written.
    ssize_t bytes_written = write(fd_escrita, buffer_write, strlen(buffer_write));
    // Error checking for write() is also good practice, e.g., if (bytes_written < 0) { perror("write"); }
    printf("bytes_written: %ld\n", bytes_written);

    // Fecha o arquivo de escrita
    // Closes the file descriptor, releasing system resources associated with the open file.
    close(fd_escrita);
    printf("Arquivo fechado.\n");
    
}

// Esta função deve ser chamada pelo handler quando este for acionado

// Crie AQUI a função que será o handler do sinal
// handler: This function is a signal handler for the SIGINT signal (Ctrl+C).
//
// Parameter:
//   sig: The signal number that caused this handler to be invoked (expected to be SIGINT).
void handler(int sig) {
    // Print a message indicating that Ctrl+C was received and the current value of pi.
    printf("Ctrl-C recebido, exportando pi = %f\n", pi);

    // Define the name of the file where pi will be saved.
    char file_name[] = "pi.txt";
    // Create a buffer to hold the string representation of pi.
    char buffer_write[1000];

    // Format the double value of 'pi' into a string and store it in 'buffer_write'.
    // Includes a newline character for better file formatting.
    snprintf(buffer_write, sizeof(buffer_write), "%f\n", pi);

    // Call export_pi to write the formatted string to "pi.txt".
    export_pi(file_name, buffer_write);

    // Send a SIGKILL signal to the current process (identified by the global 'pid').
    // SIGKILL causes immediate termination and cannot be caught or ignored.
    // While effective, using exit(0) or exit(EXIT_SUCCESS) after cleanup
    // is often a cleaner way to terminate a process from a signal handler if
    // abrupt termination isn't strictly necessary. However, SIGKILL is used here.
    kill(pid, SIGKILL);
}


// main: The entry point of the program.
int main() {
    // Get the current process's ID and store it in the global variable 'pid'.
    // This PID is used by the signal handler to terminate the process.
    pid = getpid();
    // Exiba o PID deste processo
    // Print the PID of the current process to the console.
    printf("PID do processo: %d\n", getpid());

    // Registre AQUI seu handler para o sinal SIGINT!
    // Register the 'handler' function to be called when a SIGINT signal
    // (e.g., from Ctrl+C) is received by the process.
    signal(SIGINT, handler);

    
    // Seed the pseudo-random number generator using the current time.
    // This is important to ensure that `random_zero_one()` (and thus `aproxima_pi`)
    // produces different sequences of random numbers on different program runs.
    srand(time(NULL));

    // Start the Monte Carlo simulation to approximate Pi.
    // This function runs in an infinite loop and will only terminate when the
    // process receives a signal (like SIGINT, handled above) or is otherwise killed.
    aproxima_pi();

    // This return statement is technically unreachable because aproxima_pi()
    // contains an infinite loop and the process is expected to be terminated
    // by a signal.
    return 0;
}