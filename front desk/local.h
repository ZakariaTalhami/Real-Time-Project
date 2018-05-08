#ifndef FRONT
#define FRONT _FRONT_H_

#define ARG_COUNT  4
void GET_SERVE_CUSTOEMR_PERIOD_fROM_ARGV(char *[]);
void GET_NUMBER_SWEETS_TYPES_fROM_ARGV(char *[]);
int check_for_zeros();
/*
	Reads and sets the values of SERVE_CUSTOEMR_PERIOD 
	passed from Initiator
*/
void GET_SERVE_CUSTOEMR_PERIOD_fROM_ARGV(char *argv[]){
	if((SERVE_CUSTOEMR_PERIOD[0] = atoi(argv[1]))== -1){
		perror("SERVE_CUSTOEMR_PERIOD[0]");
		exit(1);
	}

	if((SERVE_CUSTOEMR_PERIOD[1] = atoi(argv[2]))== -1){
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
#endif