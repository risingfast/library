/*  bookAddStatus.c - CGI to add a new status from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 21-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      21-Nov-2021 started by copying bookAddSource.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin CCORS http header
 *      07-Oct-2022 add validity checks on QUERY_STRING environment variable
 *      08-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE for returns
 *      09-Oct-2022 clean up comments
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
 *      10-Nov-2022 change sprintf() to asprintf()
 *      16-Nov-2022 change strcpy() to strncpy()
 *      23-Jan-2023 set freed pointers to NULL
 *      17-Apr-2-23 changes sgServer from 102.168.0.13 to 192.168.1.64
 *  Enhancements:
 *      None
*/

#define _GNU_SOURCE                                                                           // required for asprintf()
#define MAXLEN 1024

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

// global declarations -------------------------------------------------------------------------------------------------

char *sgServer = "192.168.1.64";                                                                //mysqlServer IP address
char *sgUsername = "gjarman";                                                               // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char caStatus[MAXLEN + 1] = {'\0'};
char *sStatus = NULL;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char *strSQL = NULL;

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"status=<newstatus>\". Terminating \"bookAddStatus.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"status=<newstatus>>\". Terminating \"bookAddStatus.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "status=%s", caStatus);

    if (caStatus[0] == '\0') {
        printf("Query string \"%s\" has no status to add, Expecting QUERY_STRING=\"status=<newstatus>\". Terminating \"bookAddStatus.cgi\"", sParam);
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sStatus = fUrlDecode(caStatus);
    strncpy(caStatus, sStatus, MAXLEN);
    free(sStatus);
    sStatus = NULL;

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

// set a SQL query to insert the new status ----------------------------------------------------------------------------

    asprintf(&strSQL, "INSERT INTO risingfast.`Book Statuses` "
                   "(`Status Name`)  "
                   "VALUES ('%s');", caStatus);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Status '%s' inserted into Statuses table", caStatus);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

// free resources used by strSQL ---------------------------------------------------------------------------------------

    free(strSQL);
    strSQL = NULL;

    return EXIT_SUCCESS;
}
