/*  bookAddAuthor.c - CGI to add a new author from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 19-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      19-Nov-2021 started by copying bookCharacters.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin CORS header
 *      20-Sep-2022 add prompt for parameter if no parameter given
 *      21-Sep-2022 add a test for a NULL QUERY_STRING
 *      21-Sep-2022 fix test for empty QUERY_STRING
 *      21-Sep-2022 fix test for parameter-less QUERY_STRING
 *      06-Oct-2022 extend validity checking for QUERY_STRING

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

char caAuthor[SQL_LEN] = {'\0'};
char *sAuthor = NULL;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and CORS HTML Headers -------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

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

// Format of QUERY_STRING parsed for the author name

//     setenv("QUERY_STRING", "author=Firstname%20Lastname");

// Fetch the QUERY_STRING environment variable parameter string --------------------------------------------------------

    sParam = getenv("QUERY_STRING");

// check for a NULL query string ---------------------------------------------------------------------------------------

//    if (sParam == NULL) {
//        printf("QUERY_STRING identifying the author does not exist. Terminating program");
//        return 1;
//    }

// check for a NULL query string ---------------------------------------------------------------------------------------

    if (sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"author=Firstname%%20Lastname\". Terminating program");
        printf("\n\n");
        return 1;
    }

// check for an empty (non-NULL) query string --------------------------------------------------------------------------

    if (sParam[0] == '\0') {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"author=Firstname%%20Lastname\". Terminating program");
        printf("\n\n");
        return 1;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "author=%s", caAuthor);
    
    if (caAuthor[0] == '\0') {
        printf("Query string \"%sm\" has no author name. Expecting QUERY_STRING=\"author=Firstname Lastname\". Terminating program", sParam);
        printf("\n\n");
        return 1;
    }
    sAuthor = fUrlDecode(caAuthor);

// set a SQL query to insert the new author ----------------------------------------------------------------------------

    sprintf(caSQL, "INSERT INTO risingfast.`Book Authors` "
                   "(`Author Name`)  "
                   "VALUES ('%s');", sAuthor);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

    printf("Author '%s' inserted into Authors table", sAuthor);

    return 0;
}
