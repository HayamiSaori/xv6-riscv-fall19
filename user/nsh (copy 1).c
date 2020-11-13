#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#define MAX_CMD_NUM 32
#define MAX_CMD_ARGC 8
#define MAX_ARGV_LEN 32
#define MAX_CMD_LEN 512
void redirect(int port, int fd[])
{
    close(port);
    dup(fd[port]);
    close(fd[0]);
    close(fd[1]);
}
// 除了打印的符号为@，其余的与sh.c的getcmd()相同
int
getcmd(char *buf, int nbuf)
{
  fprintf(2, "@ ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

int main(void)
{
    char buf[MAX_CMD_LEN];
    getcmd(buf,MAX_CMD_LEN);
    fprintf(1,"%s",buf);
    exit(0);
}