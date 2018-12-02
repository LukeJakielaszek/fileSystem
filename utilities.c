// prototypes
void createDir(char * dirName,
	       struct Indices * indices, struct Blocks * blocks);

// functions
void createDir(char * dirName,
	       struct Indices * indices, struct Blocks * blocks){
  time_t curTime = time(0);
  printf("%ld\n", curTime);

  struct tm* tm_info = localtime(&curTime);
  char buffer[26];
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  
  printf("%s\n", buffer);
}
