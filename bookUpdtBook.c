/*  bookUpdtBook.c - CGI to add update a book and attributes from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 13-Jan-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      13-Jan-2022 start by copying bookAddBook.c and modifying
 *      27-Jul-2022 add abstract field
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

// global declarations

char *sgServer = "192.168.0.13";                                                               //mysqlServer IP address
char *sgUsername = "gjarman";                                                              // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                    // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
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
char *sAbstract = NULL;
char *sCmnts = NULL;
char *sTemp = NULL;
int  iBookID = 0;
int  iAuthorID = 0;
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

// print the html content type and <head> block -----------------------------------------------------------------------

    printf("Content-type: text/html\n\n");

// Initialize a connection and connect to the database

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

//    setenv("QUERY_STRING", "bookId=197&bookName=Test%20Title&authorId=107&sourceId=2&seriesId=82&genreId=1&statusId=4&clsfnId=1&ratingId=4&startDte=2021-01-01&finishDte=2021-12-30&abstract=funny book&cmnts=too bad", 1);

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("\n");
        printf("Query string is empty. No parameters passed. Terminating program");
        printf("\n\n");
        return 1;
    }

//    printf("QUERY_STRING: %s", getenv("QUERY_STRING"));
//    printf("\n\n");

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sTemp = strtok(sParam, caDelimiter);
    sscanf(sTemp, "bookId=%d", &iBookID);

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "bookName=%[^\n]s", caNameBuf);
    strcpy(caBookName, fUrlDecode(caNameBuf));

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "authorId=%d", &iAuthorID);

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "sourceId=%d", &iSourceID);
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "seriesId=%d", &iSeriesID);
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "genreId=%d", &iGenreID);
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "statusId=%d", &iStatusID);
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "clsfnId=%d", &iClsfnID);
    
    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "ratingId=%d", &iRatingID);
    
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

// test for an empty QUERY_STRING -------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return 0;
    }

// set a SQL query to insert the updated fields and make the database update ------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Book Titles` "
                   "SET `Title Name` = '%s', `Author ID` = %d, `Source ID` = %d, `Series ID` = %d, `Genre ID` = %d, `Status ID` = %d, `Classification ID` = %d, `Rating ID` = %d, `Start` = %s, `Finish` = %s, `Abstract` = %s, `Comments` = %s  WHERE `Title ID` = %d;"
                   , caBookName, iAuthorID, iSourceID, iSeriesID, iGenreID, iStatusID, iClsfnID, iRatingID, caStartDteQuoted, caFinDteQuoted, caAbstract, caCmnts, iBookID);

//    printf("%s\n", caSQL);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

// set a SQL query to fetch the title id of the updated book and execute the query ------------------------------------

    sprintf(caSQL, "SELECT BT.`Title ID` "
                   "FROM risingfast.`Book Titles` BT "
                   "WHERE BT.`Title Name` = '%s';", caBookName);

//    int iColCount = 0;

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return -1;
    }

// fetch the number of fields in the result

//    iColCount = mysql_num_fields(res);

    mysql_data_seek(res, 0);

// print each row of results

    if(row = mysql_fetch_row(res))
    {
        printf("%s", row[0]);
    } else {
        printf("No book id found");
    }

    mysql_free_result(res);

    return 0;
}
