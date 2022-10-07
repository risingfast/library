/*  bookAddGenre.c - CGI to add a new genre from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 21-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      21-Nov-2021 started by copying bookAddSource.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin: * HTTP header
 *      06-Pct-2022 check for invalid QUERY_STRING enviroment variable
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

char caGenre[SQL_LEN] = {'\0'};
char *sGenre = NULL;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

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

// check for a NULL query string ---------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "genre=Test%20genre", 1);                                     // uncomment for testing only

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"genre=<newgenre>\". Terminating program");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//    printf("QUERY_STRING: %s", getenv("QUERY_STRING"));                                  // uncomment for testing only
//    printf("\n\n");
//    return 0;

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"genre=<newgenre>\". Terminating program");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "genre=%s", caGenre);
    if (caGenre[0] == '\0') {
        printf("Query string \"%s\" has no genre to add, Expecting QUERY_STRING=\"genre=<newgenre>\". Terminating program", sParam);
        printf("\n\n");
        return EXIT_FAILURE;
    }
    sGenre = fUrlDecode(caGenre);

// set a SQL query to insert the new genre -----------------------------------------------------------------------------

    sprintf(caSQL, "INSERT INTO risingfast.`Book Genres` "
                   "(`Genre Name`)  "
                   "VALUES ('%s');", sGenre);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

//    printf("Query: %s", caSQL);                                                          // uncomment for testing only
//    printf("\n\n");                                                                      // uncomment for testing only
//    return 0;                                                                            // uncomment for testing only


    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Genre '%s' inserted into Genres table", sGenre);

    return EXIT_SUCCESS;
}
