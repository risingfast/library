/*  bookCharacters.c - CGI to retrieve a characters for a single book for the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 10-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      10-Nov-2021 started by copying bookDetails.c and modifying
 *      04-Dec-2021 add the character id to output
 *      26-Dec-2021 add filter to characters
 *      14-Sep-2022 add Access-Control-Allow-Origin: * in http headers
 *      05-Oct-2022 add Add Character ID to filter in SQL query
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

int  iTitleID = 0;
char *sParams = NULL;
char *sSubstring = NULL;
char *sFilter =  NULL;
char caDelimiter[] = "&";
char caFilterTemp[MAXLEN] = {'\0'};
char caFilter[MAXLEN + 2] = {'\0'};

void fPrintResult(char *);

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block -----------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

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

// check for a NULL query string --------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "TitleID=190&Filter=''", 1);

    sParams = getenv("QUERY_STRING");

    if(sParams == NULL) {
        printf("\n");
        printf("Query string is empty. Terminating program");
        printf("\n\n");
        return 1;
    }

// test for an empty QUERY_STRING -------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return 0;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sscanf(sParams, "TitleID=%d", &iTitleID);


//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------$

    sSubstring = strtok(sParams, caDelimiter);
    sscanf(sSubstring, "TitleID=%d", &iTitleID);

    sSubstring = strtok(NULL, caDelimiter);
    sscanf(sSubstring, "Filter=%s", caFilterTemp);

// parse the QUERY_STRING for each argument: Action and Filter ---------------------------------------------------------$

    sprintf(caFilterTemp, "%%%s", fUrlDecode(caFilterTemp));

    if (strlen(caFilterTemp) == 1) {
        sprintf(caFilter, "%s", caFilterTemp);
    } else {
        sprintf(caFilter, "%s%%", caFilterTemp);
    }
    sFilter = caFilter;

// set a SQL query based on a book ID to retrieve all characters-------------------------------------------------------

    sprintf(caSQL, "SELECT BC.`Character ID`, BC.`Character Name` "
                   "FROM risingfast.`Book Characters` BC "
                   "WHERE BC.`Title ID` = '%d' "
                   "AND CONCAT(BC.`Character ID`, BC.`Character Name`) like '%s';", iTitleID, sFilter);

//    printf("\n%s\n", caSQL);

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
    fields = mysql_fetch_fields(res);
    
    mysql_data_seek(res, 0);
    
// print the column headings

    for(int i = 0; i < iColCount; i++)
    {
        if (i - iColCount + 1 == 0) {
            printf("%s", fields[i].name);
        } else {
            printf("%s&", fields[i].name);
        }
    }
    printf("\n");

// print each row of results

    while(row = mysql_fetch_row(res))
    {
        for(int i = 0; i < iColCount; i++)
        {
            if(i == 0) {
                printf("%4s ", row[i]);
            } else if(i - iColCount + 1 == 0)
            {
                printf("%s", row[i]);
                printf("\n");
            } else {
                printf("%s&", row[i]);
            }
        }
    }

    mysql_free_result(res);
}
