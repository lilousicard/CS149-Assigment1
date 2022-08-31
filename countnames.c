#include <stdio.h>
#include <stdbool.h>

bool stringIsNotEqual(char string[50], char str[150]);

int main (int argc, char *argv[]) {
    char str[150];
    char str2[100][50];
    int num[100];
    int i=0,j=0,value[100];
    FILE *fp;

    for (int index1 = 0; index1 < 100; index1++)
    {
        num[index1] = 0;
        for (int index2 = 0; index2 < 50; index2++)
        {
            str2[index1][index2]='\0';
        }
    }

    /* opening file for reading */
    fp = fopen( argv[1], "r");
    if(fp == NULL) {
        perror("Error opening file");
        return(-1);
    }
    int line = 0;
    while( fgets (str, 60, fp)!=NULL ) {
        j=0;
        line++;
        if (str[0] == '\n'){
            fprintf(stderr,"Warning - Line %d is empty.\n",line);
            continue;
        }

        while (stringIsNotEqual(str2[j], str) && str2[j][0] != '\0'){
            j++;
        }
        int k = 0;
        while (str[k] != '\n') {
            str2[j][k] = str[k];
            k++;
        }
        str2[j][k] = '\n';
        num[j] = num[j]+1;
    }
    fclose(fp);
    int index = 0;
    while (str2[index][0] != '\0' && index<100){
        int f = 0;
        while (str2[index][f]!='\n'){
            char test = str2[index][f];
            putchar(test);
            f++;
        }
        printf(" : %d\n",num[index]);
        index++;
    }

    return(0);
}

bool stringIsNotEqual(char string[50], char str[150]) {
    int i = 0;
    while(i < 50 && string[i] != '\0') {
        if (string[i] != str[i]) {
            return true;
        }
        i++;
    }
    return false;
}
