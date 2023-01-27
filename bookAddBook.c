/*  bookAddBook.c - CGI to add a new book and attributes from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 07-Jan-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *      https://stackoverflow.com/questions/17112852/get-the-new-record-primary-key-id-from-mysql-insert-query
 *  Log:
 *      07-Jan-2022 start by copying bookAddBook.c and modifying
 *      08-Jan-2022 add print for new title ID
 *      10-Jan-2022 add NULL handling for start date, finish date and comments
 *      29-Jul-2022 add abstract field
 *      14-Sep-2022 add Access-Control-Allow-Origin HTTP header
 *      21-Sep-2022 add check for empty QUERY_STRRING
 *      21-Sep-2022 add check for missing book name in the QUERY_STRING
 *      21-Sep=2022 add a test for a NULL QUERY_STRING
 *      06-Oct-2022 add tests for invalid parameters in QUERY_STRING
 *      08-Oct-2022 use EXIT_FAILURE and EXIT_SUCCESS on returns
 *      16-Oct-2022 add chapters
 *      18-Oct-2022 extend MySQL initialization and shutdown operations
 *      18-Oct-2022 free memory for the string created by fUrlDecode()
 *      27-Oct-2022 add a query to fetch the ID of the newly inserted title
 *      09-Nov-2022 change sprintf() to asprintf()
 *      15-Nov-2022 change strcpy() to strncpy()
 *      23-Jan-2023 set freed pointers to NULL
 *  Enhancements:
*/

#define _GNU_SOURCE                                                                           // required for asprintf()
#define MAXLEN 1024
#define STRLEN 300

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
MYSQL_FIELD *fields;

char *sParam = NULL;
char sParamOrig[STRLEN] = {'\0'};
// char *sCharacter = NULL;
//char *sCharacterName = NULL;
// char caCharacterName[MAXLEN] = {'\0'};
char *strBookName = NULL;
char caNameBuf[MAXLEN] = {'\0'};
char caStartDte[11] = {'\0'};
char caFinishDte[11] = {'\0'};
char *strStartDteQuoted = NULL;
char *strFinDteQuoted = NULL;
char caAbstract[MAXLEN * 3] = {'\0'};
char caAbstractBuf[MAXLEN * 3] = {'\0'};
char *strAbstractQuoted = NULL;
char caCmnts[MAXLEN * 3] = {'\0'};
char caCmntsBuf[MAXLEN * 3] = {'\0'};
char *strCmntsQuoted = NULL;
char *sTitleID = NULL;
char *sStartDte = NULL;
char *sFinishDte = NULL;
char *sAbstrct = NULL;
char *sCmnts = NULL;
char *sTemp = NULL;
char *sTemp2 = NULL;
int  iTitleID = 0;
int  iAuthorID = 0;
int  iChapters = 0;
int  iSourceID = 0;
int  iSeriesID = 0;
int  iGenreID = 0;
int  iStatusID = 0;
int  iClsfnID = 0;
int  iRatingID = 0;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char *strSQL = NULL;

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// QUERY_STRING format and sample for tesing ---------------------------------------------------------------------------

//    setenv("QUERY_STRING", "bookName=TestBook1&authorId=67&chapters=10&sourceId=10&seriesId=57&genreId=19&statusId=6&clsfnId=3&ratingId=6&startDte=2022-10-10&finishDte=2022-11-11&abstract=&cmnts=TestComments", 1);

// Fetch the QUERY_STRING environment variable paramete string ---------------------------------------------------------

    sParam = getenv("QUERY_STRING");

