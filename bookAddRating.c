/*  bookAddRating.c - CGI to add a new rating from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 20-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      19-Nov-2021 started by copying bookAddClassification.c and modifying
 *      14-Sep-2022 added Access-Control-Allow-Headers: *
 *      07-Oct-2022 test for invalid QUERY_STRING environment variables
 *      08-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      09-Oct-2022 clean up comments
 *      19-Oct-2022 extend MySQL initialization and shutdown operations
 *      15-Nov-2022 change strcpy() to strncpy()
 *      23-Jan-2023 set freed pointer to NULL
 *      16-Apr-2023 change sgServer from 192.168.0.13 to 192.168.1.64
 *  Enhancements:
 *      Add rating value to update
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

char *sgServer = "192.168.1.64";                                                                //mysqlServer IP address
char *sgUsername = "gjarman";                                                               // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char caRating[MAXLEN] = {'\0'};
char *sRating = NULL;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS headers ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"rating=<newrating>\". Terminating \"bookAddRating.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty (non-NULL) QUERY_STRING ---------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"rating=<newrating>\". Terminating \"bookAddRating.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "rating=%s", caRating);

    if (caRating[0] == '\0') {
        printf("Query string \"%s\" has no rating to add, Expecting QUERY_STRING=\"rating=<newrating>\". Terminating \"bookAddRating.cgi\"", sParam);
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sRating = fUrlDecode(caRating);
    strncpy(caRating, sRating, MAXLEN);
    free(sRating);
    sRating = NULL;

// * initialize the MySQL client library

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
// set a SQL query to insert the new rating ----------------------------------------------------------------------------

    sprintf(caSQL, "INSERT INTO risingfast.`Book Ratings` "
                   "(`Rating Name`, `Rating Value`)  "
                   "VALUES ('%s', 0);", caRating);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Rating '%s' inserted into Ratings table", caRating);

// * close the database connection created by mysql_init(NULL) -----------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library ----------------------------------------------------------------------------

    mysql_library_end();

    return EXIT_SUCCESS;
}
