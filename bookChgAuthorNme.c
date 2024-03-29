/*  bookChgAuthorNme.c - CGI to change a Author Name from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 15-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      15-Dec-2021 start by copying bookChgGenreNme.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
 *      08-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE for returns
 *      10-Oct-2022 clean up comments
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
 *      10-Nov-2022 change sprintf() to asprintf()
 *      16-Nov-2022 change strcpy() to strncpy()
 *      23-Jan-2023 set freed pointers to NULL
 *      17-Apr-2023 change sgServer from 192.168.0.13 to 192.168.1.64
 *  Enhancements:
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
char *sAuthor = NULL;
char caAuthorName[MAXLEN + 1] = {'\0'};
char *sAuthorID = NULL;
int  iAuthorID = 0;
char caDelimiter[] = "&";

int main(void) {

    char *strSQL = NULL;

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "authorID=138&authorName=Molly%20Fritzz", 1);                 // uncomment for testing only

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"authorID=999&authorName=<chgdauthornme>\". Terminating \"bookChgAuthorNme.c\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"authorID=999&authorName=<chgdauthornme>\". Terminating \"bookChgAuthorNme.c\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sAuthorID = strtok(sParam, caDelimiter);
    sscanf(sAuthorID, "authorID=%d", &iAuthorID);

    if (iAuthorID == 0) {
        printf("authorID is NULL. Expecting QUERY_STRING=\"authorID=999&authorName=<chngdauthornme>\". Terminating \"bookChgAuthorNme.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sAuthor = strtok(NULL, caDelimiter);
    sscanf(sAuthor, "authorName=%s", caAuthorName);

    if (caAuthorName == NULL) {
        printf("author name is NULL. Expecting QUERY_STRING=\"authorID=999&authorName=<chngdauthornme>\". Terminating \"bookChgAuthorNme.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sAuthor = fUrlDecode(caAuthorName);
    strncpy(caAuthorName, sAuthor, MAXLEN);
    free(sAuthor);
    sAuthor = NULL;

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

    asprintf(&strSQL, "UPDATE risingfast.`Book Authors` BA "
                   "SET BA.`Author Name` = '%s' "
                   "WHERE BA.`Author ID` = %d;", caAuthorName, iAuthorID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Author ID %d updated to '%s'", iAuthorID, caAuthorName);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

// free resources used by strSQL

    free(strSQL);
    strSQL = NULL;

    return EXIT_SUCCESS;
}
