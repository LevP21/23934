#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    uid_t real_uid = getuid();
    uid_t effective_uid = geteuid();

    printf("Real UID: %d\n", real_uid);
    printf("Effective UID: %d\n", effective_uid);
    
    const char *filename = argv[1];
    
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("File opening error");
    } else {
        printf("File '%s' successfully opened.\n", filename);
        fclose(file);
    }

    setuid(real_uid);

    printf("Real UID: %d\n", getuid());
    printf("Effective UID: %d\n", geteuid());
    
    file = fopen(filename, "r");

    if (file == NULL) {
        perror("File opening error");
    } else {
        printf("File '%s' successfully reopened.\n", filename);
        fclose(file);
    }

    return 0;
}
