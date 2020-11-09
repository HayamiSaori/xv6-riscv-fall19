#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if(strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p) + 1);
    // memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
    return buf;
}

void 
search(char *path,char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;
  
  if((fd = open(path, 0)) < 0){
    // printf("----------\n");
    fprintf(2, "open error: cannot open %s\n", path);
    return;
  }
  if(fstat(fd, &st) < 0){
    fprintf(2, "stat error: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf("get path here!\n");
    //printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    // printf("path:%s,name:%s\n",path,fmtname(path));
    if(strcmp(filename,fmtname(path)) == 0){
        // printf("%s/%s\n",path,filename);
        printf("%s\n",path);
    }
    break;

  case T_DIR:
    // printf("----------\n");
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
            continue;
        if(strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0){
          memmove(p, de.name, DIRSIZ);
          p[DIRSIZ] = 0;
          search(buf,filename);
        }
        // memmove(p, de.name, DIRSIZ);
        // p[DIRSIZ] = 0;
        // if(stat(buf, &st) < 0){
        //     printf("ls: cannot stat %s\n", buf);
        //     continue;
        // }
        // printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);

        // search(buf,filename);
    }
    break;
  }
  close(fd);
}
int main(int argc,char *argv[])
{
    if(argc < 3)
    {
        printf("Syntax error!\n");
        exit();
    }
    // ls(".");
    search(argv[1],argv[2]);
    exit();
}