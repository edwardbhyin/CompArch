#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int rowCheck (int arr[][10])
{
    for(int rows = 0; rows < 9; rows++)
    {
        for(int i = 0; i < 9; i++)
        {
            for(int j = i+1; j < 9; j++)
            {
                int key = arr[rows][i];
                if (key == arr[rows][j])
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int fastRowCheck (int arr[9])
{
    for(int i = 0; i < 9; i++)
    {
        for(int j = i+1; j < 9; j++)
        {
            int key = arr[i];
            if (key == arr[j])
            {
                return 1;
            }
        }
    }
    return 0;
}

int colCheck (int arr[][10])
{
    for(int cols = 0; cols < 9; cols++)
    {
        for(int i = 0; i < 9; i++)
        {
            for(int j = i+1; j < 9; j++)
            {
                int key = arr[i][cols];
                if (key == arr[j][cols])
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

int sqCheck(int arr[9][10])
{
    int squares[] = {0,3,6};
    int sillyRow[9];
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            int k = 0;
            for(int row = squares[i]; row < squares[i]+3; row++)
            {
                for(int col = squares[j]; col < squares[j]+3; col++)
                {
                    sillyRow[k] = arr[row][col];
                    k = k + 1;
                }
            }
            if(fastRowCheck(sillyRow)==1)
            {
                return 1;
            }
        }
    }
    return 0;
}

int findMissing(int arr[10])
{
    int i, key, j;
    int mNum;
    for (i = 1; i < 9; i++)
    {
        key = arr[i];
        j = i - 1;
 
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }

    for(int k = 1; k < 9; k++)
    {
        if (arr[k] - arr[k-1] != 1)
        {
            mNum = arr[k] - 1;
            break;
        }
    }
    return mNum;
}

int colToRow(int arr[][10], int col)
{
    int tempArr[10];
    for(int i = 0; i < 9; i++)
    {
        tempArr[i] = arr[i][col];
    }
    return(findMissing(tempArr));
}

int sqToRow(int arr[][10], int row, int col)
{
    int squares[] = {0,3,6};
    int tempArr[10];
    int k = 0;
    for(int i = squares[row/3]; i < squares[row/3] + 3; i++)
    {
        for(int j = squares[col/3]; j < squares[col/3] + 3; j++)
        {
            tempArr[k] = arr[i][j];
            k++;
        }
    }
    return(findMissing(tempArr));
}

int main (int argc, char *argv[])
{
    char *filename = argv[1];
    FILE* ptr;
    char ch;
    int board[9][10];
    int isComplete = 0;
    int mRow, mCol;

    ptr = fopen(filename, "r");
    if (NULL == ptr)
    {
        printf("error\n");
    }

    for(int i = 0; i < 9; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            ch = fgetc(ptr);
            if(isalnum(ch))
            {
                board[i][j] = ch - '0';
            }
            else if(ch == '_')
            {
                board[i][j] = 0;
                isComplete = 1;
                mRow = i;
                mCol = j;
            }
        }
    }
    if(isComplete == 0)
    {
        if(rowCheck(board) == 0 && colCheck(board) == 0 && sqCheck(board) == 0)
        {
            printf("correct\n");
        }
        else
        {
            printf("incorrect\n");
        }
    }
    else
    {
        int mNum = findMissing(board[mRow]);
        if(mNum == colToRow(board,mCol) && mNum == sqToRow(board,mRow,mCol))
        {
            printf("solveable\n");
        }
        else
        {
            printf("unsolvable\n");
        }
    }
    return 0;
}