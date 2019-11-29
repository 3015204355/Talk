#define _GNU_SOURCE 1
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<poll.h>
#include<mysql/mysql.h>
#define USER_LIMIT 5
#define BUFFER_SIZE 64
#define FD_LIMIT 65535
MYSQL con;
MYSQL_RES *res_ptr;
MYSQL_ROW sqlrow;
struct client_data
{
struct sockaddr_in address;
char * write_buf;
char buf[ BUFFER_SIZE];

};

int setnonblocking(int fd)
{
int old_option=fcntl(fd,F_GETFL);
int new_option=old_option | O_NONBLOCK;
fcntl(fd,F_SETFL,new_option);
return old_option;

}

int main(int argc,char * argv[])
{
if(argc<=2)
{
printf("%s  ip port\n",basename(argv[0]));
return 1;

}
const char* ip=argv[1];
int port =atoi(argv[2]);

int ret=0;
struct sockaddr_in address;
bzero(&address,sizeof(address));
address.sin_family=AF_INET;
inet_pton(AF_INET,ip,&address.sin_addr);
address.sin_port=htons(port);

int listenfd=socket(PF_INET,SOCK_STREAM,0);
assert(listenfd>=0);

ret=bind(listenfd,(struct sockaddr*)&address,sizeof(address));
assert(ret!=-1);
ret=listen(listenfd,5);
assert(ret!=-1);

struct client_data* users=(struct client_data*)malloc(FD_LIMIT*sizeof(struct client_data));
struct pollfd fds[USER_LIMIT+1];
int user_counter=0;
  int j=0;
  for(;j<=USER_LIMIT;++j)
  {
  fds[j].fd=-1;
  fds[j].events=0;
  }
fds[0].fd=listenfd;
fds[0].events=POLLIN | POLLERR;
fds[0].revents=0;

 while(1)
{
   ret=poll(fds,user_counter+1,-1);
   if(ret<0)
   {
    printf("poll failure\n");
    break;
   }
   int i=0;
   for(;i<user_counter+1;++i)
   {
    if((fds[i].fd==listenfd)&&(fds[i].revents&POLLIN))
   {
   struct sockaddr_in client_address;
   socklen_t client_addrlength=sizeof(client_address);
   int connfd=accept(listenfd,(struct sockaddr*)&client_address,&client_addrlength);

   if(connfd<0)
   {
    printf("errno is: %d\n",errno);
    continue;
   }
   if(user_counter>=USER_LIMIT)
   {
    const char* info="too mang users\n";
    printf("%s",info);
    send(connfd,info,strlen(info),0);
    close(connfd);
     continue;
   }
    user_counter++;
    users[connfd].address=client_address;
    setnonblocking(connfd);
    fds[user_counter].fd=connfd;
    fds[user_counter].events=POLLIN | POLLRDHUP | POLLERR;
    printf("comes a new user,now have %d users\n",user_counter);
}
else if(fds[i].revents&POLLERR)
{
printf("get an error from %d\n",fds[i].fd);
char errors[100];
memset(errors,'\0',100);
socklen_t length=sizeof(errors);
if(getsockopt(fds[i].fd,SOL_SOCKET,SO_ERROR,&errors,&length)<0)
{
printf("get socket option failed\n");
}
continue;
}
else if(fds[i].revents & POLLRDHUP)
{
users[fds[i].fd]=users[fds[user_counter].fd];
close(fds[i].fd);
fds[i]=fds[user_counter];
i--;
user_counter--;
printf("a client left\n");

}
else if(fds[i].revents & POLLIN)
{
   int connfd=fds[i].fd;
   memset(users[connfd].buf,'\0',BUFFER_SIZE);
   ret=recv(connfd,users[connfd].buf,BUFFER_SIZE-1,0);
   printf("User:%d\tString:%s \n",connfd,users[connfd].buf);
//Start Login
   if(strstr(users[connfd].buf,"LOGIN")!=NULL)
{
   char username[20];char password[20];
   memset(username,'\0',20);
   memset(password,'\0',20);
   int count=0;
   char *remind=users[connfd].buf;
   int k;
   int u=0;int p=0;
   for(k=0;*(remind+k)!='\0';k++)
   {
    if(*(remind+k)==' ')
         { count++;
         continue;
         }
    if(count==0)
       continue;
    else if(count==2)
        password[p++]=*(remind+k);
    else
     {
        username[u++]=*(remind+k);      
     }
   }
  // printf("username:%s\tpassword:%s\n",username,password);
   char str[100];
   memset(str,'\0',100);
   char *query="select password from user where username = \"";
   strcat(str,query);
   strcat(str,username);//query statement
   strcat(str,"\"");
  // printf("query:  %s\n",str);//print query statement
  //connect to mysql 
     mysql_init(&con);
     char pass[20];
     memset(pass,'\0',20);
     if(mysql_real_connect(&con,"localhost","root","Mysql?123","Talk",0,NULL,0))
    {
      int que=mysql_query(&con,str); 
      if(que)
         {  
           printf("select failed!\n");
         } 
      else
      {
        res_ptr=mysql_use_result(&con);
        if(res_ptr)
        {
         sqlrow=mysql_fetch_row(res_ptr);
//         printf("%s\n",sqlrow[0]); 
         int l=strlen(sqlrow[0]);
         strcat(pass,sqlrow[0]);
        }
      }
    }
       mysql_free_result(res_ptr);
       mysql_close(&con);
       printf("Mysql Closed!\n");
      // printf("pass:%s!\n",pass);
      // printf("password:%s!\n",password);
      // printf("strcmp:%d\n",strcmp(pass,password));
       if(strcmp(pass,password)==0)
       {
       char s[]="Success!";
       ret =send(connfd,s,strlen(s),0);
       }
       else
       {
       char s[]="Failed!";
       ret=send(connfd,s,strlen(s),0);
       } 
   } 
// End Login;Start List
   if(strstr(users[connfd].buf,"LIST")!=NULL)
{
   char listuser[20];
   memset(listuser,'\0',20);
   char *remind=users[connfd].buf;
   int l=0;
   int flag=0;
   int count=0;
   for(;*(remind+l)!='\0';l++)
   {
    if(flag==1)
    {
      listuser[count++]=*(remind+l);
    }
    if(*(remind+l)==' ')
       flag=1;  
   }
  // printf("ListUser:%s\n",listuser);
     
     char str[200];
     memset(str,'\0',200);
     char* query="select username from user where uid in (select fid from uf where uid=(select uid from user where username =\"";
     strcat(str,query);
     strcat(str,listuser);
     strcat(str,"\"))");
     printf("List:%s\n",str);
     mysql_init(&con);
     if(mysql_real_connect(&con,"localhost","root","Mysql?123","Talk",0,NULL,0))
    {
      int que=mysql_query(&con,str); 
      if(que)
         {  
           printf("select failed!\n");
         }
      else{ 
        res_ptr=mysql_use_result(&con);
        char list[200];
        memset(list,'\0',200);
        if(res_ptr)
        {
         int s=0;
         while( sqlrow=mysql_fetch_row(res_ptr))
         {  
           if(s!=0)
              strcat(list,";");
           char* fid=sqlrow[0];
           printf("fid:%s\n",fid);
           strcat(list,fid);
          // strcat(list,";");
           s++;
         }
        }
       int ret=send(connfd,list,strlen(list),0);
       if(ret<0)
         {
         printf("Error!\n");
           }
       }
    } 

}
//End List
   if(ret <0)
   {
      if(errno!=EAGAIN)
      {
         close(connfd);
         users[fds[i].fd]=users[fds[user_counter].fd];
         fds[i]=fds[user_counter];
         i--;
          user_counter--;
      }
   }
   else if(ret==0)
   {}
   else
   {
    int j=1;
    for(;j<=user_counter;++j)
    {
      if(fds[j].fd==connfd)
      {continue;
       }
     fds[j].events |= ~POLLIN;
     fds[j].events |=POLLOUT;
     users[fds[j].fd].write_buf=users[connfd].buf;
     }
   }
}
else if(fds[i].revents & POLLOUT)
{
  int connfd =fds[i].fd;
  if(!users[connfd].write_buf)
  {
    continue;
  }
   
   ret =send(connfd,users[connfd].write_buf,strlen(users[connfd].write_buf),0);
   users[connfd].write_buf=NULL;
   fds[i].events |= ~POLLOUT;
   fds[i].events |= POLLIN;

}

}

}

free(users);
close(listenfd);
return 0;

}
