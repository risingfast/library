/*  bookCharacters.c - CGI to retrieve characters for a single book for the bookInquiry.html webpage
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
 *      09-Oct-2022 clean up comments
 *      10-Nov-2022 changes sprintf() to asprintf()
 *      23-Jan-2023 set freed pointer to NULL
 *      17-Apr-2023 change sgServer from 192.168.0.13 to 192.168.1.64
 *  Enhancements:
*/

#define _GNU_SOURCE                                                                           // required for asprintf()
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

int  iTitleID = 0;
char *sParams = NULL;
char *sSubstring = NULL;
char *sFilter =  NULL;
char caDelimiter[] = "&";
char caFilterTemp[MAXLEN] = {'\0'};
char caFilter[MAXLEN + 2] = {'\0'};
char *strTemp = NULL;

void fPrintResult(char *);

int main(void) {

    int i;
    char *strSQL = NULL;

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// * initialize the MySQL client library -------------------------------------------------------------------------------

   if (mysql_library_init(0, NULL, NULL)) {
       printf("Cannot initialize MySQL Client library\n");
       return EXIT_FAILURE;
   }

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

//    setenv("QUERY_STRING", "TitleID=190&Filter=''", 1);                                  // uncomment for testing only

    sParams = getenv("QUERY_STRING");

    if(sParams == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"TitleID=9999&Filter=<filterval>\". Terminating program");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParams[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"TitleID=9999&Filter=<filterval>\". Terminating program");
        printf("\n\n");
        return EXIT_SUCCESS;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParams, "TitleID=%d", &iTitleID);

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sSubstring = strtok(sParams, caDelimiter);
    sscanf(sSubstring, "TitleID=%d", &iTitleID);

    if (iTitleID == 0) {
        printf("Title ID is null. Expecting QUERY_STRING=\"TitleID=9999&Filter=<filterval>\". Terminating program");
        printf("\n\n");
        return EXIT_SUCCESS;
    }

    sSubstring = strtok(NULL, caDelimiter);
    sscanf(sSubstring, "Filter=%s", caFilterTemp);

// parse the QUERY_STRING for each argument: Action and Filter ---------------------------------------------------------

    strTemp = fUrlDecode(caFilterTemp);
    sprintf(caFilterTemp, "%%%s", strTemp);
    free(strTemp);
    strTemp = NULL;

    if (strlen(caFilterTemp) == 1) {
        sprintf(caFilter, "%s", caFilterTemp);
    } else {
        sprintf(caFilter, "%s%%", caFilterTemp);
    }
    sFilter = caFilter;

// set a SQL query based on a book ID to retrieve all characters--------------------------------------------------------

    asprintf(&strSQL, "SELECT BC.`Character ID`, BC.`Character Name` "
                   "FROM risingfast.`Book Characters` BC "
                   "WHERE BC.`Title ID` = '%d' "
                   "AND CONCAT(BC.`Character ID`, BC.`Character Name`) like '%s';", iTitleID, sFilter);

    fPrintResult(strSQL);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

// free resources used by strSQL ----------------------------------------------------------------------------------------

    free(strSQL);
    strSQL = NULL;

    return EXIT_SUCCESS;
}

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

    return;
}
