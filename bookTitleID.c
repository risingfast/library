/*  bookTitleID.c - CGI to retrieve a book title for the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 23-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      23-Nov-2021 started by copying bookDetails.c and modifying
 *      05-Dec-2021 change name from bookTitle.c to bookTitleID.c
 *      12-Jun-2021 move to gjarman2020.com
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

// global declarations

char *sgServer = "35.188.123.150";                                              // mysqlServer IP address
// char *sgServer = "192.168.0.13";                                             // mysqlServer IP address$
char *sgUsername = "root";                                                      // mysqlSerer logon username$
// char *sgUsername = "gjarman";                                                // mysqlSerer logon username$
char *sgPassword = "Mpa4egu$";                                                  // password to connect to mysqlserver$
char *sgDatabase = "risingfast";                                                // default database name on mysqlserver$

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

int  iTitleID = 0;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

void fPrintResult(char *);

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block -----------------------------------------------------------------------

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

// check for a NULL query string -------------------------------------------------------------------------------------=

//    setenv("QUERY_STRING", "TitleID=9999", 1);

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("\n");
        printf("Query string is empty. Terminating program");
        printf("\n\n");
        return 1;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sscanf(sParam, "TitleID=%d", &iTitleID);

// test if Null or All or non-Null values should be shown ------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
    }
        
    sprintf(caSQL, "SELECT BT.`Title Name` "
                   "FROM risingfast.`Book Titles` BT "
                   "WHERE BT.`Title ID` = '%d';", iTitleID);
    
    fPrintResult(caSQL);
    
    return 0;
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

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return;
    }
    
// fetch the number of fields in the result
    
    iColCount = mysql_num_fields(res);
    
    mysql_data_seek(res, 0);
    
// print each row of results

    if(row = mysql_fetch_row(res))
    {
        printf("%s", row[0]);
    } else {
        printf("No title found");
    }

    mysql_free_result(res);
}
