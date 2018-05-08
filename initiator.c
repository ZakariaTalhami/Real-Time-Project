
#include "headers/local.h"
#include "headers/conf.h"
#include "headers/messaging.h"
#include "headers/signals.h"

void process_msg( struct IC_MSG);
int * f_pid;
struct IC_MSG buf;
int createCustomerFlag = 0;

int UNSERVED_CUSTOMERS=0;
int FRUSTRATED_CUSTOMERS=0;
int DAILY_INCOME=0;
int SERVED_CUSTOMERS=0;
int TERNIMATE_CODE=0;

int main(int argc, char  *argv[])
{
		int waitTime = 0;
	/*
		Make Sure a argument was passed
	*/
	if(argc != 2){
		errno = EINVAL;
		perror("Usage: Configuration File");
		exit(-1);
	}

	/*
		Make sure the provided file exists
	*/
	if ((access(argv[1] , F_OK)))
	{
		perror("");
		return errno;
	}	
	/*
		Setup the signal handlers
	*/
	set_alrm_customer_arrive();
	set_ctrl_c();
	/*
		Read the Configuration File
	*/
	readConfig(argv[1]);
	print_config();

	/*
		Load up the front desk processes
			/pass the SERVE_CUSTOEMR_PERIOD
	*/
	init_front_desk();
	printf("The Front Desk Employees have arrived and taken their post\n");

	/*
		Load up the front desk processes
			/pass the SERVE_CUSTOEMR_PERIOD
	*/
	// todo: Load the back desk program

	/*Open message Queue for all customers*/
	OPEN_IC_QID();
	printf("Created Message queue with QID = %d \n", IC_QID );

	/*
		Customer  Create Loop
	*/

	while(1){
		waitTime = get_customer_create_time();		//get time between customers
		printf("Random Time => %d\n", waitTime);
		alarm(waitTime);								//set timer for creation
		while(!createCustomerFlag){
			if(peek(M_CI) != -1){
				printf("READing...\n");
				get_IC_MSG(&buf);
				printf("Got m: %ld , PID :%d  status:%d  amount:%d\n" ,buf.mtype ,buf.pid , buf.status, buf.amount);
				process_msg(buf);
			}
		}
		init_customer();
		createCustomerFlag = 0;
	}
	// printf("QID = %d \n", IC_QID );
	// OPEN_IC_QID();
	// printf("QID = %d \n", IC_QID );
	// while(peek(M_CI) == -1);
	// printf("READing...\n");
	// get_IC_MSG(&buf);
	// printf("Got m: %ld , PID :%d  status:%d  \n" ,buf.mtype ,buf.pid , buf.status);
	// remove_queue(IC_QID);
	while(1);
	return 0;
}

/*
	Free all open resources of the programe
*/
void free_resources(){
	free_IC();
}


/*
	Creates the Front desk processes
		creates depending on the Configuration 
			"NUMBER_FRONT_DESK"
		the code forks and exec the front programe
		passes the "SERVE_CUSTOEMR_PERIOD" configuration
*/
void init_front_desk(){
	int temp_pid;
	//set the arguements to be passed to front desk
	char * args[5];
	f_pid = malloc(NUMBER_FRONT_DESK*sizeof(int));
	args[0] = "./frontdesk";
	args[1] = malloc(sizeof(char)*4);
	args[2] = malloc(sizeof(char)*4);
	args[3] = malloc(sizeof(char)*4);
	sprintf(args[1] ,"%d" , SERVE_CUSTOEMR_PERIOD[0]);
	sprintf(args[2],"%d" , SERVE_CUSTOEMR_PERIOD[1]);
	sprintf(args[3],"%d" , NUMBER_SWEETS_TYPES);
	args[4] = NULL;
	for (int i = 0; i < NUMBER_FRONT_DESK; ++i)
	{
		// printf(" i = %d\n", i );
		if ((temp_pid = fork()))
		{
			// printf("PID = %d\n", temp_pid);
			f_pid[i] = temp_pid;
		}
		else{
			execv("/home/zakaria/Documents/RealTime/front desk/frontdesk" , args);
			perror("execv");
			exit(1);
		}
	}
}


/*
	Creates the Customer processes
		the code forks and exec the Customer programe
		passes the "WAIT_CUSTOEMR_PERIOD" configuration
*/

// todo: Add the max number of types
void init_customer(){
	int temp_pid;
	//set the arguements to be passed to front desk
	char * args[6];
	args[0] = "./customer";
	args[1] = malloc(sizeof(char)*4);
	args[2] = malloc(sizeof(char)*4);
	args[3] = malloc(sizeof(char)*4);
	args[4] = malloc(sizeof(char)*4);
	sprintf(args[1] ,"%d" , WAIT_CUSTOEMR_PERIOD[0]);
	sprintf(args[2],"%d" , WAIT_CUSTOEMR_PERIOD[1]);
	sprintf(args[3],"%d" , NUMBER_SWEETS_TYPES);
	sprintf(args[4],"%d" , ORDER_RANGE_PER_TYPE);
	args[5] = NULL;
	if ((temp_pid = fork()))
	{
		printf("PID = %d\n", temp_pid);
		// f_pid[i] = temp_pid;
	}
	else{
		execv("/home/zakaria/Documents/RealTime/customer/customer" , args);
		perror("execv");
		exit(1);
	}
}

