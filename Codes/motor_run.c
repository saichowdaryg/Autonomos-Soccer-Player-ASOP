/*
ASOP project
Course: Embedded Systems EE404, Semester-I 2015
IIT Gandhinagar
Instructor:	Prof. Joycee M. Mekie
TA:		Sneha Ved
Team:		Sai Chowdary (sai_gullapally@iitgn.ac.in)
		Veerabadra Lokesh (veerabadralokesh_paturu@iitgn.ac.in)
		Sidhartha Reddy (sidhartha_reddy@iitgn.ac.in)
*/

// Program for controlling the Motors

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include"superio.h"
#include <termios.h> 


int n=0;
char readBuff[4];
int myint = 1;



char rec(int);

void main() {
	unsigned char ch;
	unsigned int k=0;
	
	// assigning the Hardware pins on the Intel Atom
	unsigned int motorEnableWrite1 = 1;	// enable pin for the 4 motors attached to the wheels
	unsigned int motorEnableWrite2 = 6;	// enable pin for 2 motors used for controlling hands
	// Control pins for motors, must be connected by checking the motor rotation on using this program
	// dirctionWrite1 and directionWrite2 must be connected for the motors on the same side
	unsigned int directionWrite1 = 2;
	unsigned int directionWrite2 = 3;
	// dirctionWrite21 and directionWrite22 must be connected for the motors on the opposite side of the previously connected motors
	unsigned int directionWrite21 = 4;
	unsigned int directionWrite22 = 5;
	// Control pins for controlling the arms
	unsigned int arms1 = 7;
	unsigned int arms2 = 8;
	
	// write_gpio(pin,0,1) makes the pin high
	// write_gpio(pin,0,0) makes the pin low
	
	// Initializing the pins
	write_gpio(motorEnableWrite1, 0,1);
	write_gpio(motorEnableWrite2, 0,1);
	write_gpio(motorEnableWrite1, 0,0);
	write_gpio(motorEnableWrite2, 0,0);
        write_gpio(directionWrite1, 0,0);
	write_gpio(directionWrite2, 0,0);
	write_gpio(directionWrite21, 0,0);
	write_gpio(directionWrite22, 0,0); 
	
	int i,j;

	char key;

	int listenfd = 0, connfd = 0,n;
	char con =1;
    	struct sockaddr_in serv_addr;
    	unsigned char sendBuff[120*90];
    	
	// --------------------- Create listening Socket ------------------------------

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1)
	{
		printf("Could not create socket\n");
	}

	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));
	memset(readBuff, '0', sizeof(readBuff));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(9999);
	if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
	{printf("Bind failed\n");return(0);}

	listen(listenfd, 10);
	// printf("Waiting for incoming connection.... \n");

        
        while(myint==1){
        // ------------------------------- Waiting for Connection ---------------------------
		while(con && k==0)
		{
			connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
			if(connfd<0)
			{
				printf("accept failed\n");
			}
			else {
				printf("Connection Accepted\n");
				//con=0;
				k=1;
			}
		
		}
        
		while(k==1){
			ch=rec(connfd);
			//printf(" %c\n",ch);
			switch(ch){
				case 'a'://left turn
					write_gpio(motorEnableWrite1, 0,1);
					write_gpio(motorEnableWrite2, 0,0);
					write_gpio(directionWrite1, 0,0);
					write_gpio(directionWrite2, 0,1);
					write_gpio(directionWrite21, 0,1);
					write_gpio(directionWrite22, 0,0);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,0);
					break;
				case 's'://backward movt.
					write_gpio(motorEnableWrite1, 0,1);
					write_gpio(motorEnableWrite2, 0,0);
					write_gpio(directionWrite1, 0,1);
					write_gpio(directionWrite2, 0,0);
					write_gpio(directionWrite21, 0,1);
					write_gpio(directionWrite22, 0,0);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,0);
					break;
				case 'w'://straight movt.
					write_gpio(motorEnableWrite1, 0,1);
					write_gpio(motorEnableWrite2, 0,0);
					write_gpio(directionWrite1, 0,0);
					write_gpio(directionWrite2, 0,1);
					write_gpio(directionWrite21, 0,0);
					write_gpio(directionWrite22, 0,1);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,0);
					break;
				case 'd'://right turn
					write_gpio(motorEnableWrite1, 0,1);
					write_gpio(motorEnableWrite2, 0,0);
					write_gpio(directionWrite1, 0,1);
					write_gpio(directionWrite2, 0,0);
					write_gpio(directionWrite21, 0,0);
					write_gpio(directionWrite22, 0,1);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,0);
					break;
				case 'p'://close current socket connection and wait for a new one
					write_gpio(motorEnableWrite1, 0,0);
					write_gpio(motorEnableWrite2, 0,0);
					write_gpio(directionWrite1, 0,0);
					write_gpio(directionWrite2, 0,0);
					write_gpio(directionWrite21, 0,0);
					write_gpio(directionWrite22, 0,0);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,0);
					k = 0;
					//close(connfd);
					//exit(1);
					break;
				case '0'://on recieving zero close the socket and exit the program
					write_gpio(motorEnableWrite1, 0,0);
					write_gpio(motorEnableWrite2, 0,0);
					write_gpio(directionWrite1, 0,0);
					write_gpio(directionWrite2, 0,0);
					write_gpio(directionWrite21, 0,0);
					write_gpio(directionWrite22, 0,0);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,0);
					myint = 0;
					k = 0;
					//close(connfd);
					//exit(1);
					break;
				case 'o'://open the arms
					write_gpio(motorEnableWrite1,0,0);
					write_gpio(motorEnableWrite2,0,1);
					write_gpio(directionWrite1, 0,0);
					write_gpio(directionWrite2, 0,0);
					write_gpio(directionWrite21, 0,0);
					write_gpio(directionWrite22, 0,0);
					write_gpio(arms1,0,0);
					write_gpio(arms2,0,1);
					break;
				case 'c'://close arms
					write_gpio(motorEnableWrite1,0,0);
					write_gpio(motorEnableWrite2,0,1);
					write_gpio(directionWrite1, 0,0);
					write_gpio(directionWrite2, 0,0);
					write_gpio(directionWrite21, 0,0);
					write_gpio(directionWrite22, 0,0);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,1);
					break;
				default :
					write_gpio(motorEnableWrite1, 0,0);
					write_gpio(motorEnableWrite2, 0,0);
					write_gpio(directionWrite1, 0,0);
					write_gpio(directionWrite2, 0,0);
					write_gpio(directionWrite21, 0,0);
					write_gpio(directionWrite22, 0,0);
					write_gpio(arms2,0,0);
					write_gpio(arms1,0,0);
				}
			//usleep(1000);
			ch='q';
			}
			close(connfd);
		}
}

char rec(int connfd) 
{
  while(1)
    {
    	n = recv(connfd,readBuff,1,0);
    	if (n < 0) printf("ERROR reading from socket");
	printf("%c\n",readBuff[0]);
	if(n>0){
		return readBuff[0];
	}
	}
}

