/*  bookDelClassification.c - CGI to delete a character from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 08-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      08-Dec-2021 start by copying bookDelCharacter.c
 *      13-Dec-2021 check if no classification deleted
 *      15-Sep-2021 add Acces-Control-Allow-Header: * CORS html header
 *      11-Oct-2022 clean up comments
 *      11-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      11-Oct-2022 validate QUERY_STRING is not NULL or empty
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
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
int  iClassID = 0;
int  iDelRows = 0;

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS <header> block -----------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL (non-empty). Expecting QUERY_STRING=\"ClassID=<99>\". Terminating bookDelClassification.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty (non-NULL) query string --------------------------------------------------------------------------

    if(sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"ClassID=<99>\". Terminating bookDelClassification.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize the CharID value ----------------------------------------------------

    sscanf(sParam, "ClassID=%d", &iClassID);

    if(iClassID == '\0') {
        printf("Class ID is 0. Expecting QUERY_STRING=\"ClassID=<99>\". Terminating bookDelClassification.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

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
        return  EXIT_FAILURE;
    }

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "DELETE FROM risingfast.`Book Classifications` "
                   "WHERE `Classification ID` = %d;", iClassID);

// Call the function to execute the query ------------------------------------------------------------------------------

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// Check for a count of deleted rows -----------------------------------------------------------------------------------

    iDelRows = (int) mysql_affected_rows(conn);

    if(iDelRows == 0) {
        printf("No rows deleted. Classification ID not found\n");
    } else {
        printf("Classification ID '%d' deleted", iClassID);
    }

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

    return EXIT_SUCCESS;
}
