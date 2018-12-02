// standard c libraries
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

// filesystem information
#define DISK_SIZE 100
#define BLOCK_SIZE 3
#define INDICE_SIZE 8
#define OPEN_INDEX -1
#define EOF_INDEX -2
int INDEX_SIZE; // size of index to represent all of memory 
int DATA_SIZE; // size of memory excluding index
int NUM_BLOCKS; // number of blocks within the data section
int ALLOC_DISK_SPACE; // Size of disk represented
char fileSystemName[50] = "fileSystem.txt"; // filesystem filename

// array of data section block size
struct Blocks{
  char data[BLOCK_SIZE];
};

// array of indice section index size
struct Indices{
  char data[INDICE_SIZE];
};

// user-defined libraries
#include "llist.h"
#include "utilities.h"

// prototypes
void createFileSystem();
int invalidDiskParams();
void getDiskSizes();
void mapFileSystem(struct Blocks ** blocks, struct Indices ** indices);
int getOpenBlock(struct Indices * indices, struct Blocks * blocks);
int hexToInt(struct Indices index);
void claimIndex(struct Indices * indices, int openIndex);

int main(){
  // check validity of entered parameters
  if(invalidDiskParams()){
    fprintf(stderr, "ERROR: Invalid Disk Parameters Detected.\n");
    exit(-1);
  }

  // calculate maximum used memory and # of blocks
  getDiskSizes();
  
  printf("DISK_SIZE %d\n", DISK_SIZE);
  printf("ALLOC_DISK_SPACE %d\n\n", ALLOC_DISK_SPACE);

  printf("NUM_BLOCKS %d\n\n", NUM_BLOCKS);
  
  printf("INDICE_SIZE %d\n", INDICE_SIZE);
  printf("INDEX_SIZE %d\n\n", INDEX_SIZE);
  
  printf("BLOCK_SIZE %d\n", BLOCK_SIZE);
  printf("DATA_SIZE %d\n", DATA_SIZE);

  // creates a filesystem with allocation array initialized to empty (-1)
  createFileSystem();

  struct Blocks * blocks; // block pointers
  struct Indices * indices; //indice pointers

  // creates a filesystem mapping to memory
  mapFileSystem(&blocks, &indices);

  // find and claim the next open block
  int open = getOpenBlock(indices, blocks);

  printf("available block at [%d]\n", open);
  
  return 0;
}

// returns first available block index, -1 if none are available
int getOpenBlock(struct Indices * indices, struct Blocks * blocks){
  int retIndex = -1;
  
  // search through indices of filesystem
  int index = 0;
  while(index < NUM_BLOCKS){
    char buffer[INDICE_SIZE];

    // check if index is available
    if(hexToInt(indices[index]) == OPEN_INDEX){
      retIndex = index;
      break;
    }
    
    index++;
  }

  // claim available index
  claimIndex(indices, retIndex);

  // return index location
  return retIndex;
}

// claims index by writing claimedIndex hex val to filesystem indice section
void claimIndex(struct Indices * indices, int openIndex){
  // create hex string for claim value
  char buffer[INDICE_SIZE];
  sprintf(buffer, "%08X", EOF_INDEX);

  // overwrite index value with claimvalue
  int i = 0;
  for(i = 0; i < INDICE_SIZE; i++){
    indices[openIndex].data[i] = buffer[i];
  }
}

// converts hex to int from indice struct data
int hexToInt(struct Indices index){
  char buffer[INDICE_SIZE];

  // store index value in buffer
  int i = 0;
  for(i = 0; i < NUM_BLOCKS-1; i++){
    buffer[i] = index.data[i];
  }

  buffer[i] = '\0';

  // convert hex chars to int
  return (int)strtol(buffer, NULL, 16);
}

void mapFileSystem(struct Blocks ** blocks, struct Indices ** indices){
  int fd;
  struct stat stat_buff;

  // opens new filesystem file, checks for success
  if((fd = open(fileSystemName, O_RDWR)) < 0){
    fprintf(stderr, "ERROR: Failed to open %s.\n", fileSystemName);
    exit(EXIT_FAILURE);
  }

  // mmap filesystem
  if((*indices = (struct Indices *)mmap(NULL, ALLOC_DISK_SPACE,
				     PROT_WRITE|PROT_READ, MAP_SHARED,
				     fd, 0)) == MAP_FAILED){
    fprintf(stderr, "ERROR: Failed to map %s: %s.\n", fileSystemName,
	    strerror(errno));
    exit(EXIT_FAILURE);
  }

  // set block pointer to offset
  *blocks = (struct Blocks *)((*indices)+NUM_BLOCKS);
}

// creates initial filesystem file
void createFileSystem(){
  printf("\nConstructing File System...\n");

  int fd;

  // opens new filesystem file, checks for success
  if((fd = open(fileSystemName, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
    fprintf(stderr, "ERROR: Failed to open %s.\n", fileSystemName);
    exit(EXIT_FAILURE);
  }

  // forces file into a specific size
  if(ftruncate(fd, ALLOC_DISK_SPACE)!=0){
    fprintf(stderr, "ERROR: Failed to truncate %s.\n", fileSystemName);
    exit(EXIT_FAILURE);
  }
  
  close(fd);

  FILE * f = fopen(fileSystemName, "r+");

  // initialize allocation array to empty
  int i;
  for(i = 0; i < NUM_BLOCKS; i++){
    fprintf(f, "%08X", -1);
  }

  for(i = 0; i < DATA_SIZE; i++){
    fprintf(f, "0");
  }
  
  fclose(f);
  
  printf("File System Constructed\n\n");
}

// check validity of disk parameters
int invalidDiskParams(){
  int isInvalid = 0;

  // ensures negative values are not entered
  if(DISK_SIZE <=0 || BLOCK_SIZE <= 0 || INDICE_SIZE <= 0){
    isInvalid = 1;
  }

  // ensures at least one block can be created
  if(BLOCK_SIZE + INDICE_SIZE > DISK_SIZE){
    isInvalid = 1;
  }
  
  return isInvalid;
}

// find disk sizings
void getDiskSizes(){
  NUM_BLOCKS = (int)floor(DISK_SIZE/(BLOCK_SIZE+INDICE_SIZE));

  INDEX_SIZE = NUM_BLOCKS * INDICE_SIZE;
  DATA_SIZE = NUM_BLOCKS * BLOCK_SIZE;

  ALLOC_DISK_SPACE = DATA_SIZE + INDEX_SIZE;
}
