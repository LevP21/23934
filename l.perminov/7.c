#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define MAX_LENGTH 256

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);

    int offsets[MAX_LENGTH] = {0};
    int lengths[MAX_LENGTH] = {0};

    int count = 0, num = 0, offset = 0, bytes = 0, length = 0;

    char line[MAX_LENGTH] = {0};
    char ch = 0;

    struct stat st;
    fstat(fd, &st);

    size_t file_size = st.st_size;

    char *mapped = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

    for (int i = 0; i < file_size; ++i)
    {
        length++;

        if (mapped[i] == '\n')
        {
            lengths[count] = length;
            offsets[count] = offset;

            count++;
            length = 0;

            offset = mapped[i];
        }
    }

    if (length > 0)
    {
        lengths[count] = length;
        offsets[count] = offset;

        count++;
    }

    while (1)
    {
        printf("Enter line number (0 to quit): ");

        scanf("%d", &num);

        if (num == 0)
        {
            break;
        }

        if (num < 1 || num > count)
        {
            printf("Invalid line number. Please enter a number between 1 and %d.\n", count);
            continue;
        }

        for (int i = offsets[num - 1]; i < offsets[num - 1] + length[num - 1]; ++i)
        {
            printf("%c", mapped[i]);
        }

        printf("\n");
    }

    munmap(mapped, file_size);

    close(fd);

    return 0;
}