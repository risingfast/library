/*  bookAddCharacter.c - CGI to add a new character from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 03-Dec-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      03-Dec-2021 start by copying bookAddAuthor.c and modifying
 *      04-Dec-2021 add fUrlDecodee to the character name
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
char *sCharacterName = NULL;
char caCharacterName[MAXLEN] = {'\0'};
char *sTitleID = NULL;
int  iTitleID = 0;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block -----------------------------------------------------------------------

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

// check for a NULL query string -------------------------------------------------------------------------------------=

//    setenv("QUERY_STRING", "TitleID=89&CharacterName=Dummy%20Test", 1);

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("\n");
        printf("Query string is empty. Terminating program");
        printf("\n\n");
        return 1;
    }

//    printf("QUERY_STRING: %s", getenv("QUERY_STRING"));
//    printf("\n\n");

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sTitleID = strtok(sParam, caDelimiter);
    sscanf(sTitleID, "TitleID=%d", &iTitleID);
//    printf("TitleID: %d\n", iTitleID);

    sCharacter = strtok(NULL, caDelimiter);
    sscanf(sCharacter, "CharacterName=%s", caCharacterName);
    sCharacter = fUrlDecode(caCharacterName);
//    printf("sCharacter: %s\ncaCharacterName: %s\n", sCharacter, caCharacterName);


// test for an empty QUERY_STRING -------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return 0;
    }

// set a SQL query to insert the new author ---------------------------------------------------------------------------

    sprintf(caSQL, "INSERT INTO risingfast.`Book Characters` "
                   "(`Character Name`, `Title ID`)  "
                   "VALUES ('%s', %d);", sCharacter, iTitleID);

// Call the function to print the SQL results to stdout and terminate the program

//    printf("Query: %s", caSQL);
//    printf("\n\n");


    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

    printf("Character '%s' added", sCharacter);

    return 0;
}
