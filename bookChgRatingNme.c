/*  bookChgRatingNme.c - CGI to change a rating name from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 15-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      15-Dec-2021 start by copying bookChgGenreNme.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin: * html header
 *      10-Oct-2022 clean up comment lines
 *      10-Oct-2022 use EXIT_FAILURE and EXIT_SUCCESS on returns
 *      10-Oct-2022 validate QUERY_STRING as non-NULL and not empty
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
char *sRating = NULL;
char caRatingName[MAXLEN] = {'\0'};
char *sRatingID = NULL;
int  iRatingID = 0;
char caDelimiter[] = "&";

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS <head> block -------------------------------------------------------------------

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
        return  EXIT_FAILURE;
    }

// check for a NULL query string ---------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "ratingID=1&ratingName=Excellent%20Test", 1);                 // uncomment for testing only

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"ratingID=<99>&ratingName=<chngdratingname>\". Terminating bookChgRatingNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty. Expecting QUERY_STRING=\"ratingID=<99>&ratingName=<chngdratingname>\". Terminating bookChgRatingNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character ----------------------------------------------

    sRatingID = strtok(sParam, caDelimiter);
    sscanf(sRatingID, "ratingID=%d", &iRatingID);
    if (iRatingID == 0) {
        printf("Rating ID is 0. Expecting QUERY_STRING=\"ratingID=<99>&ratingName=<chngdratingname>\". Terminating bookChgRatingNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sRating = strtok(NULL, caDelimiter);
    sscanf(sRating, "ratingName=%s", caRatingName);

    if (caRatingName[0] == '\0') {
        printf("Rating Name is empty. Expecting QUERY_STRING=\"ratingID=<99>&ratingName=<chngdratingname>\". Terminating bookChgRatingNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sRating = fUrlDecode(caRatingName);

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Book Ratings` BR "
                   "SET BR.`Rating Name` = '%s' "
                   "WHERE BR.`Rating ID` = %d;", sRating, iRatingID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Rating ID %d updated to '%s'", iRatingID, sRating);

    return EXIT_SUCCESS;
}
