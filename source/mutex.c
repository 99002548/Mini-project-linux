#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/msg.h>
pthread_t t1,t2; 
int counter; 
pthread_mutex_t mu; 


struct message {//msgbuffer
    long msgtype; 	
    int beautyspa[4];
} service; 



void receive()
{
   key_t key;
    int msgid;
     int ret;
     //int i; 

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgrcv(msgid, &service, sizeof(service), 1, 0);
        for(i = 0; i< 4; i++)
 printf("customer %d type of service is: %d \n",(i+1), service.beautyspa[i]);
 return ;


}

void send(){
key_t key; 
    int msgid; 
    int ret,i;
    key = ftok("progfile", 65); 
    msgid = msgget(key, 0666 | IPC_CREAT);
    if(msgid < 0) {
	perror("Error in creating msgid");
	exit(1);
    }
    service.msgtype = 1; 
    printf("Welcome to the Beauty SPA center\n");
for(i=0;i<4;i++)
{
  
printf("Enter the type of service %d (enter 0 for Hair Spa or 1 for Facial): ",(i+1));
 scanf("%d",&service.beautyspa[i]);
}
    ret = msgsnd(msgid, &service, sizeof(service), 0); 
    if(ret < 0) {
	    perror("message queue not send");
	    exit(2);
    }
  
  
    return ; 
}  
void* HairSpa(void* arg) 
{ 
    pthread_mutex_lock(&mu); 
  
	int  i = 0; 
	int HairSpa=0;//carwash

    printf("Thread1 started\n"); 
    for(i = 0; i <= 3; i++)
{
if(service.beautyspa[i]==0) 
HairSpa=HairSpa+1;
}
	
   printf("Number of customers for Hair SPA are:%d\n",HairSpa);
    printf("Thread1 finished\n"); 
  
    pthread_mutex_unlock(&mu); 
  
    return NULL; 
} 
void* Facial(void* arg) 
{ 
pthread_mutex_lock(&mu); 
  
	int  i = 0; 
	int facial=0;
	
    printf("Thread2 started\n"); 
    for(i = 0; i <= 3; i++)
{
if(service.beautyspa[i]==1) 
facial=facial+1;
}
	
   printf("Number of customers for Facial are:%d\n",facial);
    printf("Thread2 finished\n"); 
  
    pthread_mutex_unlock(&mu); 
  
    return NULL; 
    
} 
  
int main(void) 
{ 
    send();
    receive();
    int i = 0; 
    int error; 
  
    if (pthread_mutex_init(&mu, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 
    else{
	printf("You are in mutex\n");
    }
  
   
    error = pthread_create(&t1,NULL,&HairSpa, NULL); 
    if (error != 0) 
           printf("\nThread can't be created :[%s]", strerror(error)); 
	
    error = pthread_create(&t2,NULL,&Facial, NULL); 
    if (error != 0) 
           printf("\nThread can't be created :[%s]", strerror(error));   
    pthread_join(t1, NULL); 
    pthread_join(t2, NULL); 
    pthread_mutex_destroy(&mu); 
    
    
  
    return 0; 
} 
