// Leia o PDF ou MD antes de iniciar este exercício!

// File-level overview:
// This program acts as a wrapper to execute an external program, "./siscoin".
// It takes three command-line arguments (origin wallet, destination wallet, value)
// and passes them to "siscoin".
// The primary purpose of this wrapper is to repeatedly attempt to run "siscoin"
// if "siscoin" indicates a retryable error by returning an exit code greater than 0.
// This is useful for scenarios where "siscoin" might fail intermittently (e.g., due
// to temporary network issues or other transient problems). The loop continues
// until "siscoin" returns 0 (success) or a negative value (non-retryable error,
// such as invalid arguments, which is handled within siscoin).

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

// main function: Entry point of the q2 program.
// argc: Argument count. It should be 4 for this program:
//       argv[0]: program name ("./q2")
//       argv[1]: origin wallet
//       argv[2]: destination wallet
//       argv[3]: value
// argv: Argument vector (array of strings).
int main(int argc, char *argv[]){

    int ret; // Stores the exit status of the child process (siscoin).
    int wst; // Stores the status information from waitpid.

    // Loop to repeatedly execute siscoin until it succeeds or returns a non-retryable error.
    // The loop continues as long as 'ret' (siscoin's exit status) is greater than 0.
    // siscoin is expected to return:
    //  0: success
    // >0: retryable error (e.g., network issue, insufficient funds if it were a real system)
    // -1: non-retryable error (e.g., invalid arguments - handled by siscoin)
    do {
        // Define the path to the executable to be run.
        char *prog = "./siscoin";
        // Prepare the argument array for execv.
        // argv[0] of the new program is its name.
        // argv[1], argv[2], argv[3] are the arguments passed from q2's command line.
        // NULL terminates the argument list for execv.
        char *args[] = {prog, argv[1], argv[2], argv[3], NULL};

        // Create a new child process.
        pid_t pid = fork();

        if (pid == -1) { // Fork failed
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            // Replace the child process's image with the siscoin program.
            // prog: path to the executable.
            // args: argument list.
            execv(prog, args);
            // If execv returns, it means an error occurred.
            perror("execv");
            // Exit the child process with a failure status if execv failed.
            exit(EXIT_FAILURE);
        } else { // Parent process
            // Wait for the child process (with PID 'pid') to terminate.
            // &wst: stores status information about how the child terminated.
            // 0: options, 0 means wait for any child matching pid.
            waitpid(pid, &wst, 0);
            // Extract the actual exit status of the child process from wst.
            // This is the value returned by siscoin's main function or exit().
            ret = WEXITSTATUS(wst);
        }

        // The comment "No pai, espere e trate o resultado" is addressed by waitpid and WEXITSTATUS.
        // The comment "DICA: ao atribuir o return do siscoin, faça casting para (char)"
        // is a hint for siscoin's internal logic, not directly applicable here for q2's 'ret'
        // which correctly stores an int. If siscoin itself were to return a char that q2
        // needed to interpret in a specific way beyond its integer value for the loop condition,
        // then casting might be relevant, but the loop condition relies on the integer exit code.

    } while (ret > 0); // Repeat if siscoin's exit status indicates a retryable error (>0).
                       // The loop terminates if siscoin returns 0 (success) or a negative value
                       // (non-retryable error, e.g., invalid parameters, as per siscoin's design).

    // The program q2 itself returns 0, indicating successful completion of its task,
    // which is to ensure siscoin eventually ran successfully (returned 0).
    return 0;
}