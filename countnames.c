/**
 * Description: This module takes a text file and return the total count of each names in the file.
 * Author name: Lilou Sicard-Noel
 * Author email: lilou.sicard-noel@sjsu.edu 
 * Last modified date: 09/05/2022
 * Creation date: 08/31/2022
 * GitHub Repo : https://github.com/lilousicard/CS149-Assigment1
 **/
#include <stdio.h>
#include <stdbool.h>

bool stringIsNotEqual(char string[50], char str[150]);

/**
 * This function takes a file name, opens it, and count the name occurences in it
 * It then print the result
 * Assumption: There are less than a 100 different names, The names are less than 50 characters
 * Input parameters: Argv[1], a text file
 * Returns: an integer
**/

int main (int argc, char *argv[]) {
    char str[150];
    char str2[100][50];
    int num[100];
    int j=0;
    FILE *fp;

    //Initialization of the array to avoid random characters
    for (int index1 = 0; index1 < 100; index1++)
    {
        num[index1] = 0;
        for (int index2 = 0; index2 < 50; index2++)
        {
            str2[index1][index2]='\0';
        }
    }
    //If no input, return 0
    if (argv[1]==NULL){
        return 0;
    }

    // opening file for reading 
    fp = fopen( argv[1], "r");
    if(fp == NULL) {
        perror("error: cannot open file\n");
        return(-1);
    }

    int line = 0;
    //Loop to read all the lines
    while( fgets (str, 60, fp)!=NULL ) {
        j=0;
        line++;
        //empty line 
        if (str[0] == '\n'){
            fprintf(stderr,"Warning - Line %d is empty.\n",line);
            continue;
        }

        //try to find if the string is already in the array
        //if not, find the next empty spot
        while (stringIsNotEqual(str2[j], str) && str2[j][0] != '\0'){
            j++;
        }
        int k = 0;
        //store the string in the 2D array
        while (str[k] != '\n') {
            str2[j][k] = str[k];
            k++;
        }
        str2[j][k] = '\n';
        //Add 1 to the total count for that name
        num[j] = num[j]+1;
    }
    fclose(fp);
    //Print the result
    int index = 0;
    while (str2[index][0] != '\0' && index<100){
        int f = 0;
        //print the name one char at the time
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
/**
 * This function compares two arrays of char and return true
 * if they are not exacly equal
 * Assumption: The first array has less than 50 characters,
 *             The second array has less than 150 characters
 * Input parameters: string, str
 * Return: a boolean
 **/
bool stringIsNotEqual(char string[50], char str[150]) {
    int i = 0;
    // This loop will terminate if we are about to have
    // an overflow or if the string is finished ('\0')
    while(i < 50 && string[i] != '\0') {
        if (string[i] != str[i]) {
            return true;
        }
        i++;
    }
    return false;
}
