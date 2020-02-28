#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct page{
    int dirtyBit;
    unsigned address;
} Page;
/*
    random page replacement function
*/
void RDM(char* filename, int frame_size, char* mode);


int main(int argc, char* argv[]){
    // check to make sure proper amount of arguments were provided to the program
    if(argc < 4){
        fprintf(stderr, "Argument error, provide proper arguments");
    }

    // grabbing the frames from the input
    int frames = atoi(argv[2]);

    // DEBUG for input arguments
    // for(int i = 0; i<5; i++){
    //     printf("%s\n", argv[i]);
    // }


   
    if(strcmp(argv[3], "rdm") == 0){
        RDM(argv[1], frames, argv[4]);
        exit(0);
    }else if(strcmp(argv[3], "lru") == 0){
        printf("Test success lru");
        exit(0);
    }else if(strcmp(argv[3], "fifo") == 0){
        printf("Test success fifo");
        exit(0);
    }else if(strcmp(argv[3], "vms") == 0){
        printf("Test success vms");
        exit(0);
    }
    return 0;
}

void RDM(char* filename, int frame_size, char* mode){
    /*
        Function to manually replace pages within computer RAM
        and allocate necessary bits to creation of new pages based on running
        program memory needs 
    */

    srand(time(0));
    Page frames[frame_size];

    // initialize the pages to some null values
    for(int i = 0; i<frame_size; i++){
        frames[i].address = -1;
        frames[i].dirtyBit = -1;
    }
    unsigned addr;
    char rw;
    int reads = 0, writes = 0, events = 0;
    FILE* file = fopen(filename, "r");
    int previous_page = 0;
    bool found;
    while(fscanf(file,"%x %c",&addr,&rw) != EOF){

        if(strcmp(mode, "debug") == 0){
            printf("Performing Event: %d, %c, %d\n", events, rw, addr);
        }


        
        found = false;
        // immediately increment the events variable if we find another trace to read
        events++;

        // if a read event is happening
        if(rw == 'R'){
            // check the previously accessed frame first
            if(frames[previous_page].address == addr){
                found = true;
                continue;
            }

            for(int i = 0; i < frame_size; i++){
                // if we find the relevant page, continue
                // and set previous page to the index we found it at
                // this is to take advantage of principle of locality
                // to avoid iterating through the entire array for multiple
                // reads of the same page
                if(frames[i].address == addr){
                    previous_page = i;
                    found = true;
                    break;
                }
            }

            // if the page was not found, evict a random page
            if(!found){
                int evict = rand() % frame_size;
                Page newPage;
                newPage.address = addr;
                // if the random page to be evicted has been modified
                // increment writes and read
                if(frames[evict].dirtyBit == 1){
                    writes++;
                    reads++;
                    frames[evict] = newPage;
                }else{
                    // if the page to be evicted has not beed modified, just increment reads
                    reads++;
                    frames[evict] = newPage;
                }
            }
        }else if(rw == 'W'){
            // look for the page, and if it is found, change its dirty bit
            if(frames[previous_page].address == addr){
                found = true;
                frames[previous_page].dirtyBit = 0;
                continue;
            }

            for(int i = 0; i < frame_size; i++){

                // if we find the relevant page, change its dirty bit
                // and set previous page to the index we found it at
                // this is to take advantage of principle of locality
                // to avoid iterating through the entire array for multiple
                // reads of the same page
                if(frames[i].address == addr){
                    previous_page = i;
                    found = true;
                    frames[previous_page].dirtyBit = 0;
                    break;
                }
            }

            // if the page was not found, evict a random page
            // increment read, then change the dirty bit
            if(!found){
                int evict = rand() % frame_size;
                Page newPage;
                newPage.address = addr;
                newPage.dirtyBit = 1;
                // if the random page to be evicted has been modified
                // increment writes and read
                if(frames[evict].dirtyBit == 1){
                    writes++;
                    reads++;
                    frames[evict] = newPage;
                }else{
                    // if the page to be evicted has not beed modified, just increment reads
                    reads++;
                    frames[evict] = newPage;
                }
            }
        }


    }
    fclose(file);
    printf("Total memory frames: %d\n", frame_size);
    printf("Events in trace: %d\n", events);
    printf("Total disk reads: %d\n", reads);
    printf("Total disk writes: %d\n", writes);
}