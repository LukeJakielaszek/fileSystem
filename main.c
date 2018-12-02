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
#define DISK_SIZE 100 // size in bytes of filesystem
#define BLOCK_SIZE 3 // size in bytes of blocks
#define INDICE_SIZE 8 // size in bytes of index
#define OPEN_INDEX -1 // indicates unused index location
#define EOF_INDEX -2 // indicates file ends at index location
#define FILE_TYPE 70 // signifies file is a file F:70
#define DIRECTORY_TYPE 68 // signifies file is a dir D:68
#define META_SIZE 21 // size in bytes of meta section
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
  int offSet;
  int parentBlock;
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
struct LFILE * createDir(char * dirName,
		  struct Indices * indices, struct Blocks * blocks);
void writeData(int blockIndex, int offset, int curChar, char * data,
	       struct Blocks * blocks, struct Indices * indices);

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

  createDir("", indices, blocks);
  return 0;
}

// create a directory with only meta data
struct LFILE * createDir(char * dirName,
			 struct Indices * indices, struct Blocks * blocks){
  // find and claim the next open block
  int open = getOpenBlock(indices, blocks);

  printf("available block at [%d]\n", open);  

  char * meta = createMeta(FILE_TYPE);

  printf("[%s]\n", meta);

  // checks if root directory
  if(open == 0){
    writeData(open, 0, 0, meta, blocks, indices);
  }
}

// recursively write data to files, getting new block if neccessary
void writeData(int blockIndex, int offset, int curChar, char * data,
	       struct Blocks * blocks, struct Indices * indices){
  int i = 0;
  for(i = 0; curChar <= strlen(data)-1; i++, curChar++){
    if(i+offset >= BLOCK_SIZE && i != 0){
      // block is full
      // find and claim the next open block
      int open = getOpenBlock(indices, blocks);

      writeData(open, 0, curChar, data, blocks, indices);
      break;
    }
    printf("%d:%d\n", blockIndex, curChar);

    // overwrite block char with new data
    blocks[blockIndex].data[i] = data[curChar];
  }
}

// creates a string representation of meta data
char * createMeta(char type){
  // gets date and time
  time_t curTime = time(0);
  struct tm* tm_info = localtime(&curTime);
  char * buffer = (char *)malloc(sizeof(char)*META_SIZE+1);
  strftime(buffer, META_SIZE, "%Y-%m-%d %H:%M:%S", tm_info);

  // adds filetype to string
  buffer[META_SIZE-2] = ' ';
  buffer[META_SIZE-1] = type;
  buffer[META_SIZE] = '\0';

  return buffer;
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
