#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int main(int argc,char *argv[])
{
    int ping[2];
    int pong[2];
    pipe(ping);
    pipe(pong);
    int fpid = fork();
    if(fpid < 0)
    {
        printf("Faild to create child process!\n");
        exit();
    }
    else if (fpid == 0)
    {
        int child_pid = getpid();
        char child_buf[100];
        read(ping[0],child_buf,sizeof(child_buf));
        printf("%d: received %s\n",child_pid,child_buf);
        write(pong[1],"pong",4);
        close(pong[1]);
        exit();
    }
    else
    {
        int father_pid = getpid();
        char father_buf[100];
        write(ping[1],"ping",4);
        read(pong[0],father_buf,sizeof(father_buf));
        printf("%d: received %s\n",father_pid,father_buf);
        close(ping[1]);
    }
    exit();
    
}