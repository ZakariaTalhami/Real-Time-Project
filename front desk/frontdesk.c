#include "../headers/local.h"
#include "../headers/conf.h"
#include "../headers/messaging.h"
#include "local.h"
struct CF_MSG buf;
struct IC_MSG snd;
int pid;
int temp_pid = 0;
int main(int argc, char  *argv[])
{
	pid = getpid();
	printf("Hello this is a front desk %d\n", argc);
	if (argc != ARG_COUNT)
	{
		errno = EINVAL;
		perror("Usage: Arguements");
		exit(errno);
	}

	/*
		Set the signal handler for signal SIGINT
	*/
	set_ctrl_c();

	/* Get SERVE_CUSTOEMR_PERIOD from the argv passed*/
	GET_SERVE_CUSTOEMR_PERIOD_fROM_ARGV(argv);
	GET_NUMBER_SWEETS_TYPES_fROM_ARGV(argv);

	/*
		Open Messge Queue to talk to Customers
	*/
	OPEN_CF_QID();
	printf("Front Desk Opens a Message queue with QID_C = %d \n", CF_QID );
	OPEN_FB_QID();
	printf("Front Desk Opens a Message queue with FB_QID = %d \n", FB_QID );
	/*Remove later*/
		// buf.pid = 25;
		// buf.status = 200;
		// for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
		// {
		// 	buf.amount[i] = 1;
		// }
		// print_CF_MSG(buf);
	/*
		Loop
	*/
	while(1){
		
	// 		Check see if there is a customer order waiting
		
		if(peek_CF(M_CF) != -1){
			/* Read the order from the customer*/
			get_CF_MSG(&buf);
			printf("Front (%d) got message from customer (%d)\n" ,getpid(),buf.pid);

			/* Save the PID of the Customer */
			temp_pid = buf.pid;
			/* Change the PID of the message to the front desk PID */
			buf.pid = pid;
			/* Forward the order to the back desks */
			forward_CF_msg(&buf, 200);
			/* Wait for the back desk Reply */
			printf("Front is waiting\n");
			while(peek_FB((long) pid) == -1);
			get_FB_RLY(&buf, pid);
			/* Process the back desk reply */
				//get status
			printf("Got reply with:>\n");
			print_CF_MSG(buf);
				snd.status = compare_order_response(buf);
				//Get the amount received
				snd.amount = get_amount(buf);
				printf("Front<-back: Status (%d) (%d)\n", snd.status , snd.amount );
			/* Construct the Front desk reply to the Customer */
			// print_CF_MSG(buf);
			send_CF_RLY(&snd,snd.status,snd.amount,temp_pid);
			// exit(1);
			// while(1);
		}
	}
	return 0;
}



/*
	Initiator Handler for SIGINT
*/
void catch_ctrl_c(){
	(void) signal(SIGINT , catch_ctrl_c);
	free_resources();
	exit(-1);
}

/*
	Sets the SIGINT handler
*/
void set_ctrl_c(){
	(void) signal(SIGINT , catch_ctrl_c);
}
void free_resources(){
	free_IC();
}

int check_for_zeros(){
	int count =0;
	int ret = 0;
	for (int i = 0; i < MAX_TYPES; ++i)
	{
		count += buf.amount[i];
	}
	if(count == 0){
		ret = 1;
	}

	return ret;
}

int compare_order_response(struct CF_MSG msg){
	int temp=200;
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{
		if (msg.amount[i] != msg.received[i])
		{
			temp = 404;
			break;
		}
	}
	return temp;
}

int get_amount(struct CF_MSG msg){
	int temp=0;
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{
		temp+=msg.received[i];
	}

	return temp;
}