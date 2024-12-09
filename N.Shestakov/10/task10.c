#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Использование: %s <команда> [аргументы]\n", argv[0]);
        exit(1);
    }

    pid_t pid = fork(); 

    if (pid < 0) { 
        perror("Ошибка при fork\n");
        exit(1);
    }

    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("Ошибка при execvp\n");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("\nПроцесс завершился с кодом: %d\n", exit_code);
        } else {
            printf("\nПроцесс завершился ненормально\n");
        }
    }

    return 0;
}