#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>
 
#define BUFFER_LENGTH 12 
static char receive[BUFFER_LENGTH];
 
int main(){
   int ret, fd;

   fd = open("/dev/RandomDevice", O_RDWR); 
   if (fd < 0){
      perror("Failed to open the device...");
      return -1;
   }
 
   printf("\nReading from the device...\n");
   ret = read(fd, receive, BUFFER_LENGTH);
   if(ret < 0){
       printf("Failed\n");
       return -1;
   }
   else{
       printf("Random number is %s\n\n",receive);
   }
   return 0;
}
