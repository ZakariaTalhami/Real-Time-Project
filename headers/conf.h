#ifndef CONFIG
#define CONFIG __CONFIG_H__

void readConfig(char *);
void GET_NUMBER_SWEETS_TYPES(FILE *);
void GET_AVAILABLE_SWEETS_QUANTITIES_RANGE(FILE * );
void GET_VALUE(FILE *  , int *);
void GET_RANGE(FILE * , int * );
void print_config();


/*
		Configuration settings
*/
int NUMBER_SWEETS_TYPES;
int AVAILABLE_SWEETS_QUANTITIES_RANGE[2];
int NUMBER_FRONT_DESK;
int NUMBER_BACK_DESK;
int CUSTOMER_ARRIVE_EVERY[2];
int SERVE_CUSTOEMR_PERIOD[2];
int WAIT_CUSTOEMR_PERIOD[2];
int ORDER_RANGE_PER_TYPE;
int MAX_UNSERVED_CUSTOMERS;
int MAX_FRUSTRATED_CUSTOMERS;
int MAX_DAILY_INCOME;
int MAX_SERVED_CUSTOMERS;


/*
	Reads from the configuration file and 
	sets the values of the setting above
*/
void readConfig(char  *file){
	FILE * fp;
	char bufr[MAXLINE];

	if((fp = fopen(file, "r")) != NULL){

	    	GET_VALUE(fp,&NUMBER_SWEETS_TYPES);
	    	GET_RANGE(fp , AVAILABLE_SWEETS_QUANTITIES_RANGE);
	    	GET_VALUE(fp,&NUMBER_FRONT_DESK);
	    	GET_VALUE(fp,&NUMBER_BACK_DESK);
	    	GET_RANGE(fp , CUSTOMER_ARRIVE_EVERY);
	    	GET_RANGE(fp , SERVE_CUSTOEMR_PERIOD);
	    	GET_VALUE(fp,&ORDER_RANGE_PER_TYPE);
	    	GET_RANGE(fp , WAIT_CUSTOEMR_PERIOD);
	    	GET_VALUE(fp,&MAX_UNSERVED_CUSTOMERS);
	    	GET_VALUE(fp,&MAX_FRUSTRATED_CUSTOMERS);
	    	GET_VALUE(fp,&MAX_DAILY_INCOME);
	    	GET_VALUE(fp,&MAX_SERVED_CUSTOMERS);
	    	fclose(fp);
	} else {
	    perror("");
	    exit(errno);
	}
}

/*
	Reads a line from the configuration file 
	extracts the value at the end of the line

	takes:
		file : configuration FILE 
		ret : address of the configuration settings to be set
*/
void GET_VALUE(FILE * file , int * ret){
	char bufr[MAXLINE];
	char * t;
	fgets(bufr, MAXLINE, file);
	bufr[strcspn(bufr, "\n")] = 0;
	strtok(bufr ,"=");
	t =  strtok(NULL ,"");
	// printf("%s\n", t);
	fflush(stdout);
	// printf("%d\n", atoi(t));
	*ret = atoi(t);
}

/*
	Reads a line from the configuration file 
	extracts the range at the end of the line

	takes:
		file : configuration FILE 
		ret : address of the configuration settings to be set
*/
void GET_RANGE(FILE * file, int * ret){
	char bufr[MAXLINE];
	char * t;
	char *temp;
	
	fgets(bufr, MAXLINE, file);
	bufr[strcspn(bufr, "\n")] = 0;
	strtok(bufr ,"=");
	t =  strtok(NULL ,"");
	// printf("%s\n", t);
	fflush(stdout);

	temp = strsep(&t , " ");
	// printf("%s\n", temp);
	// printf("%s\n", t);
	ret[0] = atoi(temp);
	ret[1] = atoi(t);
	// printf("%d\n", atoi(t));
	// temp = atoi(t);
	// return temp;
}

/*
	Prints all the configurations
*/
void print_config(){
	printf("NUMBER_SWEETS_TYPES:> %d\n", NUMBER_SWEETS_TYPES);
	printf("AVAILABLE_SWEETS_QUANTITIES_RANGE:> %d %d \n",
		AVAILABLE_SWEETS_QUANTITIES_RANGE[0],
		AVAILABLE_SWEETS_QUANTITIES_RANGE[1]
	 );
	printf("NUMBER_FRONT_DESK:> %d\n", NUMBER_FRONT_DESK);
	printf("NUMBER_BACK_DESK:> %d\n", NUMBER_BACK_DESK);
	printf("CUSTOMER_ARRIVE_EVERY:> %d %d \n",
		CUSTOMER_ARRIVE_EVERY[0],
		CUSTOMER_ARRIVE_EVERY[1]
	 );
	printf("SERVE_CUSTOEMR_PERIOD:> %d %d \n",
		SERVE_CUSTOEMR_PERIOD[0],
		SERVE_CUSTOEMR_PERIOD[1]
	 );
	printf("ORDER_RANGE_PER_TYPE:> %d\n", ORDER_RANGE_PER_TYPE);
	printf("WAIT_CUSTOEMR_PERIOD:> %d %d \n",
		WAIT_CUSTOEMR_PERIOD[0],
		WAIT_CUSTOEMR_PERIOD[1]
	 );

	printf("MAX_UNSERVED_CUSTOMERS:> %d\n", MAX_UNSERVED_CUSTOMERS);
	printf("MAX_FRUSTRATED_CUSTOMERS:> %d\n", MAX_FRUSTRATED_CUSTOMERS);
	printf("MAX_DAILY_INCOME:> %d\n", MAX_DAILY_INCOME);
	printf("MAX_SERVED_CUSTOMERS:> %d\n", MAX_SERVED_CUSTOMERS);
}
#endif