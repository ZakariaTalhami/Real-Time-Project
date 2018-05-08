#ifndef __LOCAL_H__
#define __LOCAL_H__


#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
/*
	Initiator 
*/
void init_front_desk();
void init_customer();
void customer_arrives();
void set_alrm_customer_arrive();
int get_customer_create_time();
void free_resources();
void set_ctrl_c();
void catch_ctrl_c();
void print_state();

#ifndef BUF_SIZE
#define BUF_SIZE 80
#endif

#ifndef MAXLINE
#define MAXLINE 125
#endif

#ifndef MAX_TYPES
#define MAX_TYPES 20
#endif

#ifndef PROB_TH
#define PROB_TH 70
#endif
struct MSG
{
	long mtype;
	int PID;
	char msg[BUF_SIZE];
};


void print_argv(int argc , char *argv[]){
	for (int i = 0; i < argc; ++i)
	{
		printf("%s\n", argv[i]);
	}
}

#endif