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
	for(i=0;cmd[i]!='\n' && cmd[i]!='\0' && j<MAXARGS;i++)
	{
		while(isBlank(cmd[i]))
		{
			i++;
		}
		argv[j] = cmd + i;
		j++;
		while(!isBlank(cmd[i]) && cmd[i]!='\0' && cmd[i]!='\n')
		{
			i++;
		}
		cmd[i] = '\0';	
	}
	argv[j] = '\0';
	*argc = j;
}

void RunCmd(int argc,char* argv[])
{
	
	int i;
	// printf("argc is %d,command is:",argc);
	// for(i=0;i<argc;i++)
	// {
	// 	printf("%s ",argv[i]);
	// }printf("\n");
	for(i=0;i<argc;i++)
	{
		// if(argv[i] == '|')
		if(!strcmp(argv[i],"|"))
		{
			argv[i][0] = 0;

			int pd[2];
			pipe(pd);
			if(fork() == 0)
			{
				close(1);
				dup(pd[1]);
				close(pd[0]);
				close(pd[1]);
				RunCmd(i,argv);
				return;
			}
			else
			{
				close(0);
				dup(pd[0]);
				close(pd[0]);
				close(pd[1]);
				RunCmd(argc-i-1,argv+i+1);
				return;
			}
		}
	}
	for(i=0;i<argc;i++)
	{
		// if(argv[i] == '>')
		if(!strcmp(argv[i],">"))
		{
			// Mark("find >");
			if(i != argc)
			{
				// Mark("redir");
				close(1);
				open(argv[i+1],O_CREATE|O_WRONLY);
				// fprintf(2,"%s\n","ERROR!");
				// Mark("open file");
				argv[i] = 0;
			}
			else
			{
				fprintf(2,"%s\n","Redirect error!");
			}
		}
		// if(argv[i] == '<')
		if(!strcmp(argv[i],"<"))
		{
			// // Mark("find <");
			if(i != argc)
			{
				close(0);
				open(argv[i+1],O_RDONLY);
				argv[i] = 0;
			}
			else
			{
				fprintf(2,"%s\n","Redirect error!");
			}
		}
	}
	// Mark("the cmd");
	// for(i=0;i<argc;i++)
	// {
	// 	printf("%s,",argv[i]);
	// }
	argv[argc] = 0;
	exec(argv[0],argv);
}

int main()
{
	static char buf[100];
	int fd;
    while((fd = open("console", O_RDWR)) >= 0){
        if(fd >= 3){
            close(fd);
            break;
        }
    }
	while(getcmd(buf,sizeof(buf)) >= 0)
	{
		if(buf[0] == 'c' && buf[1] == 'd' && buf[2] == ' ')
		{
      		// Chdir must be called by the parent, not the child.
      		buf[strlen(buf)-1] = 0;  // chop \n
      		if(chdir(buf+3) < 0)
        		fprintf(2, "cannot cd %s\n", buf+3);
      		continue;
    	}
		// 输入"quit "时退出nsh
		if(buf[0] == 'q' && buf[1] == 'u' && buf[2] == 'i' && buf[3] == 't' && buf[4] == ' ')
		{
			exit(0);
		}
		if(fork() == 0)
		{
			char* argv[MAXARGS];
			int argc = -1;
			// Mark("mark 1");
			ScanCmd(buf,argv,&argc);
			RunCmd(argc,argv);
		}
		wait(0);
		memset(buf,0,100);
	}
	exit(0);
}