// check for a NULL query string ---------------------------------------------------------------------------------------

    if (sParam == NULL) {
        printf("QUERY_STRING is NULL. Expecting QUERY_STRING=\"bookName=<bnme>&authorId=<authID>&sourceID=<sourceID>& ...\". \"Terminating bookAddBook.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty query string -------------------------------------------------------------------------------------

    if(sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"bookName=<bnme>&authorId=<authID>&sourceID=<sourceID>& ...\". \"Terminating bookAddBook.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    strncpy(sParamOrig, sParam, STRLEN);                                                                 // make a copy of sParam

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sTemp = strtok(sParam, caDelimiter);

    sscanf(sTemp, "bookName=%[^\n]s", caNameBuf);
    if (caNameBuf[0] == '\0') {
        printf("Query string \"%s\"has no book name. Expecting QUERY_STRING=\"bookName=<bnme>&authorId=<authID>&sourceID=<sourceID>& ...\". Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sTemp2 = fUrlDecode(caNameBuf);
    asprintf(&strBookName, "%s", sTemp2);
    free(sTemp2);
    sTemp2 = NULL;

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "authorId=%d", &iAuthorID);
    if (iAuthorID == 0) {
        printf("Query string \"%s\" has no authorId. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "chapters=%d", &iChapters);
    if (iAuthorID == 0) {
        printf("Query string \"%s\" has no chapters. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "sourceId=%d", &iSourceID);
    if (iSourceID == 0) {
        printf("Query string \"%s\" has no sourceId. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "seriesId=%d", &iSeriesID);
    if (iAuthorID == 0) {
        printf("Query string \"%s\" has no auhorId. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "genreId=%d", &iGenreID);
    if (iGenreID == 0) {
        printf("Query string \"%s\" has no genreId. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "statusId=%d", &iStatusID);
    if (iStatusID == 0) {
        printf("Query string \"%s\" has no statusId. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "clsfnId=%d", &iClsfnID);
    if (iClsfnID == 0) {
        printf("Query string \"%s\" has no clsfnId. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "ratingId=%d", &iRatingID);
    if (iRatingID == 0) {
        printf("Query string \"%s\" has no ratingId. Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "startDte=%[^\n]s", caStartDte);
    if (strlen(caStartDte) == 0) {
        asprintf(&strStartDteQuoted, "%s", "NULL");
    } else {
        asprintf(&strStartDteQuoted, "'%s'", caStartDte);
    }

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "finishDte=%[^\n]s", caFinishDte);
    if (strlen(caFinishDte) == 0) {
        asprintf(&strFinDteQuoted, "%s", "NULL");
    } else {
        asprintf(&strFinDteQuoted, "'%s'", caFinishDte);
    }

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "abstract=%[^\n]s", caAbstractBuf);
    if (strlen(caAbstractBuf) == 0) {
        asprintf(&strAbstractQuoted, "'%s'", "NULL");
    } else {
        asprintf(&strAbstractQuoted, "'%s'", caAbstractBuf);
    }
    sTemp = fUrlDecode(strAbstractQuoted);
    strncpy(caAbstract, sTemp, MAXLEN + 3);
    free(sTemp);
    sTemp = NULL;

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "cmnts=%[^\n]s", caCmntsBuf);
    if (strlen(caCmntsBuf) == 0) {
        asprintf(&strCmntsQuoted, "'%s'", "NULL");
    } else {
        asprintf(&strCmntsQuoted, "'%s'", caCmntsBuf);
    }
    sTemp = fUrlDecode(strCmntsQuoted);
    strncpy(caCmnts, sTemp, MAXLEN + 3);
    free(sTemp);
    sTemp = NULL;

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
        return EXIT_FAILURE;
    }

// set and perform a SQL query to insert the new book title ------------------------------------------------------------

    asprintf(&strSQL, "INSERT INTO risingfast.`Book Titles` "
                   "(`Title Name`, `Author ID`, `Chapters`, `Source ID`, `Series ID`, `Genre ID`, `Status ID`, `Classification ID`, `Rating ID`, Start, Finish, Abstract, Comments)  "
                   "VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, %s, %s, %s, %s);", strBookName, iAuthorID, iChapters, iSourceID, iSeriesID, iGenreID, iStatusID, iClsfnID, iRatingID, strStartDteQuoted, strFinDteQuoted, caAbstract, caCmnts);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    free(strSQL);
    strSQL = NULL;
    free(strStartDteQuoted);
    strStartDteQuoted = NULL;
    free(strFinDteQuoted);
    strFinDteQuoted = NULL;
    free(strAbstractQuoted);
    strAbstractQuoted = NULL;
    free(strCmntsQuoted);
    strCmntsQuoted = NULL;
    free(strBookName);
    strBookName = NULL;

// set and perform a SQL query to return the last inserted ID in the current session and connection --------------------

    asprintf(&strSQL, "SELECT LAST_INSERT_ID()");

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    free(strSQL);
    strSQL = NULL;

// store the result of the last query and seek to the first row of results (only one row is expected) ------------------

    res = mysql_store_result(conn);

    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);

        return EXIT_FAILURE;
    }

    mysql_data_seek(res, 0);

// print the Title ID of the newly inserted title from the first result row --------------------------------------------

    row = mysql_fetch_row(res);                                 // fetch the first row of data from the result 'res' set
    
    printf("%s", row[0]);                                                           // print the newly inserted Title ID
   
// * free resources used by the 'res' array ----------------------------------------------------------------------------

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

    return EXIT_SUCCESS;
}
