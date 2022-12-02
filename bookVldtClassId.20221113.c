/*  bookVldtClassId.c - CGI to validate and retrieve a class name using a class id for the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 18-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      18-Dec-2021 started by copying bookVldtRatingId.c and modifying
 *      15-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
 *      13-Oct-2022 clean up comments
 *      13-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      13-Oct-2022 validate QUERY_STRING for NULL and empty values
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
#define HDG_LEN 1000

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

int  iClassID = 0;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

void fPrintResult(char *);

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type header and CORS header block ------------------------------------------------------------

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

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"classID=<99>\". Terminating bookVldtClassId.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty query string -------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty. Expecting QUERY_STRING=\"classID=<99>\". Terminating bookVldtClassId.cgi");
        printf("\n\n");
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "classID=%d", &iClassID);
    if (iClassID == 0) {
        printf("Class ID is 0. Expecting QUERY_STRING=\"classID=<99>\". Terminating bookVldtClassId.cgi");
        printf("\n\n");
    }

    sprintf(caSQL, "SELECT BC.`Classification Name` "
                   "FROM risingfast.`Book Classifications` BC "
                   "WHERE BC.`Classification ID` = '%d';", iClassID);
    
    fPrintResult(caSQL);
    
    return EXIT_SUCCESS;
}

void fPrintResult(char *caSQL)
{
    int iColCount = 0;

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return;
    }

// store the result of the query ---------------------------------------------------------------------------------------

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return;
    }
    
// fetch the number of fields in the result ----------------------------------------------------------------------------
    
    iColCount = mysql_num_fields(res);
    
    mysql_data_seek(res, 0);
    
// print each row of results -------------------------------------------------------------------------------------------

    if(row = mysql_fetch_row(res))
    {
        printf("%s", row[0]);
    } else {
        printf("No classification found");
    }

    mysql_free_result(res);
    return;
}