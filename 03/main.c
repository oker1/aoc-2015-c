#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <glib.h>

int array_solution(int instruction_count, FILE *fp)
{
    long space_size = instruction_count * 2 + 1;

    int **coordinates = (int **)malloc(space_size * sizeof(int *));
    for (int i = 0; i < space_size; i++)
    {
        coordinates[i] = (int *)malloc(space_size * sizeof(int));
        memset(coordinates[i], 0, space_size * sizeof(int));
    }

    int pos_x = instruction_count;
    int pos_y = instruction_count;
    coordinates[pos_x][pos_y] = 1; // starting location gets present

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        for (int i = 0; i < read; i++)
        {
            switch (line[i])
            {
            case '^':
                pos_y++;
                coordinates[pos_x][pos_y]++;
                break;
            case 'v':
                pos_y--;
                coordinates[pos_x][pos_y]++;
                break;
            case '>':
                pos_x++;
                coordinates[pos_x][pos_y]++;
                break;
            case '<':
                pos_x--;
                coordinates[pos_x][pos_y]++;
                break;
            }
        }
    }

    int x = 0;
    for (int i = 0; i < space_size; i++)
    {
        for (int j = 0; j < space_size; j++)
        {
            if (coordinates[i][j] > 0)
            {
                x++;
            }
        }
    }

    free(line);

    return x;
}

struct coordinates
{
    int x;
    int y;
};

struct coordinates *create_coordinates(int x, int y)
{
    struct coordinates *c1 = malloc(sizeof(struct coordinates));
    c1->x = x;
    c1->y = y;

    return c1;
}

guint coordinates_hash(gconstpointer value)
{
    const struct coordinates *c = value;
    guint hash = 17;
    hash = hash * 37 + g_int_hash(&c->x);
    hash = hash * 37 + g_int_hash(&c->y);

    return hash;
}

gboolean coordinates_equal(gconstpointer a,
                           gconstpointer b)
{
    const struct coordinates *a1 = a;
    const struct coordinates *b1 = b;

    // printf("%d %d %d %d %d\n", a1->x, b1->x, a1->y, b1->y, (a1->x == b1->x) && (a1->y == b1->y));

    return (a1->x == b1->x) && (a1->y == b1->y);
}

int hashtable_solution(FILE *fp)
{
    GHashTable *map = NULL;

    map = g_hash_table_new(coordinates_hash, coordinates_equal);

    int pos_x = 0;
    int pos_y = 0;

    // add starting pos
    struct coordinates *c = malloc(sizeof(struct coordinates));
    c->x = pos_x;
    c->y = pos_y;
    int *count = malloc(sizeof(int));
    *count = 1;
    g_hash_table_insert(map, c, count);

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1)
    {
        for (int i = 0; i < read; i++)
        {
            switch (line[i])
            {
            case '^':
            {
                pos_y++;

                const struct coordinates *key = create_coordinates(pos_x, pos_y);

                count = g_hash_table_lookup(map, key);
                if (count != NULL)
                {
                    (*count)++;
                }
                else
                {
                    count = malloc(sizeof(int));
                    *count = 1;
                    g_hash_table_insert(map, (gpointer)key, count);
                }
            }

            break;
            case 'v':
            {
                pos_y--;

                const struct coordinates *key = create_coordinates(pos_x, pos_y);

                count = g_hash_table_lookup(map, key);
                if (count != NULL)
                {
                    (*count)++;
                }
                else
                {
                    count = malloc(sizeof(int));
                    *count = 1;
                    g_hash_table_insert(map, (gpointer)key, count);
                }
            }
            break;

            case '<':
            {
                pos_x--;

                const struct coordinates *key = create_coordinates(pos_x, pos_y);

                count = g_hash_table_lookup(map, key);
                if (count != NULL)
                {
                    (*count)++;
                }
                else
                {
                    count = malloc(sizeof(int));
                    *count = 1;
                    g_hash_table_insert(map, (gpointer)key, count);
                }
            }

            break;

            case '>':
            {
                pos_x++;

                const struct coordinates *key = create_coordinates(pos_x, pos_y);

                count = g_hash_table_lookup(map, key);
                if (count != NULL)
                {
                    (*count)++;
                }
                else
                {
                    count = malloc(sizeof(int));
                    *count = 1;
                    g_hash_table_insert(map, (gpointer)key, count);
                }
            }

            break;
            }
        }
    }

    GHashTableIter iter;

    g_hash_table_iter_init(&iter, map);

    struct coordinates *key;
    int *value;

    int x = 0;
    while (g_hash_table_iter_next(&iter, (gpointer)&key, (gpointer)&value))
    {
        printf("%d,%d -> %d\n", key->x, key->y, *value);

        if (*value > 0)
        {
            x++;
        }
    }

    return x;
}

int main(int argc, char *argv[])
{
    FILE *fp;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    fseek(fp, 0L, SEEK_END);
    long instruction_count = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    printf("Visited house count [array]: %d\n", array_solution(instruction_count, fp));

    fseek(fp, 0L, SEEK_SET);

    printf("Visited house count [hashmap]: %d\n", hashtable_solution(fp));

    fclose(fp);

    exit(EXIT_SUCCESS);
}