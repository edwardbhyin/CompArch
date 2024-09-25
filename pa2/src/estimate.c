#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void printMatrix(double** matrix, int rows, int cols) {
    for(int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("\n");
    }
}

double** createIdentityMatrix(int sz) {
    double** identity = (double**)malloc(sz * sizeof(double*));
    for (int i = 0; i < sz; i++) {
        identity[i] = (double*)malloc(sz * sizeof(double));
        for (int j = 0; j < sz; j++) {
            identity[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
    return identity;
}

void scaleRow(double **matrix, int row, double factor, int n) {
    for (int i = 0; i < n; i++) {
        matrix[row][i] *= factor;
    }
}

void swapRows(double **matrix, int row1, int row2, int n) {
    for (int i = 0; i < n; i++) {
        double temp = matrix[row1][i];
        matrix[row1][i] = matrix[row2][i];
        matrix[row2][i] = temp;
    }
}

double** invertMatrix(double **matrix, int n) {
    double **identity = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        identity[i] = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            identity[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }

    for (int i = 0; i < n; i++) {
        int pivotRow = -1;
        for (int j = i; j < n; j++) {
            if (matrix[j][i] != 0.0) {
                pivotRow = j;
                break;
            }
        }

        if (pivotRow == -1) {
            printf("Matrix is not invertible.\n");
            return 0;
        }

        swapRows(matrix, i, pivotRow, n);
        swapRows(identity, i, pivotRow, n);

        double pivot = matrix[i][i];
        scaleRow(matrix, i, 1.0 / pivot, n);
        scaleRow(identity, i, 1.0 / pivot, n);

        for (int j = 0; j < n; j++) {
            if (j != i) {
                double factor = -matrix[j][i];
                for (int k = 0; k < n; k++) {
                    matrix[j][k] += factor * matrix[i][k];
                    identity[j][k] += factor * identity[i][k];
                }
            }
        }
    }
    return identity;
}

double** Tpose(int houses, int attributes, double** train) {
    double** array = (double**)malloc(attributes * sizeof(double*));
    for (int i = 0; i < attributes; i++) {
        array[i] = (double*)malloc(houses * sizeof(double));
        for (int j = 0; j < houses; j++) {
            array[i][j] = train[j][i];
        }
    }
    return array;
}

double** multMatrix(double** matrix1, double** matrix2, int row1, int col1, int col2) {
    double** result = (double**)malloc(row1*sizeof(double*));
    for (int i = 0; i < row1; i++) {
        result[i] = (double*)malloc(col2*sizeof(double));
        for (int j = 0; j < col2; j++) {
            result[i][j] = 0;
            for (int k = 0; k < col1; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return result;
}

int main (int argc, char *argv[]) {
    int thouses, dhouses, attributes;
    double** train;
    double** data;
    double** price;
    for (int i = 1; i < 3; i++) {
        FILE *ptr = fopen(argv[i], "r");
        if(ptr == NULL) {
            printf("Error openign file.\n");
            return 1;
        }

        char *type = (char *)malloc(100*sizeof(char));
        fscanf(ptr, "%[^\n]%*c", type);
        if (strcmp(type,"train") == 0) {
            fscanf(ptr, "%d", &attributes);
            fscanf(ptr, "%d", &thouses);

            attributes+=1;

            train = (double**)malloc(thouses * sizeof(double*));
            price = (double**)malloc(thouses * sizeof(double*));
            for (int j = 0; j < thouses; j++) {
                train[j] = (double*)malloc((attributes) * sizeof(double));
                train[j][0] = 1;
                price[j] = (double*)malloc(sizeof(double));
                for (int k = 1; k < attributes + 1; k++) {
                    if(k < attributes) {
                        fscanf(ptr, "%lf", &train[j][k]);
                    } else {
                        fscanf(ptr, "%lf", &price[j][0]);
                    }
                }
            }
        } else if (strcmp(type,"data") == 0) {
            fscanf(ptr, "%d", &attributes);
            fscanf(ptr, "%d", &dhouses);

            attributes+=1;

            data = (double**)malloc(dhouses * sizeof(double*));
            for (int j = 0; j < dhouses; j++) {
                data[j] = (double*)malloc(attributes * sizeof(double));
                data[j][0] = 1;
                for (int k = 1; k < attributes; k++) {
                    fscanf(ptr, "%lf", &data[j][k]);
                }
            }
        } else {
            printf("Error, unrecognized file type.\n");
            return 1;
        }
        free(type);
    }
    //printMatrix(train,houses,attributes);
    //printMatrix(data,houses,attributes);
    //printMatrix(price,houses,1);

    double** tTrain = Tpose(thouses,attributes,train);
    //printMatrix(tTrain,attributes,houses);
    //size of tTrain is attributes rows, houses columnus
    double** invertable = multMatrix(tTrain, train, attributes,thouses,attributes);
    //printMatrix(invertable,attributes,attributes);
    //size of invertable is attributes rows and attributes columns
    double** inv = invertMatrix(invertable,attributes);
    //printMatrix(inv, attributes, attributes);
    //size of inv is the same as above
    double** preWeight = multMatrix(inv,tTrain,attributes,attributes,thouses);
    //printMatrix(preWeight,attributes,houses);
    //preweight is attributes rows houses columns
    double** weight = multMatrix(preWeight,price,attributes,thouses,1);
    //weight is attributes rows and 1 column
    double** finalPrice = multMatrix(data,weight,dhouses,attributes,1);
    for (int i = 0; i < dhouses; i++) {
        printf("%.0f\n", finalPrice[i][0]);
    }

    freeMatrix(train,thouses);
    freeMatrix(data,dhouses);
    freeMatrix(price,thouses);
    freeMatrix(finalPrice,dhouses);
    freeMatrix(tTrain,attributes);
    freeMatrix(invertable,attributes);
    freeMatrix(inv,attributes);
    freeMatrix(preWeight,attributes);
    freeMatrix(weight,attributes);

    return 0;
}