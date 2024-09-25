#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    FILE *ptr = fopen(filename, "r");

    if (ptr == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int sz;
    if (fscanf(ptr, "%d", &sz) != 1) {
    printf("Failed to read the integer from the file.\n");
    fclose(ptr);
    return 1;
    }
    
    int (*matrix)[10] = malloc(sizeof(int[10][10]));

    if (matrix == NULL) {
        printf("Memory allocation failed.\n");
        fclose(ptr);
        return 1;
    }

    for (int i = 0; i < sz; i++) {
    for (int j = 0; j < sz; j++) {
        int value;
        if (fscanf(ptr, "%d", &value) != 1) {
            printf("Failed to read an integer from the file.\n");
            fclose(ptr);
            free(matrix);
            return 1;
        }
        matrix[i][j] = value;
    }
    }

    int n;
    if (fscanf(ptr, "%d", &n) != 1) {
    printf("Failed to read the integer from the file.\n");
    fclose(ptr);
    return 1;
    }

    if (n == 0) {
        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                if (i == j) {
                    printf("1 ");
                } else {
                    printf("0 ");
                }
            }
            printf("\n");
        }
    } else {
        int (*base)[10] = malloc(sizeof(int[10][10]));

        if (base == NULL) {
            printf("Memory allocation failed.\n");
            free(matrix);
            fclose(ptr);
            return 1;
        }

        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                base[i][j] = matrix[i][j];
            }
        }

        for (int iter = 1; iter < n; iter++) {
            int (*answer)[10] = malloc(sizeof(int[10][10]));

            if (answer == NULL) {
                printf("Memory allocation failed.\n");
                free(matrix);
                free(base);
                fclose(ptr);
                return 1;
            }

            for (int i = 0; i < sz; i++) {
                for (int j = 0; j < sz; j++) {
                    int dotProd = 0;
                    for (int k = 0; k < sz; k++) {
                        dotProd = (matrix[i][k] * base[k][j]) + dotProd;
                    }
                    answer[i][j] = dotProd;
                }
            }

            for (int i = 0; i < sz; i++) {
                for (int j = 0; j < sz; j++) {
                    matrix[i][j] = answer[i][j];
                }
            }

            free(answer);
        }

        for (int i = 0; i < sz; i++) {
            for (int j = 0; j < sz; j++) {
                printf("%i", matrix[i][j]);
                if (j + 1 < sz) {
                    printf(" ");
                }
            }
            printf("\n");
        }

        free(base);
    }

    free(matrix);
    fclose(ptr);
    return 0;
}
