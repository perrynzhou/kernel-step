/*************************************************************************
  > File Name: mem_channel_reader.c
  > Author:perrynzhou 
  > Mail:perrynzhou@gmail.com 
  > Created Time: Fri 26 Jun 2020 01:35:15 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define BUFFER_LENGTH 128

int main(int argc, char *argv[])
{

  int fd = open(argv[1], O_RDWR);
  if (fd < 0)
  {
    fprintf(stdout, "%s,err:%s\n", argv[1], strerror(errno));
    return -1;
  }
  char *buffer = (char *)malloc(BUFFER_LENGTH);
  memset(buffer, 0, BUFFER_LENGTH);

  char *start = mmap(NULL, BUFFER_LENGTH, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  fd_set rds;

  FD_ZERO(&rds);
  FD_SET(fd, &rds);

  while (1)
  {
    int ret = select(fd + 1, &rds, NULL, NULL, NULL);
    if (ret < 0)
    {
      printf("select error\n");
      exit(1);
    }
    if (FD_ISSET(fd, &rds))
    {
#if 0
			strcpy(buffer, start);
			printf("ntychannel: %s\n", buffer);
#else
      read(fd, buffer, BUFFER_LENGTH);
      printf("channel: %s\n", buffer);
#endif
    }
  }

  munmap(start, BUFFER_LENGTH);
  free(buffer);
  close(fd);

  return 0;
}
