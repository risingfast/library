/*  bookChgCharNme.c - CGI to change a new character from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 06-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      06-Dec-2021 start by copying bookAddCharacter.c and modifying
 *      14-Dec-2021 change name from bookChgCharacter.c to bookChgCharNme.c
 *      13-Jun-2022 move to gjarman2020.com
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

char *sgServer = "35.188.123.150";                                                              //mysqlServer IP address
// char *sgServer = "192.168.0.13";                                                               //mysqlServer IP address
char *sgUsername = "root";                                                                  // mysqlSerer logon username
// char *sgUsername = "gjarman";                                                              // mysqlSerer logon username
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

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n\n");

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
        printf("\n");
        printf("Query string is empty. Terminating program");
        printf("\n\n");
        return 1;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sCharacterID = strtok(sParam, caDelimiter);
    sscanf(sCharacterID, "CharacterID=%d", &iCharacterID);

    sCharacter = strtok(NULL, caDelimiter);
    sscanf(sCharacter, "CharacterName=%s", caCharacterName);
    sCharacter = fUrlDecode(caCharacterName);


// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return 0;
    }

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Book Characters` BC "
                   "SET BC.`Character Name` = '%s' "
                   "WHERE BC.`Character ID` = %d;", sCharacter, iCharacterID);

// Call the function to print the SQL results to stdout and terminate the program

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

    printf("Character ID %d updated to '%s'", iCharacterID, sCharacter);

    return EXIT_SUCCESS;
}
