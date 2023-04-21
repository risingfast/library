/*  bookChgGenreNme.c - CGI to change a genre name from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 15-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      15-Dec-2021 start by copying bookChgStatusNme.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin: * in html headers
 *      10-Oct-2022 cleanup comment lines
 *      10-Oct-2022 use EXIT_FAILURE and EXIT_SUCCESS on returns
 *      21-Oct-2022 extend MySQL initialization and shutdown operations
 *      11-Nov-2022 change sprintf() to asprintf()
 *      16-Nov-2022 change strcpy() to strcpy()
 *      24-Jan-2023 set freed pointers to NULL
 *      17-Apr-2023 change sgServer from 192.168.0.13 o 192.169.1.64
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

char *sParam = NULL;
char *sGenre = NULL;
char caGenreName[MAXLEN + 1] = {'\0'};
char *sGenreID = NULL;
int  iGenreID = 0;
char caDelimiter[] = "&";

int main(void) {

    char *strSQL = NULL;

// print the html content type and CORS <head> block -------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"genreID=<99>&genreName=<chngdGenreName>\". Terminating bookChgGenreNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"genreID=<99>&genreName=<chngdGenreName>\". Terminating bookChgGenreNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sGenreID = strtok(sParam, caDelimiter);
    sscanf(sGenreID, "genreID=%d", &iGenreID);

    if (iGenreID == 0) {
        printf("Genre ID is 0. Expecting QUERY_STRING=\"genreID=<99>&genreName=<chngdGenreName>\". Terminating bookChgGenreNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sGenre = strtok(NULL, caDelimiter);
    sscanf(sGenre, "genreName=%s", caGenreName);

    if (caGenreName[0] == '\0') {
        printf("Genre Name is empty. Expecting QUERY_STRING=\"genreID=<99>&genreName=<chngdGenreName>\". Terminating bookChgGenreNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sGenre = fUrlDecode(caGenreName);
    strncpy(caGenreName, sGenre, MAXLEN);
    free(sGenre);
    sGenre = NULL;

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

    asprintf(&strSQL, "UPDATE risingfast.`Book Genres` BG "
                   "SET BG.`Genre Name` = '%s' "
                   "WHERE BG.`Genre ID` = %d;", caGenreName, iGenreID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Genre ID %d updated to '%s'", iGenreID, caGenreName);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

// free resources used by strSQL ---------------------------------------------------------------------------------------

    free(strSQL);
    strSQL = NULL;

    return EXIT_SUCCESS;
}
