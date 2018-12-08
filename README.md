# fileSystem
A simulation of a fat file system.

void createFileSystem();
     creates a filesystem file instantiated with a root directory. All
     indice values are initialized to -1 in hex. All data values are
     initialized to the ascii file seperator character.

int invalidDiskParams();
    // checks if disk dimensions are invalid

void getDiskSizes();
     // calculates disk dimensions

void mapFileSystem(struct Blocks ** blocks, struct Indices ** indices);
     // mmaps the filesystem, splitting it into two sections (index and blocks)

int getOpenBlock(struct Indices * indices, struct Blocks * blocks);
    // searches the index for the first available block (-1 in hex)
    
int hexToInt(struct Indices index);
    converts a hex string to an integer
    
void claimIndex(struct Indices * indices, int openIndex);
    claims an available index with EOF. 

char * createMeta(char type);
     creates meta data in the form of date filetype

int createFile(char * dirName, int fileType,
		  struct Indices * indices, struct Blocks * blocks);
    creates an empty file and initializes it with meta data. Adds file to the
    directory structure.
    
void writeData(int blockIndex, int offset, int curChar, char * data,
	       struct Blocks * blocks, struct Indices * indices);
    writes data to a file following its block structure. If file ends before
    finishing, grabs a new block
    
void updateIndex(int indexLocation, int nextIndex, struct Indices * indices);
    Updates files previous index to point to new index

int readData(int blockIndex, int offset, int count, char * buffer,
	     int buffsize, struct Blocks * blocks, struct Indices * indices);
    Reads data from a file following its block structure. Returns number of
    chars read
    
int isPathValid(char * path, struct Indices * indices, struct Blocks * blocks,
		int * dirBlock);
    Searches directory structure to determine if a path exists.
    
char * readFile(int startBlock, int offset, struct Blocks * blocks,
	       struct Indices * indices);
    Reads entire file contents. Useful for debugging and editing directories
    
int openFile(char * filePath, int mode, struct LFILE ** file,
	     struct Indices * indices, struct Blocks * blocks);
    open a file in a specified mode and store pertinent file information in a
    LFILE struct
    
int writeLFile(char * data, struct LFILE * file,
	       struct Indices * indices, struct Blocks * blocks);
    write to a file that was previously opened. Built on top of writeData.

int readLFile(char * buffer, int buffsize, struct LFILE * file,
	      struct Indices * indices, struct Blocks * blocks);
    Read from a file that was previously opened. Built on top of readData.
    
void closeLFile(struct LFILE * file);
    Close a previously opened file. (Frees the malloced file struct)

int deleteFile(char * dirName,
		struct Indices * indices, struct Blocks * blocks);
    Deletes a file, removing it from both the directory and index filesystem
    structure.
