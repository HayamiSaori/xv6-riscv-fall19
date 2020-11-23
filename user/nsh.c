// Shell.

#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

// Parsed command representation
#define EXEC  1
#define REDIR 2
#define PIPE  3
#define LIST  4
#define BACK  5

#define MAXARGS 10
#define MAX_CMD_LEN 512
#define MAX_ARG_LEN 64
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
void Mark(char *s)
{
	printf("------");printf("%s",s);printf("------\n");
}
int isBlank(char c)
{
	if(c==' ' || c=='\t')
		return 1;
	else
		return 0;
}
void ScanCmd(char *cmd,char* argv[],int* argc)
{
	int i,j;
	i = 0;
	j = 0;
	for(i=0;cmd[i]!='\n' && cmd[i]!='\0';i++)
	{
		while(isBlank(cmd[i]))
		{
			i++;
		}
		argv[j] = cmd + i;
		j++;
		while(!isBlank(cmd[i]) && cmd[i]!='\0')
		{
			i++;
		}
		cmd[i] = '\0';	
	}
	argv[j] = '\0';
	*argc = j;
}

int main()
{
	static char buf[MAX_CMD_LEN];
	
	int i;
	while(getcmd(buf,sizeof(buf)) >= 0)
	{
		if(fork() == 0)
		{
			char* argv[MAXARGS];
			int argc = -1;
			Mark("mark 1");
			ScanCmd(buf,argv,&argc);
			for(i=0;i<argc;i++)
			{
				printf("%s,",argv[i]);
				// printf("%c",buf[i]);
			}
		}
		else
		{
			wait(0);
		}
		
	}
	exit(0);
}