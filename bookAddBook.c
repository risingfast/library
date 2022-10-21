/*  bookAddBook.c - CGI to add a new book and attributes from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 07-Jan-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
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
 *      18-Oct-2022 free the string created by fUrlDecode()
 *  Enhancements:
*/

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

#define SQL_LEN 10000
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

char *sParam = NULL;
char sParamOrig[300] = {'\0'};
char *sCharacter = NULL;
char *sCharacterName = NULL;
char caCharacterName[MAXLEN] = {'\0'};
char caBookName[MAXLEN] = {'\0'};
char caNameBuf[MAXLEN] = {'\0'};
char caStartDte[11] = {'\0'};
char caFinishDte[11] = {'\0'};
char caStartDteQuoted[13] = {'\0'};
char caFinDteQuoted[13] = {'\0'};
char caAbstract[MAXLEN * 3] = {'\0'};
char caAbstractBuf[MAXLEN * 3] = {'\0'};
char caAbstractQuoted[(MAXLEN * 3) + 2] = {'\0'};
char caCmnts[MAXLEN * 3] = {'\0'};
char caCmntsBuf[MAXLEN * 3] = {'\0'};
char caCmntsQuoted[(MAXLEN * 3) + 2] = {'\0'};
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
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

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

    strcpy(sParamOrig, sParam);                                                                 // make a copy of sParam

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sTemp = strtok(sParam, caDelimiter);

    sscanf(sTemp, "bookName=%[^\n]s", caNameBuf);
    if (caNameBuf[0] == '\0') {
        printf("Query string \"%s\"has no book name. Expecting QUERY_STRING=\"bookName=<bnme>&authorId=<authID>&sourceID=<sourceID>& ...\". Terminating \"bookAddBook.cgi\"", sParamOrig);
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sTemp2 = fUrlDecode(caNameBuf);
    strcpy(caBookName, sTemp2);
    free(sTemp2);

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
        strcpy(caStartDteQuoted, "NULL");
    } else {
        sprintf(caStartDteQuoted, "'%s'", caStartDte);
    }

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "finishDte=%[^\n]s", caFinishDte);
    if (strlen(caFinishDte) == 0) {
        strcpy(caFinDteQuoted, "NULL");
    } else {
        sprintf(caFinDteQuoted, "'%s'", caFinishDte);
    }

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "abstract=%[^\n]s", caAbstractBuf);
    if (strlen(caAbstractBuf) == 0) {
        strcpy(caAbstractQuoted, "NULL");
    } else {
        sprintf(caAbstractQuoted, "'%s'", caAbstractBuf);
    }
    strcpy(caAbstract, fUrlDecode(caAbstractQuoted));

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "cmnts=%[^\n]s", caCmntsBuf);
    if (strlen(caCmntsBuf) == 0) {
        strcpy(caCmntsQuoted, "NULL");
    } else {
        sprintf(caCmntsQuoted, "'%s'", caCmntsBuf);
    }
    strcpy(caCmnts, fUrlDecode(caCmntsQuoted));

// * initialize the MySQL client library -------------------------------------------------------------------------------

   if (mysql_library_init(0, NULL, NULL)) {
   printf("Cannot initialize MySQL Client library\n");
       return EXIT_FAILURE;
    } else {
       printf("MySQL Client library resources assigned\n");
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

// set a SQL query to insert the new book title ------------------------------------------------------------------------

    sprintf(caSQL, "INSERT INTO risingfast.`Book Titles` "
                   "(`Title Name`, `Author ID`, `Chapters`, `Source ID`, `Series ID`, `Genre ID`, `Status ID`, `Classification ID`, `Rating ID`, Start, Finish, Abstract, Comments)  "
                   "VALUES ('%s', %d, %d, %d, %d, %d, %d, %d, %d, %s, %s, %s, %s);", caBookName, iAuthorID, iChapters, iSourceID, iSeriesID, iGenreID, iStatusID, iClsfnID, iRatingID, caStartDteQuoted, caFinDteQuoted, caAbstract, caCmnts);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sprintf(caSQL, "SELECT BT.`Title ID` "
                   "FROM risingfast.`Book Titles` BT "
                   "WHERE BT.`Title Name` = '%s';", caBookName);

//    int iColCount = 0;                                                                   // uncomment for testing only

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// store the result of the query ---------------------------------------------------------------------------------------

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return EXIT_FAILURE;
    }

// fetch the number of fields in the result ----------------------------------------------------------------------------

//    iColCount = mysql_num_fields(res);                                                    // uncomment for testig only

    mysql_data_seek(res, 0);

// print each row of results -------------------------------------------------------------------------------------------

    if(row = mysql_fetch_row(res))
    {
        printf("%s", row[0]);
    } else {
        printf("No book id found");
    }

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) -----------------------------------------------------------

    mysql_close(conn);
    printf("\n");
    printf("MySQL connection is closed");
    printf("\n\n");

// * free resources used by the MySQL library ----------------------------------------------------------------------------

    mysql_library_end();                                                                                                                                                                                            printf("MySQL library resources freed");
    printf("\n\n");

    return EXIT_SUCCESS;
}
