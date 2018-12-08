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

// filesystem information
#define DISK_SIZE 100000 // size in bytes of filesystem
#define BLOCK_SIZE 512 // size in bytes of blocks
#define INDICE_SIZE 8 // size in bytes of index
#define OPEN_INDEX -1 // indicates unused index location
#define EOF_INDEX -2 // indicates file ends at index location
#define EOF_CHAR 34 // character to signal final char in file
#define FILE_TYPE 70 // signifies file is a file F:70
#define DIRECTORY_TYPE 68 // signifies file is a dir D:68
#define META_SIZE 21 // size in bytes of meta section
#define WRITE 1 // mode to truncate and write to a file
#define READ 2 // mode to read a file
int INDEX_SIZE; // size of index to represent all of memory 
int DATA_SIZE; // size of memory excluding index
int NUM_BLOCKS; // number of blocks within the data section
int ALLOC_DISK_SPACE; // Size of disk represented
extern char fileSystemName[50]; // filesystem filename

// array of data section block size
struct Blocks{
  char data[BLOCK_SIZE];
};

// array of indice section index size
struct Indices{
  char data[INDICE_SIZE];
};

// file data structure
struct LFILE{
  int startBlock;
  int offset;
  char meta[META_SIZE];
  int mode;
};

// prototypes
void createFileSystem();
int invalidDiskParams();
void getDiskSizes();
void mapFileSystem(struct Blocks ** blocks, struct Indices ** indices);
int getOpenBlock(struct Indices * indices, struct Blocks * blocks);
int hexToInt(struct Indices index);
void claimIndex(struct Indices * indices, int openIndex);
char * createMeta(char type);
int createFile(char * dirName, int fileType,
		  struct Indices * indices, struct Blocks * blocks);
void writeData(int blockIndex, int offset, int curChar, char * data,
	       struct Blocks * blocks, struct Indices * indices);
void updateIndex(int indexLocation, int nextIndex, struct Indices * indices);
int readData(int blockIndex, int offset, int count, char * buffer,
	     int buffsize, struct Blocks * blocks, struct Indices * indices);
int isPathValid(char * path, struct Indices * indices, struct Blocks * blocks,
		int * dirBlock);
char * readFile(int startBlock, int offset, struct Blocks * blocks,
	       struct Indices * indices);
int openFile(char * filePath, int mode, struct LFILE ** file,
	     struct Indices * indices, struct Blocks * blocks);
int writeLFile(char * data, struct LFILE * file,
	       struct Indices * indices, struct Blocks * blocks);
int readLFile(char * buffer, int buffsize, struct LFILE * file,
	      struct Indices * indices, struct Blocks * blocks);
void closeLFile(struct LFILE * file);
int deleteFile(char * dirName,
		struct Indices * indices, struct Blocks * blocks);

#endif /** SERVER_H */
