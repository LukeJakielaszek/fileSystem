#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include "llist.h"
#include "utilities.h"
// error numbers
const int STAT_FAILURE = -2;

// filesystem information
#define DISK_SIZE 100
#define BLOCK_SIZE 3
#define INDICE_SIZE 8
int INDEX_SIZE; // size of index to represent all of memory 
int DATA_SIZE; // size of memory excluding index
int NUM_BLOCKS; // number of blocks within the data section
int ALLOC_DISK_SPACE; // Size of disk represented
char fileSystemName[50] = "fileSystem.txt"; // filesystem filename

struct Blocks{
  char data[BLOCK_SIZE];
};

struct Indices{
  char data[INDICE_SIZE];
};

// prototypes
void createFileSystem();
int invalidDiskParams();
void getDiskSizes();
void mapFileSystem(struct Blocks ** blocks, struct Indices ** indices);

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

  struct Blocks * blocks;
  struct Indices * indices;

  mapFileSystem(&blocks, &indices);

  printf("blocks:[%s]\n", blocks[0].data);

  printf("indices[%s]\n", indices[0].data);


  
  return 0;
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
