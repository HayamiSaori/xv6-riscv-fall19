#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"

int main(int argc,char *argv[]){
    if(argc != 2)
    {
        printf("Error!The syntax must be \"sleep [seconds]\"!\n");
        exit();
    }
    int time = atoi(argv[1]);
    printf("I will sleep %d seconds\n",time);
    sleep(time * 10);
    exit();
}