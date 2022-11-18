/*  bookChgStatusNme.c - CGI to change a status name from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 14-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      14-Dec-2021 start by copying bookChgCharNme.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin: * CORS html header
 *      11-Oct-2022 cleanup comments
 *      11-Oct-2022 use EXIT_FAILURE and EXIT_SUCCESS on returns
 *      11-Oct-2022 validate QUERY_STRING for NULL and empty values
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
 *      11-Nov-2022 changes sprintf() to asprintf()
 *  Enhancements:
*/

#define _GNU_SOURCE
#define MAXLEN 1024

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

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
char *sStatus = NULL;
char caStatusName[MAXLEN] = {'\0'};
char *sStatusID = NULL;
int  iStatusID = 0;
char caDelimiter[] = "&";

int main(void) {

    char *strSQL = NULL;

// print the html content type and CORS <head> block -------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"statusID=<99>&statusName=<chgedstatusname>\". Terminating program");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"statusID=<99>&statusName=<chgedstatusname>\". Terminating program");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sStatusID = strtok(sParam, caDelimiter);
    sscanf(sStatusID, "statusID=%d", &iStatusID);

    if (iStatusID == 0) {
        printf("Status ID is 0. Expecting QUERY_STRING=\"statusID=<99>&statusName=<chgedstatusname>\". Terminating program");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sStatus = strtok(NULL, caDelimiter);
    sscanf(sStatus, "statusName=%s", caStatusName);

    if (caStatusName[0] == '\0') {
        printf("Status Name is empty. Expecting QUERY_STRING=\"statusID=<99>&statusName=<chgedstatusname>\". Terminating program");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sStatus = fUrlDecode(caStatusName);
    strcpy(caStatusName, sStatus);
    free(sStatus);

// * initialize the MySQL client library -------------------------------------------------------------------------------

   if (mysql_library_init(0, NULL, NULL)) {
       printf("Cannot initialize MySQL Client library\n");
       return EXIT_FAILURE;
   }

// Initialize a connection and connect to the database -----------------------------------------------------------------

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, sgServer, sgUsername, sgPassword, sgDatabase, 0, NULL, 0))
    {
        printf("\n");
        printf("Failed to connect to MySQL Server %s in module %s()", sgServer, __func__);
        printf("\n\n");
        printf("Error: %s\n", mysql_error(conn));
        printf("\n");
        return EXIT_FAILURE;
    }

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    asprintf(&strSQL, "UPDATE risingfast.`Book Statuses` BS "
                   "SET BS.`Status Name` = '%s' "
                   "WHERE BS.`Status ID` = %d;", caStatusName, iStatusID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Status ID %d updated to '%s'", iStatusID, caStatusName);
    
// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

// free resources used by strSQL ---------------------------------------------------------------------------------------

    free(strSQL);

    return EXIT_SUCCESS;
}
