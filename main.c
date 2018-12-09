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
  createFileSystem();

  struct Blocks * blocks; // block pointers
  struct Indices * indices; //indice pointers

  // creates a filesystem mapping to memory
  printf("Mapping file system...\n");
  mapFileSystem(&blocks, &indices);
  printf("File system mapped\n");

  // file struct
  struct LFILE * file;

  // file reading buffers
  int buffsize = 10;
  char * buf = (char*)malloc(sizeof(char)*buffsize);
  int count = -1;

  // open root, print its contents
  printf("\nInitial Root Directory contents:\n");
  if(openFile("/", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");
  
  
  // attempt making invalid directory
  char *path = "/datahogunited/potataoemanw";
  printf("Creating invalid directory [%s]\n", path);
  
  createFile(path, DIRECTORY_TYPE, indices, blocks);

  // print roots contents to screen
  printf("Root Directory contents:\n");
  if(openFile("/", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");

  // attempt making valid directory
  path = "/FinishedThisLab";
  printf("Creating valid directory [%s]\n", path);
  
  createFile(path, DIRECTORY_TYPE, indices, blocks);

  // print roots contents to screen
  printf("Root Directory contents:\n");
  if(openFile("/", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");

  // attempt making valid directory
  path = "/datahogunited";
  printf("Creating valid directory [%s]\n", path);
  
  createFile(path, DIRECTORY_TYPE, indices, blocks);

  // print roots contents to screen
  printf("Root Directory contents:\n");
  if(openFile("/", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");

  // attempt making valid subdirectory
  path = "/datahogunited/thisIsAmerica";
  printf("Creating valid subdirectory [%s]\n", path);
  
  createFile(path, DIRECTORY_TYPE, indices, blocks);

  // print roots contents to screen
  printf("Root Directory contents:\n");
  if(openFile("/", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  // print first directory contents to screen
  printf("First Directory contents:\n");
  if(openFile("/datahogunited", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }

  // attempt making valid file
  path = "/datahogunited/thisIsAmerica/thisIsatestfile.txt";
  printf("Creating valid file in subdirectory [%s]\n", path);
  
  createFile(path, FILE_TYPE, indices, blocks);

  // print file's parent directory contents
  printf("Parent Directory contents:\n");
  if(openFile("/datahogunited/thisIsAmerica", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }  
  printf("file contents:\n");

  // print file contents to screen
  if(openFile("/datahogunited/thisIsAmerica/thisIsatestfile.txt", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");

  //write to created file
  printf("Writing to file %s...\n", path);
  if(openFile(path, WRITE, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    writeLFile("Johhny went down to georgia\n", file, indices, blocks);
    
    writeLFile("To eat an apple pie\n", file, indices, blocks);
    
    closeLFile(file);
  }  
  printf("Completed writing\n");

  // print file contents to screen
  printf("file contents:\n");
  if(openFile(path, READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");

  // try to delete a directory with contents in it
  path = "/datahogunited/thisIsAmerica";
  printf("Attempting to delete non-empty directory [%s]\n", path);
  deleteFile(path, indices, blocks);

  // delete file
  path = "/datahogunited/thisIsAmerica/thisIsatestfile.txt";
  printf("\nAttempting to delete file [%s]\n", path);
  deleteFile(path, indices, blocks);

  // attempt to print deleted file contents
  printf("deleted file contents:\n");
  if(openFile(path, READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }

  // print parent directory contents
  path = "/datahogunited/thisIsAmerica";
  printf("Parent Directory contents [%s]:\n", path);
  if(openFile(path, READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }  
  printf("\n");

  //create another file within parent directory
  path = "/datahogunited/thisIsAmerica/thisIsAnotherTest.txt";
  printf("Creating another file [%s] within parent directory\n", path);
  createFile(path, FILE_TYPE, indices, blocks);

  // print file's parent directory contents
  printf("Parent Directory contents:\n");
  if(openFile("/datahogunited/thisIsAmerica", READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }  
  printf("file contents:\n");

  // print file contents to screen
  if(openFile(path, READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");

  //write to created file
  printf("Writing to file %s...\n", path);
  if(openFile(path, WRITE, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    writeLFile("Hello Darkness my old friend\n", file, indices, blocks);
    
    closeLFile(file);
  }  
  printf("Completed writing\n");

  // print file contents to screen
  printf("file contents:\n");
  if(openFile(path, READ, &file, indices, blocks)){
    printf("block %d, offset : %d, mode %d, meta [%s]\n",
    	   file->startBlock, file->offset, file->mode, file->meta);
    
    count = readLFile(buf, buffsize, file, indices, blocks);
    
    while(count > 0){
      printf("offset %d : [%s]\n", file->offset, buf);
      count = readLFile(buf, buffsize, file, indices, blocks);
    }
    
    closeLFile(file);
  }
  printf("\n");

  // unmaps the filesystem
  munmap(indices, ALLOC_DISK_SPACE);
  
  return 0;
}
