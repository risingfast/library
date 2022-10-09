/*  bookChgCharacter.c - CGI to change a new character from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 06-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      06-Dec-2021 start by copying bookAddCharacter.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
 *      09-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      09-Oct-2022 add validations for NULL and empty QUERY_STRING
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

char *sParam = NULL;
char *sCharacter = NULL;
char caCharacterName[MAXLEN] = {'\0'};
char *sCharacterID = NULL;
int  iCharacterID = 0;
char caDelimiter[] = "&";

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS headers ------------------------------------------------------------------------

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

// check for a NULL query string ---------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "CharacterID=1041&CharacterName=Dummy%20Change%20Test%20Again", 1);          // for testing

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"CharacterID=999&CharacterName=<ChngdCharNme>\". Terminating \"bookChgCharacter.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"CharacterID=999&CharacterName=<ChngdCharNme>\". Terminating \"bookChgCharacter.cgi\"");
        printf("\n\n");
        return EXIT_SUCCESS;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sCharacterID = strtok(sParam, caDelimiter);
    sscanf(sCharacterID, "CharacterID=%d", &iCharacterID);

    if (iCharacterID == 0) {
        printf("characterID is NULL. Expecting QUERY_STRING=\"CharacterID=999&CharacterName=<chngdcharnme>\". Terminating \"bookChgCharacter.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sCharacter = strtok(NULL, caDelimiter);
    sscanf(sCharacter, "CharacterName=%s", caCharacterName);

    if (caCharacterName == NULL) {
        printf("CharacterName is NULL. Expecting QUERY_STRING=\"CharacterID=999&CharacterName=<chngdcharnme>\". Terminating \"bookChgCharacter.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

    sCharacter = fUrlDecode(caCharacterName);

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Book Characters` BC "
                   "SET BC.`Character Name` = '%s' "
                   "WHERE BC.`Character ID` = %d;", sCharacter, iCharacterID);

// Call the function to print the SQL results to stdout and terminate the program --------------------------------------

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Character ID %d updated to '%s'", iCharacterID, sCharacter);

    return EXIT_SUCCESS;
}
