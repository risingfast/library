/*  bookVldtRatingId.c - CGI to validate and retrieve a rating name using a rating id for the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 14-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      14-Dec-2021 started by copying bookVldtRatingId.c and modifying
 *      15-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
 *      13-Oct-2022 clean up comments
 *      13-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      13-Oct-2022 validate query string for NULL and empty values
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
 *      13-Nov-2022 change sprintf() to asprintf()
 *      25-Jan-2023 set freed pointer to NULL
 *      17-Apr-2023 change sgServer from 192.168.0.13 to 192.168.1.64
 *  Enhancements:
*/

#define _GNU_SOURCE                                                                           // required for asprintf()
#define HDG_LEN 1000
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

int  iRatingID = 0;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

void fPrintResult(char *);

int main(void) {

    int i;
    char *strSQL = NULL;

// print the html content type header and CORS header block ------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"ratingID=<99>\". Terminating bookVldtRatingId.cgi");
        printf("\n\n");
        return  EXIT_FAILURE;
    }

// check for an empty (non-NULL) query string --------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty. Expecting QUERY_STRING=\"ratingID=<99>\". Terminating bookVldtRatingId.cgi");
        printf("\n\n");
        return  EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "ratingID=%d", &iRatingID);
    
    if (iRatingID == 0) {
        printf("Rating ID is 0. Expecting QUERY_STRING=\"ratingID=<99>\". Terminating bookVldtRatingId.cgi");
        printf("\n\n");
        return  EXIT_FAILURE;
    }

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

    asprintf(&strSQL, "SELECT BR.`Rating Name` "
                      "FROM risingfast.`Book Ratings` BR "
                      "WHERE BR.`Rating ID` = '%d';", iRatingID);
    
    fPrintResult(strSQL);
    
// free resources used by strSQL ---------------------------------------------------------------------------------------

    free(strSQL);
    strSQL = NULL;

    return EXIT_SUCCESS;
}

// function to print all rows in the result of an SQL query ------------------------------------------------------------

void fPrintResult(char *strSQL)
{
    int iColCount = 0;

    if(mysql_query(conn, strSQL) != 0)
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
        printf("No rating found");
    }

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

    return;
}
