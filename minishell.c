#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

int main()
{
 while(1){
   printf("[wg@minihost ~]$");
   fflush(stdout);
   
   char tmp[1024] = {0};
   scanf("%[^\n]%*c",tmp);
   printf("[%s]\n",tmp);

   //需要将整体字符串解析出：程序名称+参数
   char* ptr=tmp;
   int redirect_flag=0;
   char* redirect_file=NULL;
   while(*ptr!='\0'){
     if(*ptr=='>'){
       redirect_flag=1;
       *ptr='\0';
       ptr++;
       if(*ptr=='>'){
         redirect_flag=2;
         *ptr='\0';
         ptr++;
       }
     while(isspace(*ptr)&& *ptr!='\0')
       ptr++;
     redirect_file=ptr;
     while(!isspace(*ptr)&& *ptr!='\0')
       ptr++;
     *ptr='\0';
     ptr++;
     continue;
     }
     ptr++;
   } 

   ptr=tmp;
   char* argv[32]={NULL};
   int argc=0;

   while(*ptr!='\0'){
     if(!isspace(*ptr)){
        argv[argc]=ptr;
        argc++;
        while(!isspace(*ptr)&& (*ptr)!='\0'){
          ptr++;
        }
          *ptr='\0';
         // ptr++;
         // continue;
         }
        ptr++;
   }
   argv[argc]=NULL;

   if(!strcmp(argv[0],"cd")){
      //改变当前工作路径
      chdir(argv[1]);
     
   } 
   
   
   int pid=fork();
   if(pid==0){
     int fd=1;
     if(redirect_flag==1){
       fd=open(redirect_file,O_CREAT|O_WRONLY|O_TRUNC,0664);
     }else if(redirect_flag==2){
        fd=open(redirect_file,O_CREAT|O_WRONLY|O_APPEND,0664);
     }
     dup2(fd,1);

     execvp(argv[0],argv);
     //若子进程程序替换失败，则直接退出，因为终端不需要多个shell
     exit(0);
   }
   //等待子进程退出，避免僵尸进程
   wait(NULL);
     

    } 


  return 0;
}
