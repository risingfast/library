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
char sParamOrig[300] = {'\0'};
char *sCharacter = NULL;
char *sCharacterName = NULL;
char caCharacterName[MAXLEN] = {'\0'};
char *sTitleID = NULL;
int  iTitleID = 0;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block ------------------------------------------------------------------------

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

// Format of QUERY_STRING parsed for character name --------------------------------------------------------------------

//    setenv("QUERY_STRING", "TitleID=89&CharacterName=Dummy%20Test", 1);                  // uncomment for testing only

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

    strcpy(sParamOrig, sParam);                                                          // preserve the value of sParam

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
    sCharacter = fUrlDecode(caCharacterName);

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "INSERT INTO risingfast.`Book Characters` "
                   "(`Character Name`, `Title ID`)  "
                   "VALUES ('%s', %d);", sCharacter, iTitleID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

//    printf("Query: %s", caSQL);                                                          // uncomment for testing only
//    printf("\n\n");                                                                      // uncomment for testing only


    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Character '%s' added", sCharacter);

    return EXIT_SUCCESS;
}
