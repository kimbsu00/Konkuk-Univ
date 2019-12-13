#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

static int fd;

void my_timer_callback(unsigned long data)
{
   close(fd);
//   printf("베팅 시간이 초과되었습니다. .\n");
   exit(1);
}
int main(int argc, char *argv[])
{

   int fd = open("./test.dat", O_RDONLY);
    if(fd==-1)
        printf("open_error\n");
    char buf[1];
    int size = 0;

   struct itimerval set_time_val, get_time_val;
   sigset(SIGALRM, my_timer_callback);

   set_time_val.it_value.tv_sec = 5;
   set_time_val.it_value.tv_usec = 0;

   set_time_val.it_interval.tv_sec = 5;
   set_time_val.it_interval.tv_usec = 0;

   if (setitimer(ITIMER_REAL, &set_time_val, NULL) == -1)
   {
      perror("Failed to set virtual timer!\n");
      return -1;
   }
   while (getitimer(ITIMER_REAL, &get_time_val) != -1)
   {
      lseek(fd, 0, SEEK_SET);
      size = 0;
      while (size += read(fd, buf, 1), size == -1 && errno == EINTR){
         if(buf[0] == '\0')
            break;
      }

      printf("\033[%d;%df", 19, 35);
      printf("time : %d", get_time_val.it_value.tv_sec);
      printf("\033[%d;%df", 20, 35);
      printf("Please bet!(enter integer)");
      fflush(stdout);
      printf("\033[%d;%df", 21, 35+size);
      fflush(stdout);
      sleep(1);
   }
   return 1;
}
