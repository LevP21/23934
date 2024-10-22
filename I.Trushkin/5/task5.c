#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define FILE_NAME_LEN 100
#define MAX_NUMB_OF_LINES 1000
#define MAX_STR_LEN 200

typedef struct lineInfo {
    size_t length;
    off_t offset;
} lineInfo;


lineInfo* makeIndentationTable(char* filename, int* countStrings) {
    FILE* fin = fopen(filename, "r");
    if (fin == NULL) {
        perror("failed to open file\n");
        return NULL;
    }

    lineInfo* lineTable = (lineInfo*)malloc(MAX_NUMB_OF_LINES * sizeof(lineInfo));
    if (lineTable == NULL) {
        perror("failed to allocate memory\n");
        fclose(fin);
        return NULL;
    }

    char tempStr[MAX_STR_LEN];
    off_t currentOffset = 0;
    while (fgets(tempStr, MAX_STR_LEN, fin) != NULL) {
        lineTable[*countStrings].length = strlen(tempStr);
        lineTable[*countStrings].offset = currentOffset;

        currentOffset += strlen(tempStr) ;
        *countStrings += 1;
    }
    fclose(fin);
    return lineTable;
}

void printLinesByNumber(char* filename, int countStr, lineInfo* lineTable) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("failed to open file\n");
        return;
    }

    int numbsrting = 0;
    char input[10];

    while (1) {
        printf("Enter line number (0 to exit):\n");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("failed to read input");
            continue;
        }

        // Удаляем символ новой строки, если он присутствует
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }

        // Преобразуем строку в число
        char *endptr;
        numbsrting = strtol(input, &endptr, 10);

        if (numbsrting == 0) {
            break;
        }

        if (numbsrting > countStr || numbsrting < 0 ) {
            printf("uncorrect numb of string\n");
            continue;
        }

        if (numbsrting > 0 && numbsrting < countStr){


            lseek(fd, lineTable[numbsrting - 1].offset, SEEK_SET);

            char tempString[MAX_STR_LEN];
            read(fd, tempString, lineTable[numbsrting - 1].length);

            tempString[lineTable[numbsrting - 1].length] = '\0';
            printf("%s\n\n", tempString);
        }
        else{
            continue;
        }
    }
        close(fd);
}

int main(int argc, char *argv[]) {
    char filename[FILE_NAME_LEN];
    if (argc == 1) {
        printf("Enter file name, please:\n");
        if (fgets(filename, FILE_NAME_LEN, stdin) == NULL) {
            perror("failed to read file name");
            return 1;
        } else {
            size_t len = strlen(filename);
            if (len > 0 && filename[len - 1] == '\n') {
                filename[len - 1] = '\0';
            }
        }
    } else {
        strncpy(filename, argv[1], FILE_NAME_LEN - 1);
        filename[FILE_NAME_LEN - 1] = '\0';
    }

    int countStrings = 0;

    lineInfo* lineTable = makeIndentationTable(filename, &countStrings);
    if (lineTable == NULL) {
        return 1;
    }

    for (int i = 0; i < countStrings; i++) {
        printf("%d %ld\n", lineTable[i].length, lineTable[i].offset);
    }

    printLinesByNumber(filename, countStrings, lineTable);

    free(lineTable);
    return 0;
}