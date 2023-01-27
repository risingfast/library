/*  bookFetchLOVs.c - CGI to fetch lists of values for book attributes
 *  Author: Geoffrey Jarman
 *  Started: 04-Jan-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      04-Jan-2022 copied from bookInquiry2.c
 *      08-May-2022 sort results in alpha order
 *      09-May-2022 fix sort by alpha for authors
 *      15-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
 *      12-Oct-2022 clean up comments
 *      12-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      12-Oct-2022 validate QUERY_STRING for NULL and empty values
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
 *      11-Nov-2022 change sprintf() to sprintf()
 *      25-Jan-2023 set freed pointers to NULL
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

char *sgServer = "192.168.0.13";                                                                //mysqlServer IP address
char *sgUsername = "gjarman";                                                               // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

char caText[MAXLEN] = {'\0'};
char caTopic[MAXLEN] = {'\0'};
char *sTopic = NULL;
char caFilterTemp[MAXLEN] = {'\0'};
char caFilter[MAXLEN + 2] = {'\0'};
char *sFilter = NULL;
char *sParams = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";
char *sTemp = NULL;

void fPrintResult(char *, char *, char *);

int main(void) {

    int i;
    char caOrder[] = {'A', 'S', 'C', '\0'};
    char *strSQL = NULL;

// print the html content type and CORS <header> block -----------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

    sParams = getenv("QUERY_STRING");

// check for a NULL query string ---------------------------------------------------------------------------------------

    if(sParams == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"topic=<topicname>&filter=<filterstring>\". Terminating bookFetchLOVs.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty (non-NULL) query string --------------------------------------------------------------------------

    if(sParams[0] == '\0') {
        printf("Query string is empty. Expecting QUERY_STRING=\"topic=<topicname>&filter=<filterstring>\". Terminating bookFetchLOVs.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sSubstring = strtok(sParams, caDelimiter);
    sscanf(sSubstring, "topic=%s", caTopic);
    if(caTopic[0] == '\0') {
        printf("Topic is empty. Expecting QUERY_STRING=\"topic=<topicname>&filter=<filterstring>\". Terminating bookFetchLOVs.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }
    sTopic = caTopic;

// parse the QUERY_STRING for the filter element and add the enclosing '%' wildcards -----------------------------------

    sSubstring = strtok(NULL, caDelimiter);
    sscanf(sSubstring, "filter=%s", caFilterTemp);

    sTemp = fUrlDecode(caFilterTemp);                                    // remove URL encoding from the filter contents
    sprintf(caFilterTemp, "%%%s", sTemp);                                     // prefix the filter with the '%' wildcard
    free(sTemp);
    sTemp = NULL;

    if (strlen(caFilterTemp) == 1) {              //add a '%' suffix bu only if there are other characters in the filter
        sprintf(caFilter, "%s", caFilterTemp);
    } else {
        sprintf(caFilter, "%s%%", caFilterTemp);
    }
    sFilter = caFilter;

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

// define and execute a query based on the topic -----------------------------------------------------------------------

    if (strstr(getenv("QUERY_STRING"), "titles") != NULL) {
        asprintf(&strSQL, "SELECT BT.`Title ID` as 'ID' "
                       ", BT.`Title Name` as 'Name' "
                       ", BT.Start"
                       ", BT.Finish"
                       ", BT.Comments "
                       "FROM risingfast.`Book Titles` BT "
                       "WHERE BT.`Title Name` LIKE '%s' "
                       "ORDER BY BT.`Title Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "characters") != NULL) {
        asprintf(&strSQL, "SELECT BC.`Character ID` as 'ID' "
                       ", BC.`Character Name` as 'Name' "
                       "FROM risingfast.`Book Characters` BC "
                       "WHERE BC.`Character Name` LIKE '%s' "
                       "ORDER BY BC.`Character Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "authors") != NULL) {
        asprintf(&strSQL, "SELECT BA.`Author ID` as 'ID' "
                       ", BA.`Author Name` as 'Name' "
                       ", ROUND(SUM(BR.`Rating Value`) / COUNT(BA.`Author Name`), 0) as `Author Rating` "
                       "FROM `Book Authors` BA "
                       "LEFT JOIN risingfast.`Book Titles` BT ON BA.`Author ID` = BT.`Author ID` "
                       "LEFT JOIN risingfast.`Book Ratings` BR ON BT.`Rating ID` = BR.`Rating ID` "
                       "WHERE BA.`Author Name` LIKE '%s' "
                       "GROUP BY BA.`Author Name`, BA.`Author ID` "
                       "ORDER BY BA.`Author Name` ASC; ", sFilter)
                       ;
         fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "recents") != NULL) {
        asprintf(&strSQL, "SELECT BT.`Title ID` as 'ID' "
                       ", BT.Start"
                       ", BT.`Title Name` as 'Name' "
                       ", BT.Finish"
                       ", BT.Comments "
                       "FROM risingfast.`Book Titles` BT "
                       "WHERE BT.`Title Name` LIKE '%s' "
                       "AND BT.`Start` IS NOT NULL "
                       "ORDER BY BT.`Start` DESC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "unreads") != NULL) {
        asprintf(&strSQL, "SELECT BT.`Title ID` as 'ID' "
                       ", BT.Start"
                       ", BT.`Title Name` as 'Name' "
                       ", BT.Finish"
                       ", BT.Comments "
                       "FROM risingfast.`Book Titles` BT "
                       "WHERE BT.`Title Name` LIKE '%s' "
                       "AND BT.`Start` IS NULL "
                       "ORDER BY BT.`Start` DESC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "classifications") != NULL) {
        asprintf(&strSQL, "SELECT BC.`Classification ID` as 'ID' "
                       ", BC.`Classification Name` as 'Name' "
                       "FROM risingfast.`Book Classifications` BC "
                       "WHERE BC.`Classification Name` LIKE '%s' "
                       "ORDER BY BC.`Classification Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "ratings") != NULL) {
        asprintf(&strSQL, "SELECT BR.`Rating ID` as 'ID' "
                       ", BR.`Rating Name` as 'Name' "
                       "FROM risingfast.`Book Ratings` BR "
                       "WHERE BR.`Rating Name` LIKE '%s' "
                       "ORDER BY BR.`Rating Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "series") != NULL) {
        asprintf(&strSQL, "SELECT BS.`Series ID` as 'ID' "
                       " , BS.`Series Name` as 'Name' "
                       " FROM risingfast.`Book Series` BS "
                       " WHERE BS.`Series Name` LIKE '%s' "
                       " ORDER BY BS.`Series Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "sources") != NULL) {
        asprintf(&strSQL, "SELECT BS.`Source ID` as 'ID' "
                       ", BS.`Source Name` as 'Name' "
                       "FROM risingfast.`Book Sources` BS "
                       "WHERE BS.`Source Name` LIKE '%s' "
                       "ORDER BY BS.`Source Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "genres") != NULL) {
        asprintf(&strSQL, "SELECT BG.`Genre ID` as 'ID' "
                       ", BG.`Genre Name` as 'Name' "
                       "FROM risingfast.`Book Genres` BG "
                       "WHERE BG.`Genre Name` LIKE '%s' "
                       "ORDER BY BG.`Genre Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "statuses") != NULL) {
        asprintf(&strSQL, "SELECT BS.`Status ID` as 'ID' "
                       ", BS.`Status Name` as 'Name' "
                       "FROM risingfast.`Book Statuses` BS "
                       "WHERE BS.`Status Name` LIKE '%s' "
                       "ORDER BY BS.`Status Name` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, strSQL);
    }

// free resources used by strSQL ---------------------------------------------------------------------------------------

    free(strSQL);
    sTemp = NULL;

    return EXIT_SUCCESS;
}

void fPrintResult(char *caTopic, char *caFilter, char *caSQL)
{
    int iColCount = 0;

// execute the query and check for no result ---------------------------------------------------------------------------
    
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

     while(row = mysql_fetch_row(res))
    {
        for (int i = 0; i < iColCount; i++)
        {
            if(i == 0)
            {
                printf("%4s,", row[i]);
            }
            else if (i == iColCount - 1)
            {
                if (row[i] != NULL) {
                    printf("%s", row[i]);
                } else {
                    ;
                }
            }
            else {
                if (row[i] != NULL) {
                    printf("%s,", row[i]);
                } else {
                    ;
                }
            }
        }
        printf("\n");
    }

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

return;
}
