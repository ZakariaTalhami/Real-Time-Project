#include "../headers/local.h"
#include "../headers/conf.h"
#include "../headers/messaging.h"
#include "local.h"
struct CF_MSG buf;
struct IC_MSG snd;
int main(int argc, char  *argv[])
{
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

	/*For debugging purposes*/
	// print_argv(argc , argv);

	/* Get SERVE_CUSTOEMR_PERIOD from the argv passed*/
	GET_SERVE_CUSTOEMR_PERIOD_fROM_ARGV(argv);
	GET_NUMBER_SWEETS_TYPES_fROM_ARGV(argv);
	
	// printf("GET_NUMBER_SWEETS_TYPES_fROM_ARGV  %d\n", NUMBER_SWEETS_TYPES);

	// OPEN_IC_QID();
	// printf("QID = %d \n", IC_QID );
	// while(peek(M_CI) == -1);
	// printf("READing...\n");
	// get_IC_MSG(&buf);
	// printf("Got m: %ld , PID :%d  status:%d  \n" ,buf.mtype ,buf.pid , buf.status);
	OPEN_CF_QID();
	printf("Front Desk Opens a Message queue with QID_C = %d \n", CF_QID );
	// exit(1);
	while(1){
		// printf("peek :> %d\n",peek_CF(M_CF) );
		if(peek_CF(M_CF) != -1){
			get_CF_MSG(&buf);
			printf("Front (%d) got message from customer (%d)" ,getpid(),buf.pid);
			print_CF_MSG(buf);
			send_CF_RLY(&snd,200,5,buf.pid);
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