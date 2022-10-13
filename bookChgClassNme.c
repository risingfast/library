/*  bookChgClassNme.c - CGI to change a classification name from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 15-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      15-Dec-2021 start by copying bookChgRatingNme.c and modifying
 *      14-Sep-2022 add Acess-Control-Allow-Origin: * CORS header
 *      10-Oct-2022 cleanup comments
 *      10-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE for returns
 *      10-Oct-2022 validate QUERY_STRING and test for NULL and empty strings
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
char *sClass = NULL;
char caClassName[MAXLEN] = {'\0'};
char *sClassID = NULL;
int  iClassID = 0;
char caDelimiter[] = "&";

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS <header> block -----------------------------------------------------------------

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

//    setenv("QUERY_STRING", "classID=1&className=Fiction%20Test", 1);                     // uncomment for testing only

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"classID=<99>&className=<chngdclassname\". Terminating bookChgClassNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty. Expecting QUERY_STRING=\"classID=<99>&className=<chngdclassname\". Terminating bookChgClassNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sClassID = strtok(sParam, caDelimiter);
    sscanf(sClassID, "classID=%d", &iClassID);
    if (iClassID == 0) {
        printf("Class ID is 0. Expecting QUERY_STRING=\"classID=<99>&className=<chngdclassname\". Terminating bookChgClassNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sClass = strtok(NULL, caDelimiter);
    sscanf(sClass, "className=%s", caClassName);
    if (caClassName[0] == '\0') {
        printf("Class Name is empty. Expecting QUERY_STRING=\"classID=<99>&className=<chngdclassname\". Terminating bookChgClassNme.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sClass = fUrlDecode(caClassName);

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Book Classifications` BC "
                   "SET BC.`Classification Name` = '%s' "
                   "WHERE BC.`Classification ID` = %d;", sClass, iClassID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

    printf("Classification ID %d updated to '%s'", iClassID, sClass);

    return EXIT_SUCCESS;
}
