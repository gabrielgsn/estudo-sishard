#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    char *prog = "./siscoin";
    char *args[] = {prog, "abcde", "2", "3", NULL};
    int exit_status = -1;


    while(exit_status != 0){
        pid_t pid = fork();

        if (pid == 0) { // Child process
            execv(prog, args);
            perror("execv"); // If execv fails
            exit(EXIT_FAILURE);
            
        } else { // Parent process
            waitpid(pid, &exit_status, 0); // Wait for child to finish
            if (WIFEXITED(exit_status)) {
                exit_status = WEXITSTATUS(exit_status);
                printf("Child exited with status: %d\n", exit_status);
            }
        }
    }

    return 0;
}