#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"
const int count = 35;
void child_process(int left_pipe[])
{
    char buf[2],num[2];
    int p,right_pipe[2],fpid = -2;
    read(left_pipe[0],buf,sizeof(buf));
    num[0]=buf[0];
    num[1]=buf[1];
    p = atoi(num);
    pipe(right_pipe);
    // dup(left_pipe[1]);
    // printf("the primes is %d\n",p);
    printf("prime %d\n",p);
    
    fpid = fork();
    if(fpid == 0)
    {
        printf("now start a child process\n");
        child_process(right_pipe);        
    }
    else
    {
        while(atoi(buf) <= count-1)
        {
            // printf("recieved %d from right\n",atoi(buf));
            read(left_pipe[0],buf,sizeof(buf));
            if(atoi(buf) % p != 0)
            {
                // printf("send %d to right pipe\n",atoi(buf));
                // printf("\n");
                write(right_pipe[1],buf,2);
            }
        }
        wait();
        printf("now close the right pipe\n");
        close(right_pipe[1]);
        close(right_pipe[0]);
        exit();
    }
}
int main(int argc,char *argv[])
{
    //int count = 40;
    int i,fpid;
    int numbers[2];
    pipe(numbers);
    fpid = fork();
    if(fpid < 0)
    {
        printf("faild to create child process.\n");
        exit();
    }
    else if(fpid == 0)
    {
        child_process(numbers);
    }
    char a[2];
    for(i=2;i<=count;i++)
    {
        a[0] = i / 10 + 48;
        a[1] = i % 10 + 48;
        write(numbers[1],a,2);
        //printf("%s\n",a);
    }
    wait();
    printf("main:close the right pipe\n");
    close(numbers[1]);
    exit();
}