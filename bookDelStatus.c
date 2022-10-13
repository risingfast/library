/*  bookDelStatus.c - CGI to delete a status from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 13-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      13-Dec-2021 start by copying bookDelStatus.c
 *      13-Dec-2021 check if no status deleted
 *      15-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
 *  Enhancements:
*/

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

#define SQL_LEN 5000

#define MAXLEN 1024

// global declarations -------------------------------------------------------------------------------------------------

char *sgServer = "192.168.0.13";                                                                //mysqlServer IP address
char *sgUsername = "gjarman";                                                               // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
int  iStatusID = 0;
int  iDelRows = 0;

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS <header> block -----------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// Initialize a connection and connect to the database -----------------------------------------------------------------

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, sgServer, sgUsername, sgPassword, sgDatabase, 0, NULL, 0))
    {
        printf("Failed to connect to MySQL Server %s in module %s()", sgServer, __func__);
        printf("\n\n");
        printf("Error: %s\n", mysql_error(conn));
        printf("\n");
        return  EXIT_FAILURE;
    }

//    setenv("QUERY_STRING", "statusID=19", 1);                                            // uncomment for testing only

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"statusID=<99>\". Terminating bookDelStatus.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty query string ---------------------------------------------------------------------------------------

    if(sParam[0] == '\0') {
        printf("Query string is empty. Expecting QUERY_STRING=\"statusID=<99>\". Terminating bookDelStatus.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize the ratingID value -------------------------------------------------

    sscanf(sParam, "statusID=%d", &iStatusID);
    if(iStatusID == 0) {
        printf("Status ID is 0. Expecting QUERY_STRING=\"statusID=<99>\". Terminating bookDelStatus.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// set a SQL query to insert the new author ---------------------------------------------------------------------------

    sprintf(caSQL, "DELETE FROM risingfast.`Book Statuses` "
                   "WHERE `Status ID` = %d;", iStatusID);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    iDelRows = (int) mysql_affected_rows(conn);

    if(iDelRows == 0) {
        printf("No rows deleted. Status ID not found\n");
    } else {
        printf("Character ID '%d' deleted", iStatusID);
    }

    return EXIT_SUCCESS;
}
