#ifndef UTILITIES_H
#define UTILITIES_H

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
#include <time.h>

#define BLOCK_SIZE 3
#define INDICE_SIZE 8
#define DISK_SIZE 100
#define OPEN_INDEX -1
#define EOF_INDEX -2
#define FILE_TYPE 10
#define DIRECTORY_TYPE 11

// filesystem information
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

struct LFILE{
  int startBlock;
  int curBlock;
  int charOffset;
};
  
// prototypes
void createDir(char * dirName,
	       struct Indices * indices, struct Blocks * blocks);
void createFileSystem();
int invalidDiskParams();
void getDiskSizes();
void mapFileSystem(struct Blocks ** blocks, struct Indices ** indices);
int getOpenBlock(struct Indices * indices, struct Blocks * blocks);
int hexToInt(struct Indices index);
void claimIndex(struct Indices * indices, int openIndex);

#endif /** UTILITIES_H */
