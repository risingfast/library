/*  bookChgGenreNme.c - CGI to change a genre name from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 15-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      15-Dec-2021 start by copying bookChgStatusNme.c and modifying
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

// global declarations

char *sgServer = "192.168.0.13";                                                               //mysqlServer IP address
char *sgUsername = "gjarman";                                                              // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                    // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
char *sGenre = NULL;
char caGenreName[MAXLEN] = {'\0'};
char *sGenreID = NULL;
int  iGenreID = 0;
char caDelimiter[] = "&";

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n\n");

// Initialize a connection and connect to the database$$

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

// check for a NULL query string ---------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "genreID=1&genreName=Mystery%20Crime%20Test", 1);          // for testing

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("\n");
        printf("Query string is empty. Terminating program");
        printf("\n\n");
        return 1;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sGenreID = strtok(sParam, caDelimiter);
    sscanf(sGenreID, "genreID=%d", &iGenreID);

    sGenre = strtok(NULL, caDelimiter);
    sscanf(sGenre, "genreName=%s", caGenreName);
    sGenre = fUrlDecode(caGenreName);


// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return 0;
    }

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Book Genres` BG "
                   "SET BG.`Genre Name` = '%s' "
                   "WHERE BG.`Genre ID` = %d;", sGenre, iGenreID);

// Call the function to print the SQL results to stdout and terminate the program

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

    printf("Genre ID %d updated to '%s'", iGenreID, sGenre);

    return EXIT_SUCCESS;
}