/*
	Initiator Handler for SIGAlRM
*/
void customer_arrives(){
	(void) signal(SIGALRM , customer_arrives);
	createCustomerFlag = 1;
}

/*
	Sets the SIGALRM handler
*/
void set_alrm_customer_arrive(){
	(void) signal(SIGALRM , customer_arrives);
}

/*
	Initiator Handler for SIGINT
*/
void catch_ctrl_c(){
	(void) signal(SIGINT , catch_ctrl_c);
	free_resources();
	switch(TERNIMATE_CODE){

		case 0: // ctrl_c was entered in the terminal
			printf("\nTERMINATED BY THE USER IN THE TERMINAL\n");
			break;

		case 1: // daily income limite reached
			printf("\nTERMINATED: REACHED DAILY INCOME LIMIT\n");
			break;

		case 2: // unserved customer limit reached
			printf("\nTERMINATED: REACHED UNSERVED CUSTOMERS LIMIT\n");
			break;

		case 3: // FRUSTRATED_CUSTOMERS limit reached
			printf("\nTERMINATED: REACHED FRUSTRATED_CUSTOMERS LIMIT\n");
			break;

		case 4: // served customer limite reached
			printf("\nTERMINATED: REACHED SERVED CUSTOMERS LIMIT\n");
			break;

	}
	exit(TERNIMATE_CODE);
}

/*
	Sets the SIGINT handler
*/
void set_ctrl_c(){
	(void) signal(SIGINT , catch_ctrl_c);
}

/*
	Gets the random value that represents 
	the time between customer arriving.

	the Value is between CUSTOMER_ARRIVE_EVERY values 
	read from the configuration file
*/
int get_customer_create_time(){
	int res = 0;
	srand(time(NULL));   // should only be called once
	int r = rand(); 
	res = (r) % (CUSTOMER_ARRIVE_EVERY[1]+1-CUSTOMER_ARRIVE_EVERY[0]) + CUSTOMER_ARRIVE_EVERY[0];
	return res;
}

/*
	Processes the status of the messsages received from the 
	Customers, updates variables accordingly 
	also checks for termination states
*/
void process_msg(struct IC_MSG buf){
	int status = buf.status;

	switch(status){

		case 100:
			break;

		case 200: // 200 ok: order complete
			DAILY_INCOME+= buf.amount;
			SERVED_CUSTOMERS++;
			if (DAILY_INCOME == MAX_DAILY_INCOME)
			{
				TERNIMATE_CODE = 1;
				raise(SIGINT);
			}
			if (SERVED_CUSTOMERS == MAX_SERVED_CUSTOMERS)
			{
				TERNIMATE_CODE = 4;
				raise(SIGINT);
			}
			break;

		case 408:// 408 Request timeout: Took too long to be served
			UNSERVED_CUSTOMERS++;
			if (UNSERVED_CUSTOMERS == MAX_UNSERVED_CUSTOMERS)
			{
				TERNIMATE_CODE = 2;
				raise(SIGINT);
			}
			break;

		case 404:// 404 not found: Item type not found
			FRUSTRATED_CUSTOMERS++;
			DAILY_INCOME+= buf.amount;
			if (FRUSTRATED_CUSTOMERS == MAX_FRUSTRATED_CUSTOMERS)
			{
				TERNIMATE_CODE = 3;
				raise(SIGINT);
			}
			if (DAILY_INCOME == MAX_DAILY_INCOME)
			{
				TERNIMATE_CODE = 1;
				raise(SIGINT);
			}
			break;

	}
	/*
		For debuging 
	*/
	print_state();
}

/*
	Prints the state at which the program has reached, this includes:
		 UNSERVED_CUSTOMERS
		 FRUSTRATED_CUSTOMERS
		 DAILY_INCOME
		 SERVED_CUSTOMERS
		 TERNIMATE_CODE
*/
void print_state(){
	printf("UNSERVED_CUSTOMERS = %d / %d\n", UNSERVED_CUSTOMERS , MAX_UNSERVED_CUSTOMERS);
	printf("FRUSTRATED_CUSTOMERS = %d / %d\n", FRUSTRATED_CUSTOMERS , MAX_FRUSTRATED_CUSTOMERS);
	printf("DAILY_INCOME = %d / %d\n", DAILY_INCOME , MAX_DAILY_INCOME);
	printf("SERVED_CUSTOMERS = %d / %d\n", SERVED_CUSTOMERS , MAX_SERVED_CUSTOMERS);
}