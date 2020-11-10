#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
struct message {//msgbuffer
    long msgtype; 	//msgtype
    int beautyspa[4]; //car_readings
} service; 
  
void send() 
{ 
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
    printf("Welcome to the Beauty Spa service\n");
    for(i=0;i<4;i++)
	{
  
		printf("Enter the type of Spa %d (enter 0 for Hair Spa or 1 for Facial ): ",(i+1));
 		scanf("%d",&service.beautyspa[i]);
	}
    ret = msgsnd(msgid, &service, sizeof(service), 0); 
    if(ret < 0) {
	    perror("message queue not send");
	    exit(2);
    }
  
  
    return ; 
} 
int val=0;

void* filled_slot(void *arg)
{
	int i,sum1=0;
	printf("thread providing the service to the cars \n");
	
  while(val<=3)
  {
   /*Wait for read thread to print*/
    
  sem_wait((sem_t *) arg);
    val++;
}
}
  void* car_service(void *arg)
{
    while(val<=3)
  {
if(service.beautyspa[val]==1)
{
    printf("customer %d Facial is done\n",(val+1));
}
else
 printf("customer %d Hair Spa is done\n",(val+1));
    sem_post((sem_t *) arg);
       sleep(1);  
  }  
 
}
void recv()
{
    key_t key;
    int msgid;
     int ret;
     int i; 

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT); 
    msgrcv(msgid, &service, sizeof(service), 1, 0);
        for(i = 0; i< 4; i++)
        {
 printf("customer %d type of service is: %d--->",(i+1), service.beautyspa[i]);
 if( service.beautyspa[i]==1)
 printf("(Hair Spa)\n");
 else
 printf("(Facial)\n");

        }
 return ;
}
int main()
{
	send();
	recv();
	sem_t rs;
	void* res;
	pthread_t pt2,pt1;
	sem_init(&rs,0,0);
	pthread_create(&pt1,NULL,filled_slot,&rs);
	pthread_create(&pt2,NULL,car_service,&rs);
	pthread_join(pt1,&res);
	pthread_join(pt2,&res);
	sem_destroy(&rs);
}



