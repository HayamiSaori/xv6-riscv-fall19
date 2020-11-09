#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#define MAX_CMD_NUM 50
#define MAX_ARG_NUM 10
#define MAX_CMD_LEN 512
int main(int argc, char *argv[])
{
	char *cmd[MAX_CMD_NUM];
	char cmd_buf[MAX_CMD_NUM][MAX_ARG_NUM];
	char shell_buf[MAX_CMD_LEN];
	char *ch;
	int i,fd,cmd_index,arg_index;;
	for(i = 0; i < argc-1; i++)
  	{
		cmd[i] = argv[i+1];
  	}
	while((fd = read(0, shell_buf, MAX_CMD_LEN)) > 0)
	{
		cmd_index = 0;
		arg_index = 0;
		for(ch=shell_buf;*ch!=0;ch++)
		{
			if(*ch == ' ' || *ch == '\n')
			{
				cmd_buf[cmd_index][arg_index] = '\0';
				cmd_index++;
				arg_index = 0;
			}
			else
			{
				cmd_buf[cmd_index][arg_index] = *ch;
				arg_index++;
			}
		}
		cmd_buf[cmd_index][arg_index] = '\0';
		cmd_index++;
		for(i=0;i<cmd_index;i++)
		{
			cmd[i+argc-1] = cmd_buf[i];
		}
		if(fork() == 0)
		{
			exec(cmd[0], cmd);
		}
		else
		{
			wait();
		}
	}
	exit();
}