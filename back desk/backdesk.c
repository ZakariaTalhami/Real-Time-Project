#include "../headers/local.h"
#include "../headers/conf.h"
#include "../headers/messaging.h"
#include "../headers/signals.h"
#include "local.h"

struct CF_MSG buf;
int seed = 0;
int readers=0;
pthread_mutex_t mutex_flags[MAX_TYPES];
pthread_cond_t	cond_flags[MAX_TYPES];
pthread_mutex_t mutex_wr , mutex_readers;
int flags[MAX_TYPES] = { 0 };
struct thread_args args[MAX_TYPES];
int main(int argc, char  *argv[])
{
	seed =time(NULL);
	printf("Hello this is a BACK desk %d\n", argc);
	if (argc != ARG_COUNT)
	{
		errno = EINVAL;
		perror("Usage: Arguements");
		exit(errno);
	}

	// print_argv(argc , argv);
	GET_AVAILABLE_SWEETS_QUANTITIES_RANGE_fROM_ARGV(argv);
 	GET_NUMBER_SWEETS_TYPES_fROM_ARGV(argv);

 	// printf("1) %d\n" , AVAILABLE_SWEETS_QUANTITIES_RANGE[0]);
 	// printf("2) %d\n" , AVAILABLE_SWEETS_QUANTITIES_RANGE[1]);
 	// printf("3) %d\n" , NUMBER_SWEETS_TYPES);
 	pthread_t sweets_t[NUMBER_SWEETS_TYPES];
 	
 	/*
		Create all the threads for each sweet
 	*/
 	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
 	{
 		// sweet_count[i] = i;
 		args[i].index = i; 
 		args[i].available = get_amount_for_type(); 
  		if(pthread_create(&sweets_t[i] ,NULL,&back_desk, (void *)&args[i]) != 0){
 			perror("pthread_create");
 			exit(errno);
 		}
 	}
 	write_available();
 	// pthread_mutex_lock(&mutex_flags[0]);
 	// printf("available %d\n", args[0].available);
 	// buf.amount[0] = 70;
 	// flags[0] = 1;
 	// pthread_mutex_unlock(&mutex_flags[0]);


 	// pthread_mutex_lock(&mutex_flags[0]);
 	// if (flags[0] == 1)
 	// {
 	// 	pthread_cond_wait(&cond_flags[0] , &mutex_flags[0]);
 	// }
 	// pthread_mutex_unlock(&mutex_flags[0]);

 	// pthread_mutex_lock(&mutex_flags[0]);
 	// printf("available round 2 %d\n", args[0].available);
 	// buf.amount[0] = 10;
 	// flags[0] = 1;
 	// pthread_mutex_unlock(&mutex_flags[0]);



 	OPEN_FB_QID();
 	printf("Back Desk Opens a Message queue with FB_QID = %d \n", FB_QID );
 	// while(peek_FB((long) pid) == -1);
 	printf("Back desk is waiting\n");
 	while(1){
 		if(peek_FB(M_FB) != -1){
 			//read order from the front desk
 			pthread_mutex_lock(&mutex_wr);
 			get_FB_MSG(&buf);
 			pthread_mutex_unlock(&mutex_wr);
 			printf("Got message from Front Desk\n");

 			printf("Signaling for workers that there is an order\n");
 			/* Signal for all workers that there is an order */
 			set_all_flags();

 			printf("Waiting until all workers have finished their work\n");
 			/* Wait until all workers have finished their work */
 			wait_to_be_served();

 			printf("all workers have finished\n");
 			/* all workers have finished */
 			pthread_mutex_lock(&mutex_wr);
 			send_FB_RLY(&buf, 200, buf.received);
 			pthread_mutex_unlock(&mutex_wr);
 		}
 	}
}

void *back_desk(void *count){
	int index = (*((struct thread_args *)count)).index;
	int available = (*((struct thread_args *)count)).available;
	// int available = get_amount_for_type();
	int amount = 0;
	int temp = 0;
	while(1){
		/*
			Wait to be signaled to work
		*/
		pthread_mutex_lock(&mutex_flags[index]);
		if(flags[index] == 0){
			pthread_cond_wait(&cond_flags[index] , &mutex_flags[index]);
		}
		pthread_mutex_unlock(&mutex_flags[index]);
		printf("Thread  %d Started work\n", index);

		/*
			Begin work
		*/
		pthread_mutex_lock(&mutex_readers);
		readers++;
		if (readers == 1)
		{
			// lock out the main function until done
			pthread_mutex_lock(&mutex_wr);
		}
		pthread_mutex_unlock(&mutex_readers);

		/* Serve the Order  */
		amount = buf.amount[index];
		printf("%d)The amount requested is %d\n", index,amount );
		if(amount> available){	//not enough quantity
			temp = available;
		}else{
			temp = amount;
		}
		printf("%d)The amount returned %d\n", index, temp );
		available-=temp;
		buf.received[index] = temp;
		printf("%d)the Amount Remaining %d\n", index, available);
		/*
			Signal That the thread has finished work
		*/
		pthread_mutex_lock(&mutex_flags[index]);
		flags[index] = 0;
		pthread_cond_signal(&cond_flags[index]);
		pthread_mutex_unlock(&mutex_flags[index]);


		pthread_mutex_lock(&mutex_readers);
		readers--;
		if (readers == 0)
		{
			// unlock for the main when done
			pthread_mutex_unlock(&mutex_wr);
		}
		pthread_mutex_unlock(&mutex_readers);
	}
	

}
void set_all_flags(){
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{	
		pthread_mutex_lock(&mutex_flags[i]);
	}

	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{	
		flags[i] =1;
	}

	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{	
		pthread_cond_signal(&cond_flags[i]);
		pthread_mutex_unlock(&mutex_flags[i]);
	}
}

void wait_to_be_served(){
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{
		pthread_mutex_lock(&mutex_flags[i]);
		if(flags[i] == 1){
			pthread_cond_wait(&cond_flags[i], &mutex_flags[i]);
		}
		pthread_mutex_unlock(&mutex_flags[i]);
	}
}
int get_amount_for_type(){
	int res = 0;
	srand(seed);   // should only be called once
	int r = rand(); 
	seed = r;
	res = ((r) % (AVAILABLE_SWEETS_QUANTITIES_RANGE[1]+1-AVAILABLE_SWEETS_QUANTITIES_RANGE[0])) + AVAILABLE_SWEETS_QUANTITIES_RANGE[0];
	return res;
}

void write_available(){
	FILE * ptr;
	ptr = fopen("available_at_start.txt", "w");
	for (int i = 0; i < NUMBER_SWEETS_TYPES; ++i)
	{
		fprintf(ptr, "%d has => %d\n", args[i].index , args[i].available);
	}
	fclose(ptr);
}