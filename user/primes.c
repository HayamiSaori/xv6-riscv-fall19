#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"
void redirect(int port, int fd[])
{
    close(port);
    dup(fd[port]);
    close(fd[0]);
    close(fd[1]);
}

void judge(int n)
{
    int buf;
    while(read(0, &buf, sizeof(buf)))
    {
        if(buf % n != 0)
        {
            write(1, &buf, sizeof(buf));
        }
    }
    close(1);
}

void child_process()
{
    int fd[2];
    int buf;
    int fpid;
    if(read(0, &buf, sizeof(buf)))
    {
        printf("prime %d\n", buf);
        pipe(fd);
        fpid = fork();
        if(fpid == 0)
        {
            redirect(0, fd);
            child_process();    // 递归调用
        }
        else
        {
            redirect(1,fd);
            judge(buf);
        }
    }
}

int main(int argc,char *argv[])
{
    int fd[2];
    pipe(fd);
    int fpid = fork();
    if(fpid == 0)
    {
        redirect(0,fd);
        child_process();
    }
    else
    {
        for(int i=2;i<35;i++)
        {
            write(fd[1], &i, sizeof(i));
        }
        close(fd[1]);
        close(fd[0]);
    }
    wait();     // 等待子进程全部结束
    exit();
}