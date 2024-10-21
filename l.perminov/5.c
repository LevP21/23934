#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LENGTH 256

int main(int argc, char *argv[]) {
    int fd = open(argv[1], O_RDONLY);

    int offsets[MAX_LENGTH] = {0};
    int lengths[MAX_LENGTH] = {0};

    int count = 0, num = 0, offset = 0, bytes = 0, length = 0;
    
    char line[MAX_LENGTH] = {0};
    char ch = 0;
    
    while (read(fd, &ch, 1)) {
        length++;

        if (ch == '\n') {
            lengths[count] = length;               
            offsets[count] = offset;

            count++;
            length = 0;
           
            offset = lseek(fd, 0L, 1);
        }                      
    }

    if (length > 0) {
        lengths[count] = length;
        offsets[count] = offset;

        count++;
    }

    while (1) {
        printf("Enter line number (0 to quit): ");

        scanf("%d", &num);

        if (num == 0) {
            break;
        }

        if (num < 1 || num > count) {
            printf("Invalid line number. Please enter a number between 1 and %d.\n", count);
            continue;
        }

        lseek(fd, offsets[num-1], 0);

        read(fd, line, lengths[num-1]);

        line[lengths[num-1]] = '\0';

        printf("%s\n", line);
    }   

    close(fd);

    return 0;
}