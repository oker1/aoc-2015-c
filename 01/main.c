#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int floor = 0;
    int basement_char = -1;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        for (int i = 0; i < read; i++)
        {
            switch (line[i])
            {
            case '(':
                floor++;
                break;
            case ')':
                floor--;
                break;
            default:
                break;
            }

            if (floor == -1 && basement_char == -1)
            {
                basement_char = i + 1;
            }
        }
    }

    fclose(fp);
    if (line)
        free(line);

    printf("Floor: %d\n", floor);
    printf("Basement char: %d\n", basement_char);

    exit(EXIT_SUCCESS);
}