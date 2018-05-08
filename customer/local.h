#ifndef FRONT
#define FRONT _CUSTOMER_H_

#define ARG_COUNT  5
void GET_SERVE_CUSTOEMR_PERIOD_fROM_ARGV(char *[]);
void GET_ORDER_RANGE_PER_TYPE_fROM_ARGV(char *[]);
int get_customer_wait_time();
void catch_alarm(int );
void set_alrm_customer_wait();
int get_amount_for_type();
int probability_to_order();
void prepare_order();
/*
	Reads and sets the values of SERVE_CUSTOEMR_PERIOD 
	passed from Initiator
*/
void GET_SERVE_CUSTOEMR_PERIOD_fROM_ARGV(char *argv[]){
	if((WAIT_CUSTOEMR_PERIOD[0] = atoi(argv[1]))== -1){
		perror("SERVE_CUSTOEMR_PERIOD[0]");
		exit(1);
	}

	if((WAIT_CUSTOEMR_PERIOD[1] = atoi(argv[2]))== -1){
		perror("SERVE_CUSTOEMR_PERIOD[1]");
		exit(1);
	}
}

void GET_NUMBER_SWEETS_TYPES_fROM_ARGV(char *argv[]){
	
	if((NUMBER_SWEETS_TYPES = atoi(argv[3]))== -1){
		perror("NUMBER_SWEETS_TYPES");
		exit(1);
	}
}

void GET_ORDER_RANGE_PER_TYPE_fROM_ARGV(char *argv[]){
	
	if((ORDER_RANGE_PER_TYPE = atoi(argv[4]))== -1){
		perror("ORDER_RANGE_PER_TYPE");
		exit(1);
	}
}
#endif