/*  bookDetails2.c - CGI to retrieve a single book for the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 08-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      08-Nov-2021 started by copying bookInquiry.c and modifying
 *      09-Nov-2021 create heading row above data row
 *      26-Jul-2022 add abstract field to the book query
 *      12-Oct-2022 clean up comments
 *      12-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      12-Oct-2022 validate qUERY_STRING for NULL or empty values
 *      12-Oct-2022 remove test comments
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

int  iTitleID = 0;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

void fPrintResult(char *);

int main(void) {

    int i;
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

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"TitleID=<999>\". Terminating bookDetails2.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test if Null or All or non-Null values should be shown --------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"TitleID=<999>\". Terminating bookDetails2.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "TitleID=%d", &iTitleID);

    sprintf(caSQL, "SELECT BT.`Title ID` "
                   ", BT.`Title Name` "
                   ", BA.`Author ID` "
                   ", BA.`Author Name` "
                   ", BS1.`Source ID` "
                   ", BS1.`Source Name` "
                   ", BS2.`Series ID` "
                   ", BS2.`Series Name` "
                   ", BS3.`Status ID` "
                   ", BS3.`Status Name` "
                   ", BG.`Genre ID` "
                   ", BG.`Genre Name` "
                   ", BC.`Classification ID` "
                   ", BC.`Classification Name` "
                   ", BR.`Rating ID` "
                   ", BR.`Rating Name` "
                   ", BT.`Start` "
                   ", BT.`Finish` "
                   ", BT.`Comments` "
                   ", BT.`Abstract` "
                   "FROM risingfast.`Book Titles` BT "
                   "LEFT JOIN risingfast.`Book Authors` BA ON BT.`Author ID` = BA.`Author ID` "
                   "LEFT JOIN risingfast.`Book Sources` BS1 ON BT.`Source ID` = BS1.`Source ID` "
                   "LEFT JOIN risingfast.`Book Series` BS2 ON BT.`Series ID` = BS2.`Series ID` "
                   "LEFT JOIN risingfast.`Book Statuses` BS3 ON BT.`Status ID` = BS3.`Status ID` "
                   "LEFT JOIN risingfast.`Book Genres` BG ON BT.`Genre ID` = BG.`Genre ID` "
                   "LEFT JOIN risingfast.`Book Classifications` BC ON BT.`Classification ID` = BC.`Classification ID` "
                   "LEFT JOIN risingfast.`Book Ratings` BR ON BT.`Rating ID` = BR.`Rating ID` "
                   "WHERE BT.`Title ID` = '%d';", iTitleID);

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
    fields = mysql_fetch_fields(res);

    mysql_data_seek(res, 0);

// print the column headings -------------------------------------------------------------------------------------------

    for(int i = 0; i < iColCount; i++)
    {
        if (i - iColCount + 1 == 0) {
            printf("%s", fields[i].name);
        } else {
            printf("%s&", fields[i].name);
        }
    }
    printf("\n");

// print each row of results -------------------------------------------------------------------------------------------

    while(row = mysql_fetch_row(res))
    {
        for(int i = 0; i < iColCount; i++)
        {
            if(i - iColCount + 1 == 0)
            {
                printf("%s", row[i]);
                printf("\n");
            } else {
                printf("%s&", row[i]);
            }
        }
    }

    mysql_free_result(res);
    return;
}
