#include <stdio.h>

#include "utilities.h"

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
  //  createFileSystem();

  struct Blocks * blocks; // block pointers
  struct Indices * indices; //indice pointers

  // creates a filesystem mapping to memory
  mapFileSystem(&blocks, &indices);

  char path[] = "/datahogunited/potataoemanw";
  
  createFile(path, DIRECTORY_TYPE, indices, blocks);
  
  struct LFILE * file;

  /*
  if(openFile("/", WRITE, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    char text[50] = "mouse";  
    
    char tst[50] = "humpback whale";
    
    writeLFile(text, file, indices, blocks);
    
    writeLFile(tst, file, indices, blocks);
    writeLFile(text, file, indices, blocks);
    writeLFile(text, file, indices, blocks);
    writeLFile(text, file, indices, blocks);
    
      
    int buffsize = 10;
    char * buf = (char*)malloc(sizeof(char)*buffsize);
    
    int count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
	
    printf("offset %d : [%s]\n", file->offset, buf);
    
    closeLFile(file);
  }
  */
  
  //  deleteFile("/datahogunited/tropicalbaannaan", indices, blocks);
  
  // get contents of directory
  char * contents = readFile(0, 0, blocks, indices);
  printf("contents : \n[%s]\n", contents);

  munmap(indices, ALLOC_DISK_SPACE);
  
  return 0;
}
