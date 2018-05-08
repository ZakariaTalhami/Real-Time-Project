int open_queue(key_t );
int send_message(int  , struct MSG * , int);
int get_message(int  , struct MSG *  , long  ,int );
int peek_message(int  , long );
int remove_queue(int );
int stat_queue(int  , struct msqid_ds * );

/*
	Creates or opens a queue from the provided Key value 
		returns the Queue id of the opened queue
*/
int open_queue(key_t key){

	int qid;
	if((qid  = msgget(key , IPC_CREAT|0666)) == -1){
		printf("Error: Unable to open Queue\n");
		return errno;
	}
	return qid;
}

/*
	Sends a message trough the Provided queue (qid)
		message is provided as a pointer to the struct
		per: 0 for blocking else IPC_NOWAIT
*/
int send_message(int qid , struct MSG * msg , int per){
	int res , len;
	len = sizeof(struct MSG) - sizeof(long);
	if ((res = msgsnd(qid , msg , len , per)) == -1)
	{
		printf("Error: unable to send a message\n");
		return res;
	}
	return res;
}

/*
	reveives a message from the provided queue with the given mtype
*/
int get_message(int qid , struct MSG * msg , long type ,int per){
	int res , len;
	len = sizeof(struct MSG) - sizeof(long);
	if ((res = msgrcv(qid , msg , len , type , per)) == -1)
	{
		printf("Error: unable to read a message\n");
		return res;
	}
	return res;
}

/*
	Checks if there is a message in the queue of the provided type
*/
int peek_message(int qid , long type){
	int res;
	if ((res = msgrcv(qid , NULL , 0 , type , IPC_NOWAIT)) == -1)
	{
		if (errno == E2BIG)
		{
			return 0;
		}
	}
	return -1;
}

int remove_queue(int qid){
	int res;
	if((res  =  msgctl(qid , IPC_RMID , NULL)) == -1){
		printf("Error: unable to remove Queue\n");
		return errno;
	}
	return res;
}
int stat_queue(int qid , struct msqid_ds * buf){
	int res;
	if((res  =  msgctl(qid , IPC_STAT, buf)) == -1){
		printf("Error: unable to remove Queue\n");
		return errno;
	}
	return res;
}

