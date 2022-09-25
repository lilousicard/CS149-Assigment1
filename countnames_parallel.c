/**
 * Description: This module takes one or more text files and return the total count of each names in the files.
 * Author name: Lilou Sicard-Noel
 * Author email: lilou.sicard-noel@sjsu.edu 
 * Last modified date: 09/25/2022
 * Creation date: 09/13/2022
 * GitHub Repo : https://github.com/lilousicard/CS149-Assigment1
 **/
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_NAME 100
#define MAX_LENGTH 30


struct Person {
    char name[MAX_LENGTH];
    int occurrence;
    bool hasAName;
};
bool stringIsNotEqual(char string[MAX_LENGTH], char str[MAX_LENGTH]);

/**
 * This function takes a list of file names, opens them, and count the name occurrences in it
 * It then print the result
 * Assumption: There are less than a 100 different names, The names are less than 30 characters, A line
 * with less than 2 characters is not a name and is ignored by the program
 * Input parameters: Argv[], a list of text files
 * Returns: an integer
**/

int main (int argc, char *argv[]) {

    int pfds[2];
    struct Person buf[MAX_NAME];
    struct Person listNames[MAX_NAME];
    pipe(pfds);

    
    for (int i = 0; i < MAX_NAME; i++){
        listNames[i].name[0] = '\0';
        listNames[i].hasAName = false;
    }
    

    for (int i = 1; i<argc; i++){
        char *fileName = argv[i];
        int childID = fork();
        if (childID == 0){
            char str[MAX_LENGTH];
            struct Person names[MAX_NAME];
            int namesSize = 0;
            FILE *fp;
            
            for (int i = 0; i < MAX_NAME; i++){
        	names[i].name[0] = '\0';
       	 	names[i].hasAName = false;
       	 	}

            // opening file for reading
            fp = fopen( fileName, "r");

            // check that fp is not null, return an error otherwise
            if(fp == NULL) {
                fprintf(stderr,"range: cannot open file %s \n",fileName);
                _Exit(1);
            }
            //int j = 0;
            int line = 0;
            //Loop to read all the lines
            while(fgets (str, 30, fp) != NULL ) {
                line++;
                //empty line
                if (str[0] == '\n'){
                    fprintf(stderr,"Warning - file %s Line %d is empty.\n",fileName,line);
                    continue;
                }
                //printf("File %s line: %d input : %s\n",argv[i],line,str);
                //If the line's length is less than or equal to 1, ignore the line
                //See a1 channel on discord, message from ProfB on 09/06 at 0904 for reasoning
                if  (str[1] == '\n'){
                    continue;
                }

                //try to find if the name is already in the array
                //if not, find the next empty spot
                int index = 0;
                while (stringIsNotEqual(names[index].name,str) && index<namesSize){
                    if (str[index] == '\n') {
                        index = namesSize-1;
                    }
                    index++;
                }
                if (index>=namesSize){
                    int i = 0;
                    while (str[i]!='\n'){
                        names[index].name[i]=str[i];
                        i++;
                    }
                    names[index].name[i]='\0';
                    names[index].occurrence = 1;
                    names[index].hasAName = true;
                    namesSize++;
                } else names[index].occurrence++;
            }
            fclose(fp);
            write (pfds[1],names,sizeof(buf));
            close (pfds[1]);
            exit(0);
        }
    }
    int pid;
    //PARENT PROCESS
    while ((pid = wait(NULL)) > 0) {
        read(pfds[0], buf, sizeof(buf));
        //Combining the array process
        int i = 0;
        //for every name in the array returned by the child
        while (buf[i].hasAName && i<100){
            //see if the name is already in the array
            int j = 0;
            while (listNames[j].hasAName && stringIsNotEqual(listNames[j].name,buf[i].name)){
                j++;
            }
            if (listNames[j].hasAName){
                listNames[j].occurrence += buf[i].occurrence;
            } else {
                strcpy(listNames[j].name,buf[i].name);
                listNames[j].occurrence = buf[i].occurrence;
                listNames[j].hasAName = true;
            }
            i++;
        }
        //close(pfds[0]); Strange behavior when place here
    }
    close(pfds[0]);
    int j = 0;
    while (listNames[j].hasAName && j<100) {
        printf("%s : %d\n", listNames[j].name, listNames[j].occurrence);
        j++;
    }
   // close(pfds[0]);
    return(0);
}

/**
 * This function compares two arrays of char and return true
 * if they are not exactly equal
 * Assumption: The first array has less than 30 characters,
 *             The second array has less than 30 characters
 * Input parameters: string, str
 * Return: a boolean
 **/
bool stringIsNotEqual(char string[MAX_LENGTH], char str[MAX_LENGTH]) {
    int i = 0;
    // This loop will terminate if we are about to have
    // an overflow or if the string is finished ('\0')
    while(i < MAX_LENGTH && string[i] != '\0') {
        if (string[i] != str[i]) {
            return true;
        }
        i++;
    }
    return false;
}
