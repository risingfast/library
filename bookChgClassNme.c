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
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
 *      16-Nov-2022 change strcpy() to strncpy()
 *      24-Jan-2023 set freed pointer to NULL
 *      17-Apr-2023 change sgServer from 192.168.0.13 to 192.168.1.64
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

char *sgServer = "192.168.1.64";                                                                //mysqlServer IP address
char *sgUsername = "gjarman";                                                               // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
char *sClass = NULL;
char caClassName[MAXLEN + 1] = {'\0'};
char *sClassID = NULL;
int  iClassID = 0;
char caDelimiter[] = "&";

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS <header> block -----------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

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
    strncpy(caClassName, sClass, MAXLEN);
    free(sClass);
    sClass = NULL;

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

    sprintf(caSQL, "UPDATE risingfast.`Book Classifications` BC "
                   "SET BC.`Classification Name` = '%s' "
                   "WHERE BC.`Classification ID` = %d;", caClassName, iClassID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Classification ID %d updated to '%s'", iClassID, caClassName);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

    return EXIT_SUCCESS;
}
