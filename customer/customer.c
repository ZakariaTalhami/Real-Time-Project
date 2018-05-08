#include "../headers/local.h"
#include "../headers/conf.h"
#include "../headers/messaging.h"
#include "../headers/signals.h"
#include "local.h"
struct IC_MSG buf;
struct CF_MSG buf_CF;
int status[] = {200,404,408};
int amount[MAX_TYPES] = { 0 };
int wait_time = 0;
int seed = 0;
int main(int argc, char  *argv[])
{
	seed =time(NULL);
	// CF_MSG - malloc(struct CF_MSG);
	/*
		Make sure that the customer got all
		the configuration data
	*/
	// printf("Hello this is a customer %d\n", argc);
	if (argc != ARG_COUNT)
	{
		errno = EINVAL;
		perror("Usage: Arguements");
		exit(errno);
	}
	GET_SERVE_CUSTOEMR_PERIOD_fROM_ARGV(argv);
	GET_NUMBER_SWEETS_TYPES_fROM_ARGV(argv);
	GET_ORDER_RANGE_PER_TYPE_fROM_ARGV(argv);
	
	/*
		Set the signal handler for signal SIGINT
	*/
	set_ctrl_c();
	/*
		Set the signal handlers for SIGALRM
	*/
	set_alrm_customer_wait();
	//get wait time
	wait_time = get_customer_wait_time();
	printf("Customer %d  with %d wait time \n",  getpid() , wait_time);
	//set up the timer for timeout
	
	alarm(wait_time);


	/*
		Open the Message Queue
			//public queue 
			//priavte queue
	*/
	//public queue
	OPEN_IC_QID();
	printf("Customer Opens a Message queue with QID_C = %d \n", IC_QID );

	//private queue
	OPEN_CF_QID();
	printf("Customer Opens a Message queue with QID_C = %d \n", CF_QID );


	/*
		Prepare Customer Order
	*/
	prepare_order();
	// for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	// {
	// 	printf("Type %d  => %d\n", i , amount[i]);
	// }
	// exit(0);


	/*
		Send Order to the Front desk
	*/
	send_CF_MSG(&buf_CF, 0 , amount);

	while(1);
	/*
		Wait for front desk reply
	*/

	/*
		Get front desk reply 
			//From the private queue
			//stop the timer
	*/

	/*
		process front desk reply
	*/

	/*
		Send Response of status to Initiator
	*/











	/*For debugging purposes*/
	// print_argv(argc , argv);

	// printf("QID = %d \n", IC_QID );

	// printf("Sending\n");
	// send_IC_MSG(&buf , status[getpid()%3] , getpid()%10);
	// printf("Sent\n");
	// remove_queue(IC_QID);
	while(1);
	return 0;
}

int get_customer_wait_time(){
	int res = 0;
	srand(time(NULL));   // should only be called once
	int r = rand(); 
	res = (r) % (WAIT_CUSTOEMR_PERIOD[1]+1-WAIT_CUSTOEMR_PERIOD[0]) + WAIT_CUSTOEMR_PERIOD[0];
	return res;
}

void catch_alarm(int signum){
(void) signal(SIGALRM , catch_alarm);
	int status = 404;
	struct IC_MSG reply;
	if(IC_QID != -1){
		send_IC_MSG(&reply , status , 3);
	}
	printf("Customer Leaving!\n");
	
	exit(status);
}


void set_alrm_customer_wait(){
	(void) signal(SIGALRM , catch_alarm);
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

void prepare_order(){
	printf("in prepare oreder\n");
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{
		if(probability_to_order()){
			amount[i] = get_amount_for_type();
		}
	}
}

int get_amount_for_type(){
	int res = 0;
	srand(seed);   // should only be called once
	int r = rand(); 
	seed = r;
	res = (r) % ORDER_RANGE_PER_TYPE;
	return res;
}

int probability_to_order(){
	int res = 0;
	srand(seed);   // should only be called once
	int r = rand(); 
	seed = r;
	// printf("%d PROB_TH\n", r % 100);
	if ((r % 100)>= PROB_TH )
	{
		res = 1;
	}
	return res;
}

void free_resources(){
	free_CF();
}