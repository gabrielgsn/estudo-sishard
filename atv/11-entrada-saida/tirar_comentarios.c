/* POSIX headers */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* Standard C */
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        return 1;
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        return 1;
    }
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0700);
    if (fd2 == -1) {
        close(fd1);
        return 1;
    }

    int estado = 0;
    char buf[1];
    ssize_t nread;

    while ((nread = read(fd1, buf, 1)) > 0) {
        char c = buf[0];
        switch (estado) {
            case 0:
                if (c == '/') {
                    estado = 1;
                } else {
                    write(fd2, buf, 1);
                }
                break;

            case 1: // dps de '/'
                if (c == '/') {
                    estado = 2; // linha
                } else if (c == '*') {
                    estado = 3; // multilinha
                } else {
                    buf[0] = '/';
                    write(fd2, buf, 1);
                    buf[0] = c;
                    write(fd2, buf, 1);
                    estado = 0;
                }
                break;

            case 2: // linha (//)
                if (c == '\n') {
                    write(fd2, buf, 1);
                    estado = 0;
                }
                break;

            case 3: // multilinha (/*)
                if (c == '*') {
                    estado = 4;
                }
                break;

            case 4: // dps de '*' em multilinha
                if (c == '/') {
                    estado = 0;
                } else if (c != '*') {
                    estado = 3;
                }
                break;
        }
    }

    if (estado == 1) {
        buf[0] = '/';
        write(fd2, buf, 1);
    }

    if (nread == -1) {
        fprintf(stderr, "Erro ao ler o arquivo de entrada '%s'\n", argv[1]);
        close(fd1);
        close(fd2);
        return 1;
    }

    close(fd1);
    close(fd2);

    return 0;
}