#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char * encrypt(char text[])
{
    int i,n;
    char ch;
    for(i = 0; i<strlen(text);i++)
    {
        if(isalpha(text[i]))
        {
            n = text[i];
            n = n+13;
            if(islower(text[i]))
            {
                if(n > 'z')
                {
                    n = n - 'z' + '`';
                    ch = n;
                    text[i] = ch;
                }
                else
                {
                    ch = n;
                    text[i] = ch;
                }
            }
            else
            {
                if(n > 'Z')
                {
                    n = n - 'Z' + '@';
                    ch = n;
                    text[i] = ch;
                }
                else
                {
                    ch = n;
                    text[i] = ch;
                }
            }
        }
    }
    return text;
}


int main(int argc, char** argv)
{
    char text[500];
    strcpy(text, encrypt(argv[1]));
    printf("%s\n",text);
}