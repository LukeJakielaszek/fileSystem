#include <stdio.h>
#include <time.h>

void createDir(char * dirName);

int main(){
  createDir("hello");

  return 0;
}

void createDir(char * dirName){
  time_t curTime = time(0);
  printf("%ld\n", curTime);

  struct tm* tm_info = localtime(&curTime);
  char buffer[26];
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  
  printf("%s\n", buffer);
}
