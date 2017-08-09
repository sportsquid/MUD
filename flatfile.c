#include <stdio.h>
#include <string.h>



int writeff(char * dbname, char * variable, char * data){
    char readval[50];
    char strcat_var[50];
    int i, getl;
    int filelength = 0;
    FILE * file;
    
    
    memset(strcat_var, '\0', sizeof(strcat_var));
    strcpy(strcat_var, dbname); 
    strcat(strcat_var, ".txt");
    //open file, see if it exists 
    file = fopen(strcat_var, "r+");
    if (!file){
        return(-1);
    }
    FILE * copy = fopen("copy.txt", "w");

    //see how long the file is
    while(fgets(readval, sizeof(readval), file) != NULL){
        filelength++;
    }
    printf("The file is %d lines long!\n", filelength);
   
    //set cursor to 0
    fseek(file, 0, SEEK_SET);

    //loop through the lines
    for(i = 0; i < filelength + 1; i++){
        
        fgets(readval, sizeof(readval), file);
        
        //compare each line to look for wanted variable
        memset(strcat_var, '\0', sizeof(strcat_var));
         
        strcpy(strcat_var, "~"); 
        strcat(strcat_var, variable);
        //see if the variable already exists
        if (strncmp(readval, strcat_var , sizeof(variable) + 1) == 0){
            fseek(file, sizeof(variable) + 1, SEEK_CUR);
            fprintf(file, " \n~%s %s\n", variable, data);
            puts("variable found and changed");
            fclose(file);
            fclose(copy);
            remove("copy.txt");
            return(1);
        } 
        //see if no match was found
        else if (i == filelength){
            fprintf(file, " \n~%s %s\n", variable, data);
            puts("Variable created");
            fclose(file);
            fclose(copy);
            remove("copy.txt");
            return(1);
        }
    }
    puts("failed for statment");
    fclose(copy);
    remove("copy.txt");
    return(-1);
}//end of writeff





int main(){
    int test = writeff("test", "meme_type", "dank");
    printf("%d\n", test);

}