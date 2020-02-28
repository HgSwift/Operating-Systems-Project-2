#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    random page replacement function
*/
void RDM_page_rep(char* filename, int frame_size);


int main(int argc, char* argv[]){
    // check to make sure proper amount of arguments were provided to the program
    // if(argc < 2){
    //     fprintf(stderr, "Argument error, provide proper arguments");
    // }

    unsigned addr;
    char rw;
    FILE* file = fopen("bzip.trace", "r");
    fscanf(file,"%x %c",&addr,&rw);
    fclose(file);
    RDM_page_rep("bzip.trace", 100);
    return 0;
}

void RDM_page_rep(char* filename, int frame_size){
    /*
        Function to manually replace pages within computer RAM
        and allocate necessary bits to creation of new pages based on running
        program memory needs 
    */
    unsigned frames[frame_size];
    unsigned addr;
    char rw;
    FILE* file = fopen(filename, "r");
    int i = 0;
    while(fscanf(file,"%x %c",&addr,&rw)){
        // This loop will iterate over every page reference string within
        // a trace file - leaving page destruction and creation for the RDM algorithm
    }
    fclose(file);
}