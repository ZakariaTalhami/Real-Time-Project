#ifndef MESSAGING
#define MESSAGING _MESSAGING_H_

#include "queue.h"

#ifndef IC_KEY
#define IC_KEY 1002
#endif

#ifndef CF_KEY
#define CF_KEY 500
#endif

#ifndef FB_KEY
#define FB_KEY 800
#endif


/*
	Defining the mTypes
*/

#ifndef M_CI
#define M_CI 2
#endif

#ifndef M_CF
#define M_CF 3
#endif


#ifndef M_FB
#define M_FB 96
#endif



/*
		INITIATOR - CUSTOMER channel
*/
struct IC_MSG
{
	long mtype;
	int pid;
	int status;
	int amount;
};

// struct CF_MSG
// {
// 	long mtype;
// 	int pid;
// 	int status;
// 	int amount;
// };

struct CF_MSG
{
	long mtype;
	int pid;
	int status;
	int amount[MAX_TYPES];
	int received[MAX_TYPES];
};



void OPEN_IC_QID();
int send_IC_MSG(struct IC_MSG *  , int , int);
void print_IC_MSG(struct IC_MSG );
int get_IC_MSG(struct IC_MSG * );


void OPEN_CF_QID();
int send_CF_MSG(struct CF_MSG * , int  , int[] );
int get_CF_MSG(struct CF_MSG * );
int peek_CF(long );
void print_CF_MSG(struct CF_MSG );
void print_CF_MSG(struct CF_MSG );
void free_CF();
int send_CF_RLY(struct IC_MSG * , int  , int , long );
int get_CF_RLY(struct IC_MSG * , long );

void OPEN_FB_QID();
int forward_CF_msg(struct CF_MSG * , int );
int send_FB_RLY(struct CF_MSG * , int , int []);
int get_FB_MSG(struct CF_MSG * );
int get_FB_RLY(struct CF_MSG * , long );
int peek_FB(long mtype);

int IC_QID=-1;
int CF_QID=-1;
int FB_QID=-1;



void OPEN_IC_QID(){
	if ((IC_QID = open_queue(IC_KEY)) == -1)
	{
		perror("OPEN_IC_QID");
		exit(errno);
	}
}

int send_IC_MSG(struct IC_MSG * msg, int status , int amount){
	int res , len;
	(*(msg)).mtype = M_CI;
	(*(msg)).pid = getpid();
	(*(msg)).status = status;
	(*(msg)).amount = amount;
	print_IC_MSG(*(msg));
	len = sizeof(struct IC_MSG) - sizeof(long);
	if ((res = msgsnd(IC_QID , msg , len , 0)) == -1)
	{
		printf("Error: unable to send a message\n");
		return res;
	}
	return res;
}

int get_IC_MSG(struct IC_MSG * msg){
	int res , len;
	len = sizeof(struct IC_MSG) - sizeof(long);
	if ((res = msgrcv(IC_QID , msg , len , M_CI , 0)) == -1)
	{
		printf("Error: unable to read a message\n");
		return res;
	}
	return res;
}

int peek(long mtype){
	return peek_message(IC_QID , mtype);
}

void print_IC_MSG(struct IC_MSG msg){
	printf("IC_MSG has the Following Parameters\n");
	printf("mtype => %ld\n" , msg.mtype);
	printf("pid => %d\n" , msg.pid);
	printf("status => %d\n" , msg.status);
	printf("Amount => %d\n" , msg.amount);
}

void free_IC(){
	remove_queue(IC_QID);
}



/*
	
	Customer and Front desk messaging

*/

void OPEN_CF_QID(){
	if ((CF_QID = open_queue(CF_KEY)) == -1)
	{
		perror("OPEN_CF_QID");
		exit(errno);
	}
}

// int send_CF_MSG(struct CF_MSG * msg, int status , int amount[]){
// 	int res , len;
// 	(*(msg)).mtype = M_CF;
// 	(*(msg)).pid = getpid();
// 	(*(msg)).status = status;
// 	// for (int i = 0; i < MAX_TYPES; ++i)
// 	// {
// 	// 	(*(msg)).amount[i] = amount[i];
// 	// }
// 	(*(msg)).amount = 5;
// 	print_CF_MSG(*(msg));
// 	printf("Send message\n");
// 	len = sizeof(struct CF_MSG) - sizeof(long);
// 	if ((res = msgsnd(CF_QID , msg , len , 0)) == -1)
// 	{
// 		printf("Error: unable to send a message\n");
// 		return res;
// 	}
// 	return res;
// }
int send_CF_MSG(struct CF_MSG * msg, int status , int amount[]){
	int res , len;
	(*(msg)).mtype = M_CF;
	(*(msg)).pid = getpid();
	(*(msg)).status = status;
	for (int i = 0; i < MAX_TYPES; ++i)
	{
		(*(msg)).amount[i] = amount[i];
	}
	for (int i = 0; i < MAX_TYPES; ++i)
	{
		(*(msg)).received[i] = 0;
	}

	// print_CF_MSG(*(msg));
	printf("Send message\n");
	len = sizeof(struct CF_MSG) - sizeof(long);
	if ((res = msgsnd(CF_QID , msg , len , 0)) == -1)
	{
		printf("Error: unable to send a message\n");
		return res;
	}
	return res;
}
/*
	Send Front desk reply to customer
*/
int send_CF_RLY(struct IC_MSG * msg, int status , int amount , long mtype){
	int res , len;
	(*(msg)).mtype = mtype;
	(*(msg)).pid = getpid();
	(*(msg)).status = status;
	(*(msg)).amount = amount;
	

	// `_CF_MSG(*(msg));
	printf("***************Send Reply %ld\n" , (*(msg)).mtype);
	len = sizeof(struct IC_MSG) - sizeof(long);
	if ((res = msgsnd(CF_QID , msg , len , 0)) == -1)
	{
		printf("Error: unable to send a message\n");
		return res;
	}
	return res;
}

int get_CF_MSG(struct CF_MSG * msg){
	int res , len;
	len = sizeof(struct CF_MSG) - sizeof(long);
	if ((res = msgrcv(CF_QID , msg , len , M_CF , 0)) == -1)
	{
		printf("Error: unable to read a message\n");
		// return res;
		perror("get_CF_MSG");
		exit(errno);
	}
	return res;
}

int get_CF_RLY(struct IC_MSG * msg, long mytype){
	int res , len;
	len = sizeof(struct IC_MSG) - sizeof(long);
	if ((res = msgrcv(CF_QID , msg , len , mytype , 0)) == -1)
	{
		printf("Error: unable to read a message\n");
		return res;
	}
	return res;
}

int peek_CF(long mtype){
	return peek_message(CF_QID , mtype);
}

// void print_CF_MSG(struct CF_MSG msg){
// 	printf("mtype => %ld\n" , msg.mtype);
// 	printf("pid => %d\n" , msg.pid);
// 	printf("status => %d\n" , msg.status);
// 	printf("amount => %d\n", msg.amount);
// }
void print_CF_MSG(struct CF_MSG msg){
	printf("mtype => %ld\n" , msg.mtype);
	printf("pid => %d\n" , msg.pid);
	printf("status => %d\n" , msg.status);
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{
		printf("\tamount => %d\n" , msg.amount[i]);
	}
}

void free_CF(){
	remove_queue(CF_QID);
}

/*

	Front desk and back desk messaging

*/
void OPEN_FB_QID(){
	if ((FB_QID = open_queue(FB_KEY)) == -1)
	{
		perror("OPEN_FB_QID");
		exit(errno);
	}
}

/*
	Assumptions:
		*PID is Set previously with the front desk PID
		*
*/
int forward_CF_msg(struct CF_MSG * msg, int status){
	int res , len;
	(*(msg)).mtype = M_FB;
	(*(msg)).status = status;

	print_CF_MSG(*(msg));
	printf("Forwarding message to back desk\n");
	len = sizeof(struct CF_MSG) - sizeof(long);
	if ((res = msgsnd(FB_QID , msg , len , 0)) == -1)
	{
		printf("Error: unable to send a message\n");
		return res;
	}
	return res;
}

/*
	Assumptions:
		*msg is the same struct as was sent to the back desk
		*Amount array is full 
*/
int send_FB_RLY(struct CF_MSG * msg, int status, int received[]){
	int res , len;
	(*(msg)).mtype = (*(msg)).pid;
	(*(msg)).status = status;
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{
		(*(msg)).received[i] = received[i];
	}
	// print_CF_MSG(*(msg));
	printf("Replying from back desk\n");
	len = sizeof(struct CF_MSG) - sizeof(long);
	if ((res = msgsnd(FB_QID , msg , len , 0)) == -1)
	{
		printf("Error: unable to send a message\n");
		return res;
	}
	return res;
}

int get_FB_MSG(struct CF_MSG * msg){
	printf("getting message\n" );
	int res , len;
	len = sizeof(struct CF_MSG) - sizeof(long);
	if ((res = msgrcv(FB_QID , msg , len , M_FB , 0)) == -1)
	{
		printf("Error: unable to read a message\n");
		// return res;
		perror("get_FB_MSG");
		exit(errno);
	}
	return res;
}

int get_FB_RLY(struct CF_MSG * msg, long mytype){
	int res , len;
	len = sizeof(struct CF_MSG) - sizeof(long);
	if ((res = msgrcv(FB_QID , msg , len , mytype , 0)) == -1)
	{
		printf("Error: unable to read a message\n");
		perror("get_FB_RLY");
		return res;
	}
	return res;
}

int peek_FB(long mtype){
	return peek_message(FB_QID , mtype);
}

#endif