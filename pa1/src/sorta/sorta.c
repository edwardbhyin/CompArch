#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sort(int size, char *arr[])
{
    int i, j;
    const char *key;
    for (i = 1; i < size; i++)
    {
        key = arr[i];
        j = i - 1;

        while (j >= 0 && strcmp(arr[j], key) > 0)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = (char *)key;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 0;
    }
    else
    {
        char **arr = (char **)malloc((argc - 1) * sizeof(char *));
        for (int i = 0; i < argc - 1; i++)
        {
            arr[i] = (char *)malloc(strlen(argv[i + 1]) + 1);
            strcpy(arr[i], argv[i + 1]);
        }
        sort(argc - 1, arr);
        for (int i = 0; i < argc - 1; i++)
        {
            printf("%s\n", arr[i]);
            free(arr[i]);
        }
        free(arr);
        return 0;
    }
}
