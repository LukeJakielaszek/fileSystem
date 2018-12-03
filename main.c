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
#define BLOCK_SIZE 5 // size in bytes of blocks
#define INDICE_SIZE 8 // size in bytes of index
#define OPEN_INDEX -1 // indicates unused index location
#define EOF_INDEX -2 // indicates file ends at index location
#define EOF_CHAR 34 // character to signal final char in file
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

// file data structure
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
void updateIndex(int indexLocation, int nextIndex, struct Indices * indices);
int readData(int blockIndex, int offset, int count, char * buffer,
	     int buffsize, struct Blocks * blocks, struct Indices * indices);
int isPathValid(char * path, struct Indices * indices, struct Blocks * blocks);
char * readFile(int startBlock, int offset, struct Blocks * blocks,
	       struct Indices * indices);

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

  char str[80] = "/hello/world/boy.txt";

  char * buf = (char*)malloc(sizeof(char)*50);
  
  int count = readData(0,0, 0, buf, 50, blocks, indices);

  char path[] = "/data/isip/luke.txt";
  
  isPathValid(path, indices, blocks);

  munmap(indices, ALLOC_DISK_SPACE);
  
  return 0;
}

// create a directory with only meta data
struct LFILE * createDir(char * dirName,
			 struct Indices * indices, struct Blocks * blocks){
  // find and claim the next open block
  int open = getOpenBlock(indices, blocks);

  char * meta = createMeta(DIRECTORY_TYPE);

  // checks if root directory
  if(open == 0){
    writeData(open, 0, 0, meta, blocks, indices);
  }else if(open == -1){
    // checks if unable to find next block
    fprintf(stderr, "ERROR: Disk Full. Unable to create directory [%s]",
	    dirName);
    exit(EXIT_FAILURE);
  }else{
    
  }
}

// checks if path exists, returns 1 on success 0 on failure
int isPathValid(char * path, struct Indices * indices, struct Blocks * blocks){
  // check if path is empty
  if(path == NULL){
    return 0;
  }
  
  // ensures absolute path
  if(path[0] != '/'){
    return 0;
  }

  char delim[] = "/";
  char * token;

  int startBlock = 0;
  int offset = 0;

  char * total = readFile(startBlock, offset, blocks, indices);

  printf("[%s]\n", total);
  
  /*
  token = strtok(path, delim);

  while(token != NULL){
    printf("%s\n", token);
    token = strtok(NULL, delim);
  }
  */
  return 1;
}

char * readFile(int startBlock, int offset, struct Blocks * blocks,
	       struct Indices * indices){
  // temporary buffer for reading sections
  int buffsize = 50;
  char * buffer = (char*)malloc(sizeof(char)*buffsize);

  // total size of directory
  int totalSize = buffsize;
  char * total = (char*)malloc(sizeof(char)*totalSize);
  total[0] = '\0';
  
  int count = 0;  
  // read directory until end
  do{
    // partially read directory
    count = readData(startBlock, offset, 0, buffer, buffsize,
	     blocks, indices);

    // increment offset
    offset += count;

    // if total fills up, realloc more space
    if(offset <= totalSize){
      totalSize *= 2;
      total = (char*)realloc(total, sizeof(char)*totalSize);
    }

    // concatenate buffer into total
    strcat(total, buffer);
  }while(count == buffsize-1);

  free(buffer);
  
  return total;
}

// reads data until buffer is full or file ends, returns number of characters
// read
int readData(int blockIndex, int offset, int count, char * buffer,
	      int buffsize, struct Blocks * blocks, struct Indices * indices){
  // reads data to buffer at char offset
  int i = offset;
  for(i = offset; count < buffsize-1; i++, count++){
    // if hit end of file
    if(blocks[blockIndex].data[i] == EOF_CHAR){
      break;
    }
    
    // if block is full
    if(i >= BLOCK_SIZE){
      // get value in indice
      int nextBlock = hexToInt(indices[blockIndex]);
      
      // if index was last block in file
      if(nextBlock == EOF_INDEX){
	// return total chars read
	return count;
      }

      // calculates new offset ensuring it is always >= 0
      int newOffset = offset - BLOCK_SIZE;
      if(newOffset < 0){
      	newOffset = 0;
      }
      
      // continue reading from next block
      count = readData(nextBlock, newOffset, count, buffer,
		       buffsize, blocks, indices);
      break;
    }
    // store block char in buffer
    buffer[count] = blocks[blockIndex].data[i];
  }

  // add null to end of buffer
  buffer[count] = '\0';

  // return total number of chars read
  return count;
}

// updates indexLocation to point to next Index 
void updateIndex(int indexLocation, int nextIndex, struct Indices * indices){
  // converts nextIndex to hex
  char buffer[INDICE_SIZE];
  sprintf(buffer, "%08X", nextIndex);

  // copies nextIndex into indexLocation
  int i = 0;
  for(i = 0; i < INDICE_SIZE; i++){
    indices[indexLocation].data[i] = buffer[i];
  }
}

// recursively write data to files, getting new block if neccessary
void writeData(int blockIndex, int offset, int curChar, char * data,
	       struct Blocks * blocks, struct Indices * indices){
  // writes data to file at char offset
  int i = offset;
  for(i = offset; curChar < strlen(data); i++, curChar++){
    // if block is full
    if(i >= BLOCK_SIZE){
      // get value in indice
      int open = hexToInt(indices[blockIndex]);
      
      // if index was last block in file
      if(open == EOF_INDEX){
	// find and claim the next open block
	open = getOpenBlock(indices, blocks);

	// if there is another block available, point to it
	updateIndex(blockIndex, open, indices);
      }

      // calculates new offset ensuring it is always >= 0
      int newOffset = offset - BLOCK_SIZE;
      if(newOffset < 0){
      	newOffset = 0;
      }
      
      // continue writing data to next block
      writeData(open, newOffset, curChar, data, blocks, indices);
      break;
    }

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

  if(retIndex != -1){
    // claim available index
    claimIndex(indices, retIndex);
  }else{
    // checks if unable to find next block
    fprintf(stderr, "ERROR: Disk Full. Unable to find open index\n");
    exit(EXIT_FAILURE);
  }
  
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
  for(i = 0; i < INDICE_SIZE; i++){
    buffer[i] = index.data[i];
  }

  buffer[i] = '\0';
  
  // convert hex chars to int
  return (int)strtol(buffer, NULL, 16);
}

// mmap filesystem file to memory
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
    fprintf(f, "%c", EOF_CHAR);
  }
  
  fclose(f);

  struct Blocks * blocks; // block pointers
  struct Indices * indices; //indice pointers

  // creates a filesystem mapping to memory
  mapFileSystem(&blocks, &indices);

  createDir("", indices, blocks);

  // unmap filesystem
  munmap(indices, ALLOC_DISK_SPACE);
  
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
