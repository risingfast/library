/*  bookAddCharacter.c - CGI to add a new character from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 03-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      03-Dec-2021 start by copying bookAddAuthor.c and modifying
 *      04-Dec-2021 add fUrlDecodee to the character name
 *      14-Sep-2022 add Access-Control-Allow-Origin * HTTP header
 *      06-Oct-2022 check QUERY_STRING for NULL, empty and invalid values
 *      08-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      18-Oct-2022 extend MySQL initialization and shutdown operations
 *      09-Nov-2022 change sprintf() to asprintf()
 *      15-Nov-2022 change strcpy() to strncpy()
    Enhancements:
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
char *sgUsername = "gjarman";                                                              // mysqlServer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
char *sTemp = NULL;
char sParamOrig[STRLEN] = {'\0'};
char *sCharacter = NULL;
char *sCharacterName = NULL;
char caCharacterName[MAXLEN] = {'\0'};
char *sTitleID = NULL;
int  iTitleID = 0;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char *strSQL = NULL;

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// Fetch the QUERY_STRING environment variable parameter string --------------------------------------------------------

    sParam = getenv("QUERY_STRING");

// Check for a NULL query string ---------------------------------------------------------------------------------------

    if (sParam == NULL) {
        printf("QUERY_STRING is NULL. Expecting QUERY_STRING=\"TitleID=<Number>&CharacterName=<Character Name>\". \"Terminating bookAddCharacter\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// Check for an empty query (non-Null) string --------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"TitleID=<Number>&CharacterName=<Character Name>\". \"Terminating bookAddCharacter\"");
        return EXIT_FAILURE;
    }

    strncpy(sParamOrig, sParam, STRLEN);                                                 // preserve the value of sParam

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sTitleID = strtok(sParam, caDelimiter);
    sscanf(sTitleID, "TitleID=%d", &iTitleID);
    if (iTitleID == 0) {
         printf("Query string \"%s\" has no TitleID. Expecting QUERY_STRING=\"TitleID=<number>&CharacterName=<Character Name>\". \"Terminating bookAddCharacter\"", sParamOrig);
         printf("\n\n");
         return EXIT_FAILURE;
    }

    sCharacter = strtok(NULL, caDelimiter);
    sscanf(sCharacter, "CharacterName=%s", caCharacterName);
    if (caCharacterName[0] == '\0') {
         printf("Query string \"%s\" has no CharacterName. Expecting QUERY_STRING=\"TitleID=<number>&CharacterName=<Character Name>\". \"Terminating bookAddCharacter\"", sParamOrig);
         printf("\n\n");
         return EXIT_FAILURE;
    }

    sTemp = fUrlDecode(caCharacterName);
    strncpy(caCharacterName, sTemp, MAXLEN);
    free(sTemp);

// * initialize the MySQL client library

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
        return EXIT_FAILURE;
    }

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    asprintf(&strSQL, "INSERT INTO risingfast.`Book Characters` "
                   "(`Character Name`, `Title ID`)  "
                   "VALUES ('%s', %d);", caCharacterName, iTitleID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Character '%s' added", caCharacterName);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

// * free resources used by strSQL

    free(strSQL);

    return EXIT_SUCCESS;
}
