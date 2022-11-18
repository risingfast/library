/*  bookDelGenre.c - CGI to delete a source from the bookDelSource.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 13-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      13-Dec-2021 start by copying bookDelGenre.c
 *      11-Oct-2022 clean up comments
 *      11-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      11-Oct-2022 validate QUERY_STRING for NULL and empty values
 *      19-Oct-2022 add check for no rows deleted
  *     12-Nov-2022 extend MySQL initialization and shutdown operations
 *      12-Nov-2022 change sprintf() to asprintf()
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
int iDelRows = 0;
int  iGenreID = 0;

int main(void) {

    char *strSQL = NULL;

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// * initialize the MySQL client library ------------------------------------------------------------

   if (mysql_library_init(0, NULL, NULL)) {
       printf("Cannot initialize MySQL Client library\n");
       return EXIT_FAILURE;
   }

// Initialize a connection and connect to the database -----------------------------------------------------------------

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, sgServer, sgUsername, sgPassword, sgDatabase, 0, NULL, 0))
    {
        printf("Failed to connect to MySQL Server %s in module %s()", sgServer, __func__);
        printf("\n\n");
        printf("Error: %s\n", mysql_error(conn));
        printf("\n");
        return EXIT_FAILURE;
    }

// check for a NULL query string ---------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "genreID=38", 1);                                             // uncomment for testing only

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"genreID=<99>\". Terminating bookDelGenre.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty query string ---------------------------------------------------------------------------------------

    if(sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"genreID=<99>\". Terminating bookDelGenre.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize the ratingID value --------------------------------------------------

    sscanf(sParam, "genreID=%d", &iGenreID);
    if(iGenreID == 0) {
        printf("Genre ID is 0. Expecting QUERY_STRING=\"genreID=<99>\". Terminating bookDelGenre.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    asprintf(&strSQL, "DELETE FROM risingfast.`Book Genres` "
                   "WHERE `Genre ID` = %d;", iGenreID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    iDelRows = (int) mysql_affected_rows(conn);

    if(iDelRows == 0) {
        printf("No rows deleted. Genre ID not found\n");
    } else {
        printf("Genre ID '%d' deleted", iGenreID);
    }

// * close the database connection created by mysql_init(NULL) --------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library -------------------------------------------------------

    mysql_library_end();

// free resources used by strSQL

    free(strSQL);

    return EXIT_SUCCESS;
}
