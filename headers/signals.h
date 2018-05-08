// void catch_alarm(int );
// void set_alrm_customer_wait();

// void catch_alarm(int signum){
// (void) signal(SIGALRM , catch_alarm);
// 	int status = 408;
// 	struct IC_MSG reply;
// 	if(IC_QID != -1){
// 		send_IC_MSG(&reply , status , -1);
// 	}
// 	printf("Customer Leaving!\n");
	
// 	exit(status)
// }


// void set_alrm_customer_wait(){
// 	(void) signal(SIGALRM , catch_alarm);
// }
