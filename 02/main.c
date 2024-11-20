#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    char *line = NULL;
    char *token = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int line_count = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        line_count++;
    }

    fseek(fp, 0, SEEK_SET);

    int min_side_area;
    int l, w, h;
    int paper_area_sum = 0;
    int ribbon_sum = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        do
        {
            token = strsep(&line, "x");
            l = atoi(token);
            token = strsep(&line, "x");
            w = atoi(token);
            token = strsep(&line, "x");
            h = atoi(token);

            min_side_area = MIN(l * w, MIN(h * l, w * h));

            paper_area_sum = paper_area_sum + 2 * l * w + 2 * w * h + 2 * h * l + min_side_area;

            ribbon_sum = ribbon_sum + ((2 * (l + w + h)) - (2 * MAX(l, MAX(w, h)))) + (l * w * h);
        } while (line != NULL);
    }

    fclose(fp);
    free(line);

    printf("Paper sum: %d\n", paper_area_sum);
    printf("Ribbon sum: %d\n", ribbon_sum);

    exit(EXIT_SUCCESS);
}