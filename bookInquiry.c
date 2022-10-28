/*  bookInquiry.c - CGI to show books on the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 01-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      01-Nov-2021
 *      02-Nov-2021 add SQL for titles
 *      02-Nov-2021 add SQL for characters
 *      02-Nov-2021 add SQL for authors
 *      03-Nov-2021 add SQL for recent, classifications, ratings, series, sources, genres and statuses
 *      15-Nov-2021 remove unread books from recents listing
 *      15-Nov-2021 add unreads listing
 *      19-Dec-2021 add author to series listing
 *      22-Mar-2022 remove author sort from author listing sql
 *      25-Jul-2022 add author name to title query
 *      25-Jul-2022 add author name to title query filter
 *      03-Aug-2022 add author rating to author query filter
 *      03-Aug-2022 add series rating to series query filter
 *      08-Aug-2022 add author rating to unreads list
 *      08-Aug-2022 add author to recents and include all fields in filter
 *      08-Aug-2022 add spaces between fields in series list
 *      08-Aug-2022 add spaces between fields in titles list
 *      09-Aug-2022 add spaces between fields in other lists: unreads, ratings, sources, genres, and statuses
 *      14-Sep-2022 add Access-Control-Allow-Origin HTTP header
 *      05-Oct-2022 coalesce a null author rating with '' to return authors without ratings
 *      05-Oct-2022 add ID field to each query filter target2
 *      09-Oct-2022 add COALESCE to series query to handle NULL's in filter
 *      09-Oct-2022 clean up comments
 *      09-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      20-Oct-2022 extend MySQL initialization and shutdown operations
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
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParams = getenv("QUERY_STRING");

// test for a NULL parameter string in QUERY_STRING --------------------------------------------------------------------

    if (sParams == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"topic=<querytopic>&filter=<queryfilter>\". Terminating \"bookInquiry.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty (non-NULL) parameter string in QUERY_STRING -------------------------------------------------------

    if (sParams[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"topic=<querytopic>&filter=<queryfilter>\". Terminating \"bookInquiry.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character for the first element: topic -----------------

    sSubstring = strtok(sParams, caDelimiter);
    sscanf(sSubstring, "topic=%s", caTopic);
    if (caTopic[0] == '\0') {
        printf("Topic string is empty (non-NULL). Expecting QUERY_STRING=\"topic=<querytopic>&filter=<queryfilter>\". Terminating \"bookInquiry.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }
    sTopic = caTopic;

// parse the QUERY_STRING for the filter element and prefix and add the enclosing '%' wildcards ------------------------

    sSubstring = strtok(NULL, caDelimiter);
    sscanf(sSubstring, "filter=%s", caFilterTemp);

    sTemp = fUrlDecode(caFilterTemp);                                    // Remove URL encoding from the filter contents
    sprintf(caFilterTemp, "%%%s", sTemp);                                     // Prefix the filter with the '%' wildcard
    free(sTemp);

    if (strlen(caFilterTemp) == 1) {            // add a '%' suffix but only if there are other characters in the filter
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

// set the SQL query based on the topic passed in QUER_STRING ----------------------------------------------------------

    if (strstr(getenv("QUERY_STRING"), "titles") != NULL) {
        sprintf(caSQL, "SELECT BT.`Title ID` as 'ID' "
                       ", CONCAT(' ', BT.`Title Name`) as 'Name' "
                       ", CONCAT(' ', BA.`Author Name`) as 'Author' "
                       ", CONCAT(' ', BT.Start) "
                       ", CONCAT(' ', BT.Finish)"
                       ", CONCAT(' ', BT.Comments) "
                       "FROM risingfast.`Book Titles` BT "
                       "LEFT OUTER JOIN risingfast.`Book Authors` BA on BT.`Author ID` = BA.`Author ID` "
                       "WHERE CONCAT(BT.`Title ID`, BT.`Title Name`, BA.`Author Name`, IFNULL(BT.Comments, ' ')) LIKE '%s' "
                       "ORDER BY BT.`Title ID` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "characters") != NULL) {
        sprintf(caSQL, "SELECT BC.`Character ID` as 'ID' "
                       ", BC.`Character Name` as 'Name' "
                       "FROM risingfast.`Book Characters` BC "
                       "WHERE CONCAT(BC.`Character ID`, BC.`Character Name`) LIKE '%s' "
                       "ORDER BY BC.`Character ID` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "authors") != NULL) {

        sprintf(caSQL, "SELECT T.ID, T.`Author Name`, T.`Author Rating` FROM (SELECT BA.`Author ID` as `ID` "
                       ", BA.`Author Name` as `Author Name` "
                       ", ROUND(SUM(BR.`Rating Value`) / COUNT(BT.`Author ID`), 0) as `Author Rating` "
                       "FROM `Book Authors` BA "
                       "LEFT JOIN risingfast.`Book Titles` BT ON BA.`Author ID` = BT.`Author ID` "
                       "LEFT JOIN risingfast.`Book Ratings` BR ON BT.`Rating ID` = BR.`Rating ID` "
                       "WHERE risingfast.BA.`Author Name` != 'Unassigned' "
                       "GROUP BY BA.`Author ID` "
                       "ORDER BY BA.`Author ID` ASC) AS T "
                       "WHERE CONCAT(T.ID, COALESCE(T.`Author Rating`, ''), T.`Author Name`) like '%s'; ", sFilter)
                       ;

         fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "recents") != NULL) {
        sprintf(caSQL, "SELECT BT.`Title ID` as 'ID' "
                       ", CONCAT(' ', BT.Start) "
                       ", CONCAT(' ', BT.`Title Name`) "
                       ", CONCAT(' ', BA.`Author Name`) "
                       ", CONCAT(' ', BT.Finish) "
                       ", CONCAT(' ', BT.Comments) "
                       "FROM risingfast.`Book Titles` BT "
                       "LEFT JOIN risingfast.`Book Authors` BA on BT.`Author ID` = BA.`Author ID` "
                       "WHERE CONCAT(BT.`Title ID`, BT.`Start`, BT.`Title Name`, BA.`Author Name`) LIKE '%s' "
                       "AND BT.`Start` IS NOT NULL "
                       "ORDER BY BT.`Start` DESC", sFilter)
        ;

        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "unreads") != NULL) {
        sprintf(caSQL, "SELECT BT.`Title ID` as 'ID' "
                       ", CONCAT(' ', BT.`Title Name`) as 'Name' "
                       ", CONCAT(' ', BA.`Author Name`) as 'Author' "
                       ", CONCAT(' ', COALESCE(AR.`Author Rating`, 0)) as 'Rating' "
                       " FROM risingfast.`Book Titles` BT "
                       " LEFT JOIN (SELECT BT1.`Author ID`, ROUND(AVG(BR1.`Rating Value`), 0) AS 'Author Rating' from risingfast.`Book Titles` BT1 "
                       " LEFT JOIN risingfast.`Book Ratings` BR1 on BT1.`Rating ID` = BR1.`Rating ID` "
                       " WHERE BT1.`Status ID` = 4 GROUP BY `Author ID`) AR on BT.`Author ID` = AR.`Author ID` "
                       " LEFT JOIN risingfast.`Book Authors` BA on BT.`Author ID` = BA.`Author ID` "
                       " WHERE CONCAT(BT.`Title ID`, BT.`Title Name`, BA.`Author Name`, COALESCE(AR.`Author Rating`, 0)) LIKE '%s' "
                       " AND BT.`Start` IS NULL "
                       " ORDER BY BT.`Title ID` ASC", sFilter)
        ; 

        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "classifications") != NULL) {
        sprintf(caSQL, "SELECT BC.`Classification ID` as 'ID' "
                       ", CONCAT(' ', BC.`Classification Name`) as 'Name' "
                       "FROM risingfast.`Book Classifications` BC "
                       "WHERE CONCAT(BC.`Classification ID`, BC.`Classification Name`) LIKE '%s' "
                       "ORDER BY BC.`Classification ID` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "ratings") != NULL) {
        sprintf(caSQL, "SELECT BR.`Rating ID` as 'ID' "
                       ", CONCAT(' ', BR.`Rating Name`) as 'Name' "
                       "FROM risingfast.`Book Ratings` BR "
                       "WHERE CONCAT(BR.`Rating ID`, BR.`Rating Name`) LIKE '%s' "
                       "ORDER BY BR.`Rating ID` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "series") != NULL) {
        sprintf(caSQL, "SELECT T.`Series ID`, CONCAT(' ', T.`Series Name`), CONCAT(' ', T.`Author Name`), CONCAT(' ', T.`Rating`), CONCAT(' ', T.`Count`) "
                       " FROM (SELECT BS.`Series ID` as 'Series ID' "
                       " , BS.`Series Name` as 'Series Name' "
                       " , BA.`Author Name` as 'Author Name' "
                       " , TRUNCATE(SUM(BR.`Rating Value`)/COUNT(BR.`Rating Value`), 0) as 'Rating' "
                       " , COUNT(BR.`Rating Value`) as 'Count' "
                       " FROM risingfast.`Book Series` BS "
                       " LEFT JOIN risingfast.`Book Titles` BT on BS.`Series ID` = BT.`Series ID` "
                       " LEFT JOIN risingfast.`Book Authors` BA on BT.`Author ID` = BA.`Author ID` "
                       " LEFT JOIN risingfast.`Book Ratings` BR on BT.`Rating ID` = BR.`Rating ID` "
                       " GROUP BY BS.`Series ID`, BS.`Series Name`, BA.`Author Name` "
                       " ORDER BY BS.`Series ID` ASC) AS T "
                       " WHERE CONCAT(T.`Series ID`, T.`Series Name`, COALESCE(T.`Author Name`, ''), COALESCE(T.`Rating`, ''), COALESCE(T.`Count`, '')) like '%s'; ", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "sources") != NULL) {
        sprintf(caSQL, "SELECT BS.`Source ID` as 'ID' "
                       ", CONCAT(' ', BS.`Source Name`) as 'Name' "
                       "FROM risingfast.`Book Sources` BS "
                       "WHERE CONCAT(BS.`Source ID`, BS.`Source Name`) LIKE '%s' "
                       "ORDER BY BS.`Source ID` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "genres") != NULL) {
        sprintf(caSQL, "SELECT BG.`Genre ID` as 'ID' "
                       ", CONCAT(' ', BG.`Genre Name`) as 'Name' "
                       "FROM risingfast.`Book Genres` BG "
                       "WHERE CONCAT(BG.`Genre ID`, BG.`Genre Name`) LIKE '%s' "
                       "ORDER BY BG.`Genre ID` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
    else if (strstr(getenv("QUERY_STRING"), "statuses") != NULL) {
        sprintf(caSQL, "SELECT BS.`Status ID` as 'ID' "
                       ", CONCAT(' ', BS.`Status Name`) as 'Name' "
                       "FROM risingfast.`Book Statuses` BS "
                       "WHERE CONCAT(BS.`Status ID`, BS.`Status Name`) LIKE '%s' "
                       "ORDER BY BS.`Status ID` ASC", sFilter)
        ;
        fPrintResult(sTopic, sFilter, caSQL);
    }
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
        for(int i = 0; i < iColCount; i++)
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
