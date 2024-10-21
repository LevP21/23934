#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
    int opt;
    char *endptr;
    
    while ((opt = getopt(argc, argv, "ispucC:dvV:U:")) != -1) {
        switch (opt) {
            case 'i':
                printf("Real UID: %d, Effective UID: %d\n", getuid(), geteuid());
                printf("Real GID: %d, Effective GID: %d\n", getgid(), getegid());

                break;
            case 's':
                if (setpgid(0, 0) == -1) {
                    perror("setpgid failed");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Process is now the leader of its process group.\n");
                }

                break;
            case 'p':
                printf("Process ID: %d, Parent Process ID: %d, Group ID: %d\n", getpid(), getppid(), getpgrp());

                break;
            case 'u':
                printf("Ulimit: %ld\n", ulimit(UL_GETFSIZE));

                break;
            case 'U':
                long new_ulimit = strtol(optarg, &endptr, 10);

                if (*endptr != '\0') {
                    fprintf(stderr, "Invalid ulimit value: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                if (ulimit(UL_SETFSIZE, new_ulimit) == -1) {
                    perror("Failed to set ulimit");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Ulimit changed to: %ld\n", new_ulimit);
                }
                
                break;
            case 'c':
                struct rlimit rl;

                if (getrlimit(RLIMIT_CORE, &rl) == -1) {
                    perror("getrlimit failed");
                    exit(EXIT_FAILURE);
                }

                printf("Core file size limit: %ld bytes\n", rl.rlim_cur);

                break;
            case 'C':
                rlim_t new_size = (rlim_t)strtol(optarg, &endptr, 10);

                if (*endptr != '\0') {
                    fprintf(stderr, "Invalid core size value: %s\n", optarg);
                    exit(EXIT_FAILURE);
                }

                struct rlimit rl;

                rl.rlim_max = new_size;
                rl.rlim_cur = new_size;
                
                if (setrlimit(RLIMIT_CORE, &rl) == -1) {
                    perror("setrlimit failed");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Core file size changed to: %ld bytes\n", new_size);
                }
                
                break;
            case 'd':
                char cwd[PATH_MAX];

                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("Current working directory: %s\n", cwd);
                } else {
                    perror("getcwd failed");
                    exit(EXIT_FAILURE);
                }

                break;
            case 'v':
                extern char** environ;

                for (char** env = environ; *env; ++env) {
                    printf("%s\n", *env);
                }

                break;
            case 'V':
                char* name_value = optarg;

                if (putenv((char*)name_value) != 0) {
                    perror("putenv failed");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Environment variable set: %s\n", name_value);
                }

                break;
            default:
                fprintf(stderr, "Usage: %s [-ispucC:dvV:U:]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    return 0;
}