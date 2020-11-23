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
// void PipeCmd(int argc,char* argv[])
// {
// 	int i;
// 	for(i=0;i<argc;i++)
// 	{
// 		// if(argv[i] == '|')
// 		if(strcmp(argv[i],"|"))
// 		{
// 			argv[i] = '\0';
// 			break;
// 		}
// 	}
// 	int pd[2];
// 	pipe(pd);
// 	if(fork() == 0)
// 	{
// 		close(1);
// 		dup(pd[1]);
// 		close(pd[0]);
// 		close(pd[1]);
// 		RunCmd(i,argv);
// 	}
// 	else
// 	{
// 		close(0);
// 		dup(pd[0]);
// 		close(pd[0]);
// 		close(pd[1]);
// 		RunCmd(argc-i-1,argv+i+1);
// 	}
// }
void RunCmd(int argc,char* argv[])
{
	
	int i,j;
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
			// PipeCmd(argc,argv);
			for(j=0;j<argc;j++)
			{
				if(!strcmp(argv[j],"|"))
				{
					argv[j] = '\0';
					break;
				}
			}
			int pd[2];
			pipe(pd);
			if(fork() == 0)
			{
				close(1);
				dup(pd[1]);
				close(pd[0]);
				close(pd[1]);
				RunCmd(j,argv);
			}
			else
			{
				close(0);
				dup(pd[0]);
				close(pd[0]);
				close(pd[1]);
				RunCmd(argc-j-1,argv+j+1);
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
				// Mark("open file");
				argv[i] = 0;
				// Mark("recur");
				RunCmd(i,argv);
			}
			else
			{
				fprintf(2,"%s\n","Redirect error!");
			}
		}
		// if(argv[i] == '<')
		if(!strcmp(argv[i],"<"))
		{
			// Mark("find <");
			if(i != argc)
			{
				close(0);
				open(argv[i+1],O_RDONLY);
				argv[i] = 0;
				RunCmd(i,argv);
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
	exec(argv[0],argv);
}

int main()
{
	static char buf[MAX_CMD_LEN];
	
	// int i;
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
		if(fork() == 0)
		{
			char* argv[MAXARGS];
			int argc = -1;
			// Mark("mark 1");
			ScanCmd(buf,argv,&argc);
			RunCmd(argc,argv);
		}
		else
		{
			wait(0);
		}
	}
	exit(0);
}