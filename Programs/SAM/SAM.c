#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>



// this funtion is to print out data to the terminal
void show() {
	
// variables to do the task
	MYSQL *connect01;
    	MYSQL_RES *res;
    	MYSQL_ROW row;

    	char *server = "add_server";
    	char *user = "add_username";
    	char *password = "add_password";
    	char *database = "add database";

    	connect01 = mysql_init(NULL);

	
// connection to database 
    	if (!mysql_real_connect(connect01, server,
                            user, password, database, 0, NULL, 0)) {
        	fprintf(stderr, "%s\n", mysql_error(connect01));
        	exit(1);
    }
// showtables for database;
   
    	if (mysql_query(connect01, "show tables\n")) {
		
        	fprintf(stderr, "%s\n", mysql_error(connect01));
        	exit(1);
    	}
	
    	res = mysql_use_result(connect01);
	

// print out the database name adn table name
	while ((row = mysql_fetch_row(res)) != NULL)
        	printf("%s \n", row[0]);
	

	printf("+-----------------------------+\n");
	printf("\n");
	printf("MySQL Tables in mysql database:\n");
	printf("\n");
	printf("Database selected: \n");
	
    	
// usign the mysql_query to use a mysql statement to show the data from the database table	
	if (mysql_query(connect01, "SELECT * FROM workhours")) {
		printf("error: %s\r\n", mysql_error(connect01));
		mysql_close(connect01);
		exit(1);
	}
	
	MYSQL_RES *result = mysql_store_result(connect01);
	
	if (result == NULL) {
		printf("error: %s\r\n", mysql_error(connect01));
		mysql_close(connect01);
		exit(1);
	}

	// Codepart to print the column and row borders

	int lines = mysql_num_fields(result);
	int i;


	/* Here we print the titles*/
	printf("+");
	
	for(i = 0; i < lines; i++) {
    		printf("-----------------+");
	}
	printf("\n| id              | date            | day             | time            | 	extratime |	\n");
	printf("+");


	for(i = 0; i < lines; i++) {
	    	printf("-----------------+");
	}
	printf("\n");

// this loop will print the column and row lines out to terminal

	while ((row = mysql_fetch_row(result))) {
    
    		printf("| %-15s ", row[0] ? row[0] : "NULL");
    		printf("| %-15s ", row[1] ? row[1] : "NULL");
    		printf("| %-15s ", row[2] ? row[2] : "NULL");
    		printf("| %-15s ", row[3] ? row[3] : "NULL");
    		printf("| %-15s |\n", row[4] ? row[4] : "NULL");
    		// Print separator between rows
    		printf("+");
    	
		for(i = 0; i < lines; i++) {
        		printf("-----------------+");
    	}
    	
	printf("\n");
	
	}
			
	mysql_free_result(result);
	mysql_free_result(res);
    	mysql_close(connect01);
	
}


// function to add data to the server	
void add(){

// variables 	
	MYSQL *connect01;
    	MYSQL_STMT *stmt;

    	char *server = "add server ";
    	char *user = "add_user";
    	char *password = "add_password";
    	char *database = "add_database";
	
	connect01 = mysql_init(NULL);

// connection to database
    	if (!mysql_real_connect(connect01, server,
                            user, password, database, 0, NULL, 0)) {
        	fprintf(stderr, "%s\n", mysql_error(connect01));
        	exit(1);
    	}

// variabes for data with MYSQL_BIND funcion with a prepared statement.
// theese 4 variables ar going to be passed in a scanf function and over to store the data in the db
	MYSQL_BIND data[4];
	int date;
	char day[100];	
	int time; 
	int minutes;

// using the mysql_stmt_prepare we store the mysql query in a variable for both ints and strings.
	
	stmt = mysql_stmt_init(connect01);
	mysql_stmt_prepare(stmt, "INSERT INTO workhours (date, day, time, minutes)	VALUES (?,?,?,?)", strlen("INSERT INTO workhours 	(date,day, time, minutes) 	VALUES (?,?,?,?)")); 


// here the data is going to be inserted , stored in a variable and passed  over to the table, note that data to be inserted
// are chars and strings. so the conversion is defferent
	printf("Enter date: \n");
	scanf("%d", &date);
	

	printf("Enter day: \n");
	scanf("%s", &day);
	unsigned long str_length_day = strlen(day);

	printf("Enter amount of hours worked: \n");
	scanf("%d", &time);

	printf("Enter extra time: \n");
	scanf("%d", &minutes);

// this part passes the data inserted and handles the "memory distribution" to be stored in the database
	memset(data, 0, sizeof(data));

	data[0].buffer_type = MYSQL_TYPE_LONG;
	data[0].buffer = (char *)&date;
	data[0].is_null = 0;
	data[0].length = 0;

	data[1].buffer_type = MYSQL_TYPE_STRING;
	data[1].buffer = (char *)day;
	data[1].is_null = 0;
	data[1].length = &str_length_day;

	data[2].buffer_type = MYSQL_TYPE_LONG;
	data[2].buffer = (char *)&time;
	data[2].is_null = 0;
	data[2].length = 0;

	data[3].buffer_type = MYSQL_TYPE_LONG;
	data[3].buffer = (char *)&minutes;
	data[3].is_null = 0;
	data[3].length = 0;

	mysql_stmt_bind_param(stmt, data);

	if(mysql_stmt_execute(stmt)!= 0) {

		printf("Error: %s\n", mysql_stmt_error(stmt));
	}

	printf("+------------------------+\n");
	printf("Data added to database\r\n");
	printf("\n");
	mysql_stmt_close(stmt);
	
	printf("Closing connection.\r\n");
	printf("\n");
	printf("+------------------------+\n");
	mysql_close(connect01);


	
}

void delete_data() {
	MYSQL *connect01;
    	MYSQL_STMT *stmt;

	// variables
    	char *server = "add_server";
    	char *user = "add_user";
    	char *password = "add_password";
    	char *database = "add_database";
	
	connect01 = mysql_init(NULL);

   	// connection to database
    	if (!mysql_real_connect(connect01, server,
                            user, password, database, 0, NULL, 0)) {
        	fprintf(stderr, "%s\n", mysql_error(connect01));
        	exit(1);
    	}

	
	MYSQL_BIND data[1];
	int id;
	// using prepraed statement stored in a integer named id that is passed on to memset and data
	stmt = mysql_stmt_init(connect01);
	mysql_stmt_prepare(stmt,"DELETE from workhours WHERE time_id=?",strlen ("DELETE from workhours WHERE time_id=?"));
	
	printf("Select the ID you wish to delete:  \n");
	scanf("%d", &id);

	memset(data, 0, sizeof(data));

	data[0].buffer_type = MYSQL_TYPE_LONG;
	data[0].buffer = (char *)&id;
	data[0].is_null = 0;
	data[0].length = 0;

	mysql_stmt_bind_param(stmt, data);

	if(mysql_stmt_execute(stmt)!= 0) {

		printf("Error: %s\n", mysql_stmt_error(stmt));
	}

	printf("+------------------------+\n");
	printf("Data deleted\r\n");
	printf("\n");
	mysql_stmt_close(stmt);
	
	printf("Closing connection.\r\n");
	printf("\n");
	printf("+------------------------+\n");
	mysql_close(connect01);

}

void show_count() {

// Variables 	
	MYSQL *connect01;
    	MYSQL_RES *res;
    	MYSQL_ROW row;

    	char *server = "add_server";
    	char *user = "add_user";
    	char *password = "add_password";
    	char *database = "add_database";

    	connect01 = mysql_init(NULL);

   // connection to database 
    	if (!mysql_real_connect(connect01, server,
                            user, password, database, 0, NULL, 0)) {
        	fprintf(stderr, "%s\n", mysql_error(connect01));
        	exit(1);
    	}

	// query for mysql as sum of all hours stored and sum of all minutes stored
	char query[200] = "SELECT SUM(time) as Total_Hours, SUM(minutes) as Total_Extra_Minutes FROM workhours";

	if (mysql_query(connect01, query)) {

		fprintf(stderr, "%s\n", mysql_error(connect01));
        	exit(1);

	}


	res = mysql_use_result(connect01);

	while((row = mysql_fetch_row(res)) != NULL) {
		
		
		printf("Total Hours: %s\n", row[0]);
		printf("Total Extra Minutes: %s\n" , row[1]);
		

	}

	mysql_free_result(res);
	mysql_close(connect01);
	


}

void artwork() {
printf("\n");
printf("███████╗    █████╗    ███╗   ███╗\n");
printf("██╔════╝   ██╔══██╗   ████╗ ████\n");
printf("███████╗   ███████║   ██╔████╔██\n");
printf("╚════██║   ██╔══██║   ██║╚██╔╝██║\n");
printf("███████║██╗██║  ██║██╗██║ ╚═╝ ██║\n");
printf("╚══════╝╚═╝╚═╝  ╚═╝╚═╝╚═╝     ╚═╝\n");
printf("Student Assistant Manager v1.1\n");
printf("\n");


}

int main(int argc, char **argv) {
	
// art function.
	artwork();
	
// menu
	
		int choice = 1;
	while(1) {
		printf("1.) Add Data\n");
		printf("2.) Show Data\n");
		printf("3.) Delete Data\n");
		printf("4.) Show Total Hours\n");
		printf("5.) Exit\n");
		printf("\n");
		printf("Enter Choice: \n");
		
		printf("+-----------------------------+\n");
		
		if(scanf("%d", &choice) != 1) {

			printf("Invalid input, Use numbers instead of letters.\n");
			printf("Restart program.\n");
			
			break;
		
		}
		
		fflush(stdin);

		switch(choice) {
			
			case 1:
				
				add();
				break;
			
			case 2:
				show();
				printf("\n");
				break;

			case 3: 
				delete_data();
				printf("\n"); 
				break;
			case 4: 
				show_count();
				printf("\n");
				break;

			case 5: 
				printf("\n");
				printf("Bye!\n");
				exit(0);
			default:
				printf("Invalid choice\n");
				printf("\n");
				break;

		}
	}

	return 0;

}
