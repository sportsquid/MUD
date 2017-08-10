#include <stdio.h>
#include <string.h>
#include "flatfile.h"


//write to flat file
int writeff(char * dbname, char * variable, char * data){
    char readval[50];
    char strcat_var[50];
    char filename[50];
    int i;
    int total_size = 0;
    long filelength = 0;
    int found = 0;
    FILE * file;


    memset(strcat_var, '\0', sizeof(strcat_var));
    strcpy(strcat_var, dbname);
    strcat(strcat_var, ".txt");
    strcpy(filename, strcat_var);
    //open file, see if it exists
    file = fopen(strcat_var, "r+");
    if (!file){
        return(-1);
    }

    FILE * copy = fopen("copy.txt", "w");

    //see how long the file is
		filelength = getfilelength(file);

    //printf("file is %d lines long\n", filelength);
    //set cursor to 0
    fseek(file, 0, SEEK_SET);

    //loop through the lines to find match
    i = 0;
    do{
        //reset char arrays
        memset(strcat_var, '\0', sizeof(strcat_var));
        memset(readval, '\0', sizeof(readval));
        //read line
        fgets(readval, sizeof(readval), file);


        //get strings ready
        strcpy(strcat_var, "~");
        strcat(strcat_var, variable);


        //printf("iteration: %d\n", i);
        //printf("%s", readval);
        //see if the variable already exists
        if (strncmp(readval, strcat_var , strlen(variable)+ 1) == 0){

            found = 1;
            fprintf(copy, "~%s %s\n", variable, data);
            //puts("variable found and changed");

        }
        //see if no match was found
        else if (i == filelength && !found){

            fprintf(copy, "~%s %s\n", variable, data);
             //puts("reached end of file");

        } else{
            fputs(readval, copy);
            //puts("copied existing");
        }

        i++;
    } while (i <= filelength);//end do while

            fclose(file);
            fclose(copy);
            rename("copy.txt", filename);
            remove("copy.txt");
            return(1);
}//end of writeff

//read flatfile
int readff(char * dbname, char * variable, char * data){

    char strcat_var[50];
    char filename[50];
    char readval[50];
    char c[1];
    int j;
    int total_size = 0;
    long filelength = 0, i;

    FILE * file;

    //zero out strings, set up filename
    memset(strcat_var, '\0', sizeof(strcat_var));
    memset(data, '\0', sizeof(data));
    strcpy(strcat_var, dbname);
    strcat(strcat_var, ".txt");
    strcpy(filename, strcat_var);
    memset(strcat_var, '\0', sizeof(strcat_var));
    //open file, see if it exists
    file = fopen(filename, "r");
    if (!file){
        return(-1);
    }



    //see how long the file is
    filelength = getfilelength(file);

    //move back to begginign of file to look for variable
    fseek(file, 0, SEEK_SET);
    strcat(strcat_var, "~");
    strcat(strcat_var, variable);
    //loop through every line
    for (i = 0; i <= filelength; i++){
        fgets(readval, sizeof(readval), file);
        total_size += strlen(readval);

        if (strncmp(strcat_var, readval, strlen(strcat_var))==0){
            total_size -= strlen(readval);
            fseek(file, total_size + strlen(variable) +2, SEEK_SET);
            //re read line
            fgets(readval, sizeof(readval), file);
            j = 0;
            //wait for end of line
            while(readval[j] != '\n'){
                j++;
            }

            memcpy(data, readval, j);
            data[j] = '\0';

            break;
        }
    }
    fclose(file);
    return(0);

}

int createff(char * name){
    char filename[50];
    FILE *create;
    strcpy(filename, name);
    strcat(filename, ".txt");
    create = fopen(filename, "w");
    fclose(create);
    return(0);
}

long getfilelength(FILE * file) {
	long ret = 0;

	fseek(file, 0, SEEK_END);
	ret = ftell(file);
	rewind(file);

	return ret;
}
