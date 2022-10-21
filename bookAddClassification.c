/*  bookAddClassification.c - CGI to add a new classification from the bookInquiry.html webpage
 *  Author: Geoffrey Jarman
 *  Started: 19-Nov-2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      19-Nov-2021 started by copying bookAddAuthor.c and modifying
 *      14-Sep-2022 add Access-Control-Allow-Origin: * HTTP header
 *      21-Sep-2022 add check for null string for classification name
 *      21-Sep-2022 add check for empty string for classification name
 *      06-Oct-2022 validate QUERY_STRING parameters
 *      08-Oct-2022 use EXIT_SUCCESS and EXIT_FAILURE on returns
 *      19-Oct-2022 extend MySQL initialization and shutdown operations
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
#define CLASFN_LEN 4000

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

char caClassification[CLASFN_LEN] = {'\0'};
char *sClassification = NULL;
char *sParam = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

int main(void) {

    int i;
    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block ------------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

    sParam = getenv("QUERY_STRING");

// check for a NULL query string ---------------------------------------------------------------------------------------

    if(sParam == NULL) {
        printf("Query string is NULL. Expecting QUERY_STRING=\"classification=<clasfn>\". 1 Terminating \"bookAddClassification.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// check for an empty query string -------------------------------------------------------------------------------------

    if (sParam[0] == 0) {
        printf("Query string is empty (non-NULL). Expecting QUERY_STRING=\"classification=<clsfn>\". Terminating \"bookAddClassification.cgi\"");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "classification=%s", caClassification);
    if (caClassification[0] == '\0') {
         printf("Query string \"%s\" has no classification, Expecting QUERY_STRING=\"classification=<clsfn>\". Terminating \"bookAddClassification.cgi\"", sParam);
         printf("\n\n");
         return EXIT_FAILURE;
    }
    sClassification = fUrlDecode(caClassification);
    strcpy(caClassification, sClassification);
    free(sClassification);

// * initialize the MySQL client library

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

// set a SQL query to insert the new classification --------------------------------------------------------------------

    sprintf(caSQL, "INSERT INTO risingfast.`Book Classifications` "
                   "(`Classification Name`)  "
                   "VALUES ('%s');", caClassification);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    printf("Classification '%s' inserted into Classifications table", caClassification);

// * close the database connection created by mysql_init(NULL) -----------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library ----------------------------------------------------------------------------

    mysql_library_end();

return EXIT_SUCCESS;
}
