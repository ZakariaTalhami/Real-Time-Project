#ifndef BACK
#define BACK _BACK_DESK_

#include <pthread.h>
#define ARG_COUNT  4
struct thread_args
{
	int index;
	int available;
};


void GET_AVAILABLE_SWEETS_QUANTITIES_RANGE_fROM_ARGV(char *[]);
void GET_NUMBER_SWEETS_TYPES_fROM_ARGV(char *[]);
int get_amount_for_type();
void *back_desk(void *);
void set_all_flags();
void wait_to_be_served();
void write_available();

void GET_AVAILABLE_SWEETS_QUANTITIES_RANGE_fROM_ARGV(char *argv[]){
	if((AVAILABLE_SWEETS_QUANTITIES_RANGE[0] = atoi(argv[1]))== -1){
		perror("AVAILABLE_SWEETS_QUANTITIES_RANGE[0]");
		exit(1);
	}

	if((AVAILABLE_SWEETS_QUANTITIES_RANGE[1] = atoi(argv[2]))== -1){
		perror("AVAILABLE_SWEETS_QUANTITIES_RANGE[1]");
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