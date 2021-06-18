// includes and defines

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs50.h"
#include "rf50.h"
#include <ctype.h>
#include <stdbool.h>

#define SQL_LEN 5000

void fListTitles(char *, int *, char *, char *, char *); //show all titles & authors on the console
void fListAuthors(char *, int *, char *, char *);//show all authors and title counts on the console
void fMaintainAttributes(char *, int *);                               // maintain attribute tables
void fSetOptions(char *, int *, char *, char *, char *);                // main menu to set options
int  fAddSeries(char *);          // add a series, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddAuthor(char *);         // add an author, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddClassification(char *);// add a classifn, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddRating(char *);          // add a rating, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddSource(char *);          // add a source, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddStatus(char *);          // add a status, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddGenre(char *);            // add a genre, return EXIT_FAILURE if not added or EXIT_SUCCESS
void fAddTitle(char *);                                      // add a title with default attributes
void fCharacters(char *);                         // view, add, change delete characters on a title
int  fGetMaxTitleID(void);                                       // get the max value of Title ID's
int  fGetMinTitleID(void);                                       // get the min value of Title ID's
void fShowAttributes(int);                                  // show title attributes on the console
void fShowCharacters(int, char *);                                // show characters on the console
void fShowAllAuthors(int *);                                     // show all authors on the console
void fShowAllSeries(int *);                                       // show all series on the console
void fShowAllSources(int *);                                     // show all sources on the console
void fShowAllGenres(int *);                                       // show all genres on the console
void fShowAllStatuses(int *);                                   // show all statuses on the console
void fShowAllClassifications(int *);                     // show all classifications on the console
void fShowAllRatings(int *);                                     // show all ratings on the console
void fShowMainMenu(void);                                                     // show the main menu
void fShowAttributesMenu(void);                                               // show the main menu
void fShowTitle(int, char *);                                   // show a title name on the console
char *fEscapeSingleQuote(char *);                    // escape ' as '' for mysql in string literals
bool fCheckTitleIDExists(int);                        // check a Title ID and return True if exists
void fDeleteAuthor(char *);                                      // delete an author with no titles
void fDeleteSeries(char *);                                       // delete a series with no titles
void fDeleteGenre(char *);                                         // delete a genre with no titles
void fDeleteSource(char *);                                       // delete a source with no titles
void fDeleteRating(char *);                                       // delete a rating with no titles
void fDeleteStatus(char *);                                       // delete a status with no titles
void fDeleteClassification(char *);                       // delete a classification with no titles
void fSearchAuthors(char *);                                // search authors and show an author id
void fSearchTitles(char *);                                    // search titles and show a title ID
void fSearchSeries(char *);                                   // search series and show a series ID
void fSearchGenres(char *);                                    // search genres and show a genre ID
void fSearchSources(char *);                                 // search sources and show a source ID
void fSearchRatings(char *);                                 // search ratings and show a rating ID
void fSearchStatuses(char *);                               // search statuses and show a status ID
void fSearchClassifications(char *);         // search classifications and show a classification ID
void fUpdateTitleAndAttributes(char *, int *);   // update attributes of a Title but not characters
void fUpdateAuthor(char *);                                                     // update an author
void fUpdateClassification(char *);                                      // update a classification
void fUpdateRating(char *);                                                      // update a rating
void fUpdateSeries(char *);                                                      // update a series
void fUpdateSource(char *);                                                      // update a source
void fUpdateStatus(char *);                                                      // update a status
void fUpdateGenre(char *);                                                       // update an genre
bool fTestDbConnection(void);                                    // test connection to the database
int  fGetFieldLength(char *, char *);             // get the max field length of a field in a table
void fGetPwdFromConsole(void);                                   // get a password from the console

// global declarations

char *sgServer = "192.168.0.13";                                           //mysqlServer IP address
char *sgUsername = "gjarman";                                          // mysqlSerer logon username
char sgPassword[20] = {'\0'};                                 // password to connect to mysqlserver
char *sgDatabase = "risingfast";                            // default database name on mysqlserver

MYSQL *conn;

int main(int argc, char** argv)
{
// declarations

    char *strPrgNme = strcat(argv[0] + 2, " -- Library and reading log");           // program name
    bool bHelp = false;                                              // help flag to show help text
    bool bExitMain = false;                                             // flag to exit the program
    char charMainChoice = '0';                                                  // main menu choice
    int  intDisplayPageLength = 83;                                // display page lines for paging
    char charDisplayPageWidth = 'N';                           // display page width wide or narrow
    char charDisplayPageFormat = 'T';                         // display page format (T)ab or (C)SV
    char charDisplayOrder = 'A';                           // display order asc or desc on Title ID

// Search for input parameters

    for(int i = 1; i < argc; i++)
    {
        if(strcmp("--help", argv[i]) == 0 || strcmp("--h", argv[i]) == 0)
        {
            bHelp = true;
        }
    }

    if(bHelp == true)
    {
        printf("\n");
        printf("Command line switch --h or --help not specified so help suppressed");
        printf("\n");
    }

// get a password from the console and test the db connection

    fRetitleConsole(strPrgNme);
    fGetPwdFromConsole();
    if(strcmp("BadSoExit", sgPassword) == 0)
    {
        printf("\n");
        return EXIT_FAILURE;
    }
    fRetitleConsole(strPrgNme);

// Initialize a connection and connect to the database$

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, sgServer, sgUsername, sgPassword, sgDatabase, 0, NULL, 0))
    {
        printf("\n");
        printf("Failed to connect to MySQL Server %s in module %s()", sgServer, __func__);
        printf("\n\n");
        printf("Error: %s\n", mysql_error(conn));
        printf("\n");
        fPressEnterToContinue();
        printf("\n");
        return -1;
    }

// print the main menu

    while(bExitMain == false)
    {
        while(charMainChoice == '0')
        {
            fRetitleConsole(strPrgNme);
            fShowMainMenu();
            charMainChoice = GetChar();
            if(strchr("12345678xXcCpPuUaAlLtTrR", charMainChoice) == NULL)
            {
                charMainChoice = '0';
                fRetitleConsole(strPrgNme);
                printf("\n");
            }
        }

        if(strchr("1lL", charMainChoice) != NULL)
        {
            printf("\n");
            fListTitles(strPrgNme, &intDisplayPageLength, &charDisplayPageWidth, &charDisplayPageFormat, &charDisplayOrder);
            printf("\n");
            charMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("2rR", charMainChoice) != NULL)
        {
            printf("\n");
            fListAuthors(strPrgNme, &intDisplayPageLength, &charDisplayPageWidth, &charDisplayOrder);
            printf("\n");
            charMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("3tT", charMainChoice) != NULL)
        {
            printf("\n");
            fAddTitle(strPrgNme);
            printf("\n");
            charMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("aA4", charMainChoice) != NULL)
        {
            printf("\n");
            fMaintainAttributes(strPrgNme, &intDisplayPageLength);
            printf("\n");
            charMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("uU5", charMainChoice) != NULL)
        {
            printf("\n");
            fUpdateTitleAndAttributes(strPrgNme, &intDisplayPageLength);
            printf("\n");
            charMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("cC6", charMainChoice) != NULL)
        {
            printf("\n");
            fCharacters(strPrgNme);
            printf("\n");
            charMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("pP7", charMainChoice) != NULL)
        {
            printf("\n");
            fSetOptions(strPrgNme, &intDisplayPageLength, &charDisplayPageWidth, &charDisplayPageFormat, &charDisplayOrder);
            printf("\n");
            charMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("xX8", charMainChoice) != NULL)
        {
            bExitMain = true;
        }
    }

    mysql_close(conn);
    system("clear");
    return EXIT_SUCCESS;
}

void fListTitles(char *strPrgNme, int *pintDisplayPageLength, char *pcharDisplayPageWidth, char *pcharDisplayPageFormat, char *pCharDisplayOrder)
{
    int intColCount = 0;
    int *intLengths = NULL;
    int intRowCount = 0;
    int  intStatusFilter = 0;
    char charStatusFilterChoice = '0';
    char strOrder[6] = {'D', 'E', 'S', 'C', '\0'};
    char strSQL[SQL_LEN] = {'\0'};
    bool bEndOfPrintBlock = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    if(*pCharDisplayOrder == 'A')
    {
        strcpy(strOrder, "ASC");
    }
    else if(*pCharDisplayOrder == 'D')
    {
        strcpy(strOrder, "DESC");
    }

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > List Titles > Titles and Authors and Character Counts");
    printf("\n\n");
    printf("(A)ll, (U)nassigned, (N)ot Started, (S)tarted, (P)aused, (F)inished or E(x)it");
    printf("\n\n");
    while(strchr("aAuUnNsSfFpPxX", charStatusFilterChoice) == NULL)
    {
        printf("Choice: ");
        charStatusFilterChoice = toupper(GetChar());
    }
    if(toupper(charStatusFilterChoice) == 'X')
    {
        return;
    }
    else if(charStatusFilterChoice == 'A')
    {
        intStatusFilter = 0;
    }
    else if(charStatusFilterChoice == 'U')
    {
        intStatusFilter = 6;
    }
    else if(charStatusFilterChoice == 'N')
    {
        intStatusFilter = 1;
    }
    else if(charStatusFilterChoice == 'S')
    {
        intStatusFilter = 2;
    }
    else if(charStatusFilterChoice == 'P')
    {
        intStatusFilter = 5;
    }
    else if(charStatusFilterChoice == 'F')
    {
        intStatusFilter = 4;
    }

    if(intStatusFilter == 0)
    {
        sprintf(strSQL, "SELECT T.`Title ID` , T.`Title Name` 'Title' , A.`Author Name` 'Author', "
                        "COUNT(C.`Character Name`) 'Chars', T.`Start`, T.`Finish`,"
                        "T.`Comments` FROM risingfast.`Book Titles` T "
                        "LEFT JOIN risingfast.`Book Authors` A ON T.`Author ID` = A.`Author ID`"
                        "LEFT JOIN risingfast.`Book Characters` C on T.`Title ID` = C.`Title ID`"
                        "GROUP BY T.`Title ID`"
                        "ORDER BY T.`Title ID` %s", strOrder);
    }
    else
    {
        sprintf(strSQL, "SELECT T.`Title ID` , T.`Title Name` 'Title' , A.`Author Name` 'Author', "
                        "COUNT(C.`Character Name`) 'Chars', T.`Start`, T.`Finish`,"
                        "T.`Comments` FROM risingfast.`Book Titles` T "
                        "LEFT JOIN risingfast.`Book Authors` A ON T.`Author ID` = A.`Author ID`"
                        "LEFT JOIN risingfast.`Book Characters` C on T.`Title ID` = C.`Title ID`"
                        "WHERE T.`Status ID` = %d "
                        "GROUP BY T.`Title ID`"
                        "ORDER BY T.`Title ID` %s", intStatusFilter, strOrder);
    }

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return;
    }

// fetch the number of fields in the result

    if(*pcharDisplayPageWidth == 'W')
    {
        intColCount = mysql_num_fields(res);
    }
    else if(*pcharDisplayPageWidth == 'N')
    {
        intColCount = mysql_num_fields(res) - 3;
    }

// fetch the max width of each column

    intLengths = (int *)calloc(intColCount, sizeof(int));

    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < intColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > intLengths[k])
                {
                     intLengths[k] = strlen(row[k]);
                }
            }
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("\n");
    printf("Title, Author, Character Count");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        if(*pcharDisplayPageFormat == 'T')
        {
            bEndOfPrintBlock = false;
            intRowCount++;
            for(int i = 0; i < intColCount; i++)
            {
                if((i==3) || (i==0))
                {
                    printf("%*s", intLengths[i] + 1, row[i] ? row[i] : "");
                }
                else
                {
                    printf("  %-*s", intLengths[i] + 1, row[i] ? row[i] : "");
                }
            }
            printf("\n");
            if(intRowCount >= *pintDisplayPageLength)
            {
                printf("\n");
                fPressEnterToContinue();
                printf("\n");
                intRowCount = 0;
            }
        }
        else
        {
            for(int i = 0; i < intColCount; i++)
            {
                if(i == 0)
                {
                    printf("     ");
                }
                else if(i == 3)
                {
                    continue;
                }
                else if(row[i])
                {
                    printf("%s, ", row[i]);
                }
                else
                {
                    printf("%s", "");
                }
            }
            printf("\n");
        }
    }

    printf("\n");
    if(bEndOfPrintBlock == false)
    {
        fPressEnterToContinue();
    }

    fRetitleConsole(strPrgNme);
    free(intLengths);
    mysql_free_result(res);
    return;
}

void fMaintainAttributes(char *strPrgNme, int *pintDisplayPageLength)
{

    char charAttributeChoice = '0';
    char charAddChangeDeleteListSearchExitChoice = '0';
    char *strAuthorSearch = NULL;
    char *strSeriesSearch = NULL;
    char *strGenreSearch = NULL;
    char *strSourceSearch = NULL;
    char *strRatingSearch = NULL;
    char *strStatusSearch = NULL;
    char *strClassificationSearch = NULL;
    bool bExitAttributes = false;
    bool bAttributeAdded = false;

    fRetitleConsole(strPrgNme);

    while(bExitAttributes == false)
    {
        while(charAttributeChoice == '0')
        {
            bAttributeAdded = false;
            fShowAttributesMenu();
/*
            printf("\n");
            printf("Main Menu > Maintain Attributes");
            printf("\n\n");
            printf("1. (A)uthor");
            printf("\n");
            printf("2. (C)lassification");
            printf("\n");
            printf("3. (R)ating");
            printf("\n");
            printf("4. S(e)ries");
            printf("\n");
            printf("5. S(o)urces");
            printf("\n");
            printf("6. (S)tatuses");
            printf("\n");
            printf("7. (G)enres");
            printf("\n");
            printf("8. E(x)it");
            printf("\n\n");
            printf("Choice: ");
*/
            charAttributeChoice = GetChar();
            if(strchr("12345678AaCcRrEeOoSsGgXx", charAttributeChoice) == NULL)
            {
                charAttributeChoice = 0;
            }
            printf("\n");
        }
        if(strchr("1Aa", charAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(strPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Authors");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                charAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(charAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddAuthor(strPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    charAttributeChoice = '0';
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateAuthor(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteAuthor(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Authors > List");
                    printf("\n\n");
                    fShowAllAuthors(pintDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Authors > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    strAuthorSearch = GetString();
                    if(strstr("xX", strAuthorSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchAuthors(strAuthorSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    charAttributeChoice = '0';
                }
            }
        }
        else if(strchr("2Cc", charAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(strPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Classifications");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                charAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(charAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddClassification(strPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    charAttributeChoice = '0';
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateClassification(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteClassification(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Classifications > List");
                    printf("\n\n");
                    fShowAllClassifications(pintDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Classifications > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    strClassificationSearch = GetString();
                    if(strstr("xX", strClassificationSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchClassifications(strClassificationSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    charAttributeChoice = '0';
                }
            }
        }
        else if(strchr("3Rr", charAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(strPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Ratings");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                charAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(charAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddRating(strPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    charAttributeChoice = '0';
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateRating(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteRating(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Ratings > List");
                    printf("\n\n");
                    fShowAllRatings(pintDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Ratings > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    strRatingSearch = GetString();
                    if(strstr("xX", strRatingSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchRatings(strRatingSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    charAttributeChoice = '0';
                }
            }
        }
        else if(strchr("4Ee", charAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(strPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Series");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                charAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(charAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddSeries(strPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    charAttributeChoice = '0';
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateSeries(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteSeries(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Series > List");
                    printf("\n\n");
                    fShowAllSeries(pintDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Series > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    strSeriesSearch = GetString();
                    if(strstr("xX", strSeriesSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchSeries(strSeriesSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    charAttributeChoice = '0';
                }
            }
        }
        else if(strchr("5Oo", charAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(strPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attribues > Sources");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                charAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(charAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddSource(strPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    charAttributeChoice = '0';
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateSource(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteSource(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Sources > List");
                    printf("\n\n");
                    fShowAllSources(pintDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Sources > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    strSourceSearch = GetString();
                    if(strstr("xX", strSourceSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchSources(strSourceSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'X')
                {
                     bAttributeAdded = true;
                     charAttributeChoice = '0';
                }
            }
        }
        else if(strchr("6Ss", charAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(strPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Statuses");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                charAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(charAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddStatus(strPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    charAttributeChoice = '0';
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateStatus(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'D')
                {
                     printf("\n");
                     fDeleteStatus(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Statuses > List");
                    printf("\n\n");
                    fShowAllStatuses(pintDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Statuses > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    strStatusSearch = GetString();
                    if(strstr("xX", strStatusSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchStatuses(strStatusSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    charAttributeChoice = '0';
                }
            }
        }
        else if(strchr("7Gg", charAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(strPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Genres");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                charAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(charAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddGenre(strPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    charAttributeChoice = '0';
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateGenre(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'D')
                {
                    fDeleteGenre(strPrgNme);
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Genres > List");
                    printf("\n\n");
                    fShowAllGenres(pintDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(strPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Genres > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    strGenreSearch = GetString();
                    if(strstr("xX", strGenreSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchGenres(strGenreSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(charAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    charAttributeChoice = '0';
                }
            }
        }
        else if(strchr("8Xx", charAttributeChoice) != NULL)
        {
            bExitAttributes = true;
        }

        fRetitleConsole(strPrgNme);
    }

    return;
}

void fSetOptions(char *strPrgNme, int *pintDisplayPageLength, char *pcharDisplayPageWidth, char *pcharDisplayPageFormat, char *pcharDisplayOrder)
{

    int  charOptionChoice = '0';
    bool bExitOptions = false;
    bool bValidOptionChoice = false;

    fRetitleConsole(strPrgNme);

    while(bExitOptions == false)
    {
        while(charOptionChoice == '0')
        {
            printf("\n");
            printf("Main Menu > Set Options");
            printf("\n");
            printf("\n");
            printf("1. Display (L)ength: %9d", *pintDisplayPageLength);
            printf("\n");
            printf("2. Display (W)idth: %10s", (*pcharDisplayPageWidth == 'N') ? "Narrow" : "Wide");
            printf("\n");
            printf("3. List (F)ormat: %12s", (*pcharDisplayPageFormat == 'T') ? "Tab" : "CSV");
            printf("\n");
            printf("4. List (O)rder: %13s", (*pcharDisplayOrder == 'A') ? "Ascending" : "Descending");
            printf("\n");
            printf("5. (P)rint Options");
            printf("\n");
            printf("6. (R)eset Options");
            printf("\n");
            printf("7. E(x)it");
            printf("\n\n");
            printf("Choice: ");
            charOptionChoice = GetChar();
            if(strchr("1234567LloOpPrRxX", charOptionChoice) == NULL)
            {
                charOptionChoice = '0';
            }
        }
        if(strchr("1lL", charOptionChoice) != NULL)
        {
            printf("\n");
            printf("Display Length: ");
            *pintDisplayPageLength = GetInt();
            printf("\n");
            charOptionChoice = '0';
        }
        else if(strchr("2wW", charOptionChoice) != NULL)
        {
            printf("\n");
            while(bValidOptionChoice == false)
            {
                printf("Display Width ((W)ide or (N)arrow) or E(x)it: ");
                *pcharDisplayPageWidth = toupper(GetChar());
                if(*pcharDisplayPageWidth == 'W')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcharDisplayPageWidth == 'N')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcharDisplayPageWidth == 'X')
                {
                    bValidOptionChoice = true;
                }
                else
                {
                    bValidOptionChoice = false;
                }
            }
            printf("\n");
            bValidOptionChoice = false;
            charOptionChoice = '0';
        }
        else if(strchr("3fF", charOptionChoice) != NULL)
        {
            printf("\n");
            while(bValidOptionChoice == false)
            {
                printf("Display Format ((T)ab or (C)SV) or E(x)it: ");
                *pcharDisplayPageFormat = toupper(GetChar());
                if(*pcharDisplayPageFormat == 'T')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcharDisplayPageFormat == 'C')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcharDisplayPageFormat == 'X')
                {
                    bValidOptionChoice = true;
                }
                else
                {
                    bValidOptionChoice = false;
                }
            }
            printf("\n");
            bValidOptionChoice = false;
            charOptionChoice = '0';
        }
        else if(strchr("4oO", charOptionChoice) != NULL)
        {
            printf("\n");
            while(bValidOptionChoice == false)
            {
                printf("Display Order ((A)sc or (D)esc or E(x)it): ");
                *pcharDisplayOrder = toupper(GetChar());
                if(*pcharDisplayOrder == 'A')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcharDisplayOrder == 'D')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcharDisplayOrder == 'X')
                {
                    bValidOptionChoice = true;
                }
                else
                {
                    bValidOptionChoice = false;
                }
            }
            printf("\n");
            bValidOptionChoice = false;
            charOptionChoice = '0';
        }
        else if(strchr("5pP", charOptionChoice) != NULL)
        {
            *pcharDisplayPageFormat = 'C';
            *pcharDisplayOrder = 'A';
            *pintDisplayPageLength = 1000;
            *pcharDisplayPageWidth = 'W';
            bValidOptionChoice = false;
            charOptionChoice = '0';
        }
        else if(strchr("6rR", charOptionChoice) != NULL)
        {
            *pcharDisplayPageFormat = 'T';
            *pcharDisplayOrder = 'A';
            *pintDisplayPageLength = 80;
            *pcharDisplayPageWidth = 'N';
            bValidOptionChoice = false;
            charOptionChoice = '0';
        }
        else if(strchr("7xX", charOptionChoice) != NULL)
        {
            bExitOptions = true;
        }

        fRetitleConsole(strPrgNme);
    }

    return;
}

int fAddSeries(char *strPrgNme)
{
    char *strSeriesName = NULL;
    char *strSeriesNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intSeriesMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Series > Add");
    printf("\n\n");

    intSeriesMaxLength = fGetFieldLength("Book Series", "Series Name");

    do{
        printf("Series Name or E(x)it (max %d chars): ", intSeriesMaxLength);
        strSeriesName = GetString();
        if(strlen(strSeriesName) > intSeriesMaxLength)
        {
            printf("Series Name max %d chars. You entered %d chars", intSeriesMaxLength, (int) strlen(strSeriesName));
            printf("\n");
            strSeriesName[0] = '\0';
        }
    } while(strlen(strSeriesName) == 0);

    strSeriesNameEscaped = fEscapeSingleQuote(strSeriesName);

    if(strcmp(strSeriesName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(strSQL, "INSERT INTO `Book Series` (`Series Name`) VALUES('%s')", strSeriesNameEscaped);

// initialize a connection and connect to the database

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        printf("Series Added: %s", strSeriesName);
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        printf("Series not added - mysql_query failed");
        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    fRetitleConsole(strPrgNme);

    free(strSeriesName);
    free(strSeriesNameEscaped);
    return EXIT_SUCCESS;
}

int fAddAuthor(char *strPrgNme)
{
    char *strAuthorName = NULL;
    char *strEscapedAuthorName = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intAuthorMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Authors > Add");
    printf("\n\n");

// get the author name

    intAuthorMaxLength = fGetFieldLength("Book Authors", "Author Name");
    do {
        printf("Author Name or E(x)it (max %d chars): ", intAuthorMaxLength);
        strAuthorName = GetString();
        if(strlen(strAuthorName) > intAuthorMaxLength)
        {
            printf("Author Name max %d chars. You entered %d chars", intAuthorMaxLength, (int) strlen(strAuthorName));
            printf("\n");
            strAuthorName[0] = '\0';
        }
    }while (strlen(strAuthorName) == 0);

    strEscapedAuthorName = fEscapeSingleQuote(strAuthorName);

    if(strcmp(strAuthorName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(strSQL, "INSERT INTO `Book Authors` (`Author Name`) VALUES('%s')", strEscapedAuthorName);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        printf("Author Added: %s", strAuthorName);
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        printf("Author not added - mysql_query failed");
        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    free(strEscapedAuthorName);
    free(strAuthorName);
    fRetitleConsole(strPrgNme);
    return EXIT_SUCCESS;
}

int fAddClassification(char *strPrgNme)
{
    char *strClassificationName = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    char *strClassificationNameEscaped = NULL;
    int  intClassificationMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Classification > Add");
    printf("\n\n");

    intClassificationMaxLength = fGetFieldLength("Book Classifications", "Classification Name");

    do
    {
        printf("Classification Name or E(x)it (max %d chars): ", intClassificationMaxLength);
        strClassificationName = GetString();
        if(strlen(strClassificationName) > intClassificationMaxLength)
        {
            printf("Classification Name max %d chars. You entered %d chars", intClassificationMaxLength, (int) strlen(strClassificationName));
            printf("\n");
            strClassificationName[0] = '\0';
        }
    } while(strlen(strClassificationName) == 0);

    strClassificationNameEscaped = fEscapeSingleQuote(strClassificationName);

    if(strcmp(strClassificationName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");

    sprintf(strSQL, "INSERT INTO `Book Classifications` (`Classification Name`) VALUES('%s')", strClassificationNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        printf("Classification Added: %s", strClassificationName);
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        printf("Classification not added - mysql_query() failed");
        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    fRetitleConsole(strPrgNme);
    free(strClassificationNameEscaped);
    free(strClassificationName);
    return EXIT_SUCCESS;
}

int fAddRating(char *strPrgNme)
{
    char *strRatingName = NULL;
    char *strRatingNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRatingMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Rating > Add");
    printf("\n\n");

    intRatingMaxLength = fGetFieldLength("Book Ratings", "Rating Name");

    do{
        printf("Rating Name or E(x)it (max %d chars): ", intRatingMaxLength);
        strRatingName = GetString();
        if(strlen(strRatingName) > intRatingMaxLength)
        {
            printf("Rating Name max %d chars. You entered %d chars", intRatingMaxLength, (int) strlen(strRatingName));
            printf("\n");
            strRatingName[0] = '\0';
        }
    } while(strlen(strRatingName) == 0);

    strRatingNameEscaped = fEscapeSingleQuote(strRatingName);

    if(strcmp(strRatingName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(strSQL, "INSERT INTO `Book Ratings` (`Rating Name`) VALUES('%s')", strRatingNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        printf("Rating Added: %s", strRatingName);
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        printf("Rating not added - mysql_query failed");
        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    fRetitleConsole(strPrgNme);
    free(strRatingName);
    free(strRatingNameEscaped);
    return EXIT_SUCCESS;
}

int fAddSource(char *strPrgNme)
{
    char *strSourceName = NULL;
    char *strSourceNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intSourceMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Source > Add");
    printf("\n\n");

    intSourceMaxLength = fGetFieldLength("Book Sources", "Source Name");

    do {
        printf("Source Name or E(x)it (max %d chars): ", intSourceMaxLength);
        strSourceName = GetString();
        if (strlen(strSourceName) > intSourceMaxLength)
        {
            printf("Source Name max %d chars. You entered %d chars", intSourceMaxLength, (int) strlen(strSourceName));
            printf("\n");
            strSourceName[0] = '\0';
        }
    } while(strlen(strSourceName) == 0);

    strSourceNameEscaped = fEscapeSingleQuote(strSourceName);

    if(strcmp(strSourceName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(strSQL, "INSERT INTO `Book Sources` (`Source Name`) VALUES('%s')", strSourceNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        printf("Source Added: %s", strSourceName);
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        printf("Source not added - mysql_query failed");
        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    fRetitleConsole(strPrgNme);

    free(strSourceName);
    free(strSourceNameEscaped);
    return EXIT_SUCCESS;
}

int fAddStatus(char *strPrgNme)
{
    char *strStatusName = NULL;
    char *strStatusNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intStatusMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Status > Add");
    printf("\n\n");

    intStatusMaxLength = fGetFieldLength("Book Statuses", "Status Name");

    do {
        printf("Status Name or E(x)it (max %d chars): ", intStatusMaxLength);
        strStatusName = GetString();
        if (strlen(strStatusName) > intStatusMaxLength)
        {
             printf("Status Name max %d chars. You entered %d chars", intStatusMaxLength, (int) strlen(strStatusName));
             printf("\n");
             strStatusName[0] = '\0';
        }
    } while(strlen(strStatusName) == 0);

    strStatusNameEscaped = fEscapeSingleQuote(strStatusName);

    if(strcmp(strStatusName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(strSQL, "INSERT INTO `Book Statuses` (`Status Name`) VALUES('%s')", strStatusNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        printf("Status Added: %s", strStatusName);
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        printf("Status not added - mysql_query failed");
        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    fRetitleConsole(strPrgNme);
    free(strStatusName);
    free(strStatusNameEscaped);
    return EXIT_SUCCESS;
}

int fAddGenre(char *strPrgNme)
{
    char *strGenreName = NULL;
    char *strGenreDesc = NULL;
    char *strGenreNameEscaped = NULL;
    char *strGenreDescEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intGenreNameMaxLength = 0;
    int  intGenreDescMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Genre > Add:");
    printf("\n\n");

    intGenreNameMaxLength = fGetFieldLength("Book Genres", "Genre Name");
    intGenreDescMaxLength = fGetFieldLength("Book Genres", "Genre Description");

    do{
        printf("Genre Name or E(x)it (max %d chars): ", intGenreNameMaxLength);
        strGenreName = GetString();
        if (strlen(strGenreName) > intGenreNameMaxLength)
        {
             printf("Genre Name max %d chars. You entered %d chars", intGenreNameMaxLength, (int) strlen(strGenreName));
             printf("\n");
             strGenreName[0] = '\0';
        }
    } while(strlen(strGenreName) == 0);

    strGenreNameEscaped = fEscapeSingleQuote(strGenreName);

    if(strcmp(strGenreName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }

    do{
        printf("Genre Description or E(x)it (max %d chars): ", intGenreDescMaxLength);
        strGenreDesc = GetString();
        if (strlen(strGenreDesc) > intGenreDescMaxLength)
        {
             printf("Genre Desc max %d chars. You entered %d chars", intGenreDescMaxLength, (int) strlen(strGenreDesc));
             printf("\n");
             strGenreDesc[0] = '\0';
        }
    } while(strlen(strGenreDesc) == 0);

    strGenreDescEscaped = fEscapeSingleQuote(strGenreDesc);

    if(strcmp(strGenreDesc, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return EXIT_SUCCESS;
    }

    printf("\n");
    sprintf(strSQL, "INSERT INTO `Book Genres` (`Genre Name`, `Genre Description`) VALUES('%s', '%s')", strGenreNameEscaped, strGenreDescEscaped);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        printf("Genre Added: %s, %s", strGenreName, strGenreDesc);
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        printf("Genre not added - mysql_query failed");

        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

    fRetitleConsole(strPrgNme);
    free(strGenreName);
    free(strGenreNameEscaped);
    free(strGenreDesc);
    free(strGenreDescEscaped);
    return EXIT_SUCCESS;
}

void fAddTitle(char *strPrgNme)
{
    char *strTitleName = NULL;
    char *strTitleNameEscaped = NULL;
    char *strStartDate;
    char strSQL[SQL_LEN] = {'\0'};
    int  intTitleMaxLength = 0;

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Add Title");
    printf("\n\n");

// Get the title name and start date from the console

    intTitleMaxLength = fGetFieldLength("Book Titles", "Title Name");
    do {
        printf("Title Name or E(x)it (max %d chars): ", intTitleMaxLength);
        strTitleName = GetString();
        if(strlen(strTitleName) > intTitleMaxLength)
        {
            printf("Title Name max %d chars. You entered %d chars", intTitleMaxLength, (int) strlen(strTitleName));
            printf("\n");
            strTitleName[0] = '\0';
        }
    } while(strlen(strTitleName) == 0);

    strTitleNameEscaped = fEscapeSingleQuote(strTitleName);

    if(strcmp(strTitleName, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return;
    }

    printf("Start Date (YYYY-MM-DD) or E(x)it: ");
    strStartDate = GetString();
    if(strcmp(strStartDate, "x") == 0)
    {
        fRetitleConsole(strPrgNme);
        return;
    }

    printf("\n");
    if(strlen(strStartDate) == 0)
    {
        sprintf(strSQL, "INSERT INTO `Book Titles` (`Title Name`, `Author ID`, `Source ID`, `Series ID`"
                        ", `Genre ID`, `Status ID`, `Classification ID`,"
                        "`Rating ID`, `Start`) VALUES ('%s', 67, 10, 57, 17, 6, 3, 6, null)", strTitleNameEscaped);
    }
    else
    {
        sprintf(strSQL, "INSERT INTO `Book Titles` (`Title Name`, `Author ID`, `Source ID`, `Series ID`"
                        ", `Genre ID`, `Status ID`, `Classification ID`,"
                        "`Rating ID`, `Start`) VALUES ('%s', 67, 10, 57, 17, 6, 3, 6, '%s')", strTitleNameEscaped, strStartDate);
    }

// execute the query and check for no result

    if(mysql_query(conn, strSQL) == 0)
    {
        if(strlen(strStartDate) > 0)
        {
            printf("Title Added: %s starting %s", strTitleName, strStartDate);
            printf("\n\n");
            fPressEnterToContinue();
        }
        else
        {
            printf("Title Added: %s", strTitleName);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }
    else
    {
        printf("Title not added - mysql_query failed");
        printf("\n\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

    free(strTitleNameEscaped);
    free(strTitleName);
    fRetitleConsole(strPrgNme);
    return;
}

void fUpdateTitleAndAttributes(char *strPrgNme, int *pintDisplayPageLength)
{
    int  intTitleID = 0;
    int  intMaxTitleID = 0;
    int  intMinTitleID = 0;
    int  intNewAuthorID = 0;
    int  intNewSourceID = 0;
    int  intNewSeriesID = 0;
    int  intNewGenreID = 0;
    int  intNewStatusID = 0;
    int  intNewClassnID = 0;
    int  intNewRatingID = 0;
    bool bExitAttributeChoice = false;
    bool bTitleFound = false;
    bool bAttributeQueryIsValid = false;
    char strSQL[SQL_LEN] = {'\0'};
    char *strTitleName = NULL;
    char *strNewStartDate = NULL;
    char *strNewFinishDate = NULL;
    char *strNewComments = NULL;
    char *strNewCommentsEscaped = NULL;
    char charUpdateTitleAndAttributesAction = 'X';
    char *strTitleSearch = NULL;
    char *strAttribChoice = "00";                                  // choice of attribute to change

    while(bTitleFound == false)
    {
        fRetitleConsole(strPrgNme);

        printf("\n");
        printf("Main Menu > Update a Title and Attributes");
        printf("\n\n");

        printf("(T)itle ID, (S)earch or E(x)it: ");
        charUpdateTitleAndAttributesAction = GetChar();

        if(toupper(charUpdateTitleAndAttributesAction) == 'S')
        {
            printf("\n");
            printf("Search String: ");
            strTitleSearch = GetString();
            fSearchTitles(strTitleSearch);
            printf("\n");
            fPressEnterToContinue();
        }
        else if(toupper(charUpdateTitleAndAttributesAction) == 'T')
        {
            printf("\n");
            printf("Title ID: ");
            intTitleID = GetInt();
            if(fCheckTitleIDExists(intTitleID) == true)
            {
                bTitleFound = true;
            }
            else
            {
                bTitleFound = false;
            }
            printf("\n");
        }
        else if(toupper(charUpdateTitleAndAttributesAction) == 'X')
        {
            fRetitleConsole(strPrgNme);
            return;
        }
        else
        {
            printf("\n");
            printf("Not a valid choice so exiting");
            bTitleFound = true;
            printf("\n\n");
            fPressEnterToContinue();
            fRetitleConsole(strPrgNme);
            return;
        }
    }

    intMaxTitleID = fGetMaxTitleID();
    intMinTitleID = fGetMinTitleID();

    while(bExitAttributeChoice == false)
    {
        while((intTitleID < intMinTitleID) || (intTitleID > intMaxTitleID + 1))
        {
            printf("Title ID (%d - %d): ", intMinTitleID, intMaxTitleID);
            intTitleID = GetInt();
        }

        fRetitleConsole(strPrgNme);
        printf("\n");
        fShowTitle(intTitleID, strPrgNme);
        fShowAttributes(intTitleID);

        while((strstr("12345678910111213tTaAoOeEgGsScCrRdDfFmMxXhH", strAttribChoice) == NULL))
        {
            printf("\n");
            printf("Attribute Number (1-13): ");
            strAttribChoice = GetString();
            printf("\n");
        }

        if(strstr("1tT", strAttribChoice) != NULL)
        {
            printf("New Title Name or E(x)it: ");
            strTitleName = fEscapeSingleQuote(GetString());
            if((toupper(strTitleName[0]) == 'X') && (strlen(strTitleName) == 1))
            {
                strcpy(strAttribChoice, "00");
                bExitAttributeChoice == false;
                bAttributeQueryIsValid = false;
            }
            else
            {
                sprintf(strSQL, "UPDATE `Book Titles` SET `Title Name` = '%s' WHERE `Title ID` = %d", strTitleName, intTitleID);
                strcpy(strAttribChoice, "00");
                bAttributeQueryIsValid = true;
            }
        }
        else if(strstr("2aA", strAttribChoice) != NULL)
        {
            fShowAllAuthors(pintDisplayPageLength);
            printf("New Author ID: ");
            intNewAuthorID = GetInt();
            sprintf(strSQL, "UPDATE `Book Titles` SET `Author ID` = %d WHERE `Title ID` = %d", intNewAuthorID, intTitleID);
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("3Oo", strAttribChoice) != NULL)
        {
            fShowAllSources(pintDisplayPageLength);
            printf("New Source ID: ");
            intNewSourceID = GetInt();
            sprintf(strSQL, "UPDATE `Book Titles` SET `Source ID` = %d WHERE `Title ID` = %d", intNewSourceID, intTitleID);
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("4eE", strAttribChoice) != NULL)
        {
            fShowAllSeries(pintDisplayPageLength);
            printf("New Series ID: ");
            intNewSeriesID = GetInt();
            sprintf(strSQL, "UPDATE `Book Titles` SET `Series ID` = %d WHERE `Title ID` = %d", intNewSeriesID, intTitleID);
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("5gG", strAttribChoice) != NULL)
        {
            fShowAllGenres(pintDisplayPageLength);
            printf("New Genre ID: ");
            intNewGenreID = GetInt();
            sprintf(strSQL, "UPDATE `Book Titles` SET `Genre ID` = %d WHERE `Title ID` = %d", intNewGenreID, intTitleID);
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("6sS", strAttribChoice) != NULL)
        {
            fShowAllStatuses(pintDisplayPageLength);
            printf("New Status ID: ");
            intNewStatusID = GetInt();
            sprintf(strSQL, "UPDATE `Book Titles` SET `Status ID` = %d WHERE `Title ID` = %d", intNewStatusID, intTitleID);
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("7cC", strAttribChoice) != NULL)
        {
            fShowAllClassifications(pintDisplayPageLength);
            printf("New Classn ID: ");
            intNewClassnID = GetInt();
            sprintf(strSQL, "UPDATE `Book Titles` SET `Classification ID` = %d WHERE `Title ID` = %d", intNewClassnID, intTitleID);
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("8rR", strAttribChoice) != NULL)
        {
            fShowAllRatings(pintDisplayPageLength);
            printf("New Rating ID: ");
            intNewRatingID = GetInt();
            sprintf(strSQL, "UPDATE `Book Titles` SET `Rating ID` = %d WHERE `Title ID` = %d", intNewRatingID, intTitleID);
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("9dD", strAttribChoice) != NULL)
        {
            printf("New Start Date (YYYY-MM-DD): ");
            strNewStartDate = GetString();
            if(strlen(strNewStartDate) == 0)
            {
                sprintf(strSQL, "UPDATE `Book Titles` SET `Start` = NULL WHERE `Title ID` = %d", intTitleID);
            }
            else
            {
                sprintf(strSQL, "UPDATE `Book Titles` SET `Start` = '%s' WHERE `Title ID` = %d", strNewStartDate, intTitleID);
            }
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("10fF", strAttribChoice) != NULL)
        {
            printf("New Finish Date (YYYY-MM-DD): ");
            strNewFinishDate = GetString();
            if(strlen(strNewFinishDate) == 0)
            {
                sprintf(strSQL, "UPDATE `Book Titles` SET `Finish` = NULL WHERE `Title ID` = %d", intTitleID);
            }
            else
            {
                sprintf(strSQL, "UPDATE `Book Titles` SET `Finish` = '%s' WHERE `Title ID` = %d", strNewFinishDate, intTitleID);
            }
            bAttributeQueryIsValid = true;
            strcpy(strAttribChoice, "00");
        }
        else if(strstr("11mM", strAttribChoice) != NULL)
        {
            printf("New Comments or E(x)it: ");
            strNewComments = fEscapeSingleQuote(GetString());
            if((toupper(strNewComments[0]) == 'X') && (strlen(strNewComments) == 1))
            {
                strcpy(strAttribChoice, "00");
                bExitAttributeChoice == false;
                bAttributeQueryIsValid = false;
            }
            else
            {
                sprintf(strSQL, "UPDATE `Book Titles` SET `Comments` = '%s' WHERE `Title ID` = %d", strNewComments, intTitleID);
                free(strNewComments);
                strcpy(strAttribChoice, "00");
                bAttributeQueryIsValid = true;
            }
        }
        else if(strstr("12hH", strAttribChoice) != NULL)
        {
            printf("Show Characters: ");
            printf("\n\n");
            fShowCharacters(intTitleID, strPrgNme);
            sprintf(strSQL, "UPDATE `Book Titles` SET `Comments` = '%s' WHERE `Title ID` = %d", strNewComments, intTitleID);
            strcpy(strAttribChoice, "00");
            printf("\n");
            fPressEnterToContinue();
        }
        else if(strstr("13xX", strAttribChoice) != NULL)
        {
            bExitAttributeChoice = true;
        }

// execute the query and check for no result

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            return;
        }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            fRetitleConsole(strPrgNme);
            strcpy(strAttribChoice, "00");
        }
        fRetitleConsole(strPrgNme);
    }

    fRetitleConsole(strPrgNme);
    return;
}

int  fGetMaxTitleID(void)
{
    char strSQL[SQL_LEN] = {'\0'};
    char **endptr;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT MAX(`Title ID`) from `Book Titles`");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        (void) system("clear");
        printf("\n");

        return EXIT_FAILURE;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("\n");
        printf("Query failed to return a line of results in %s", __func__);
        printf("\n\n");
        printf("Count of rows returned: %d", mysql_field_count(conn));
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return EXIT_SUCCESS;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

    row = mysql_fetch_row(res);

    mysql_free_result(res);
    return(int) strtol(row[0], endptr, 10);
}

int fGetMinTitleID(void)
{
    char strSQL[SQL_LEN] = {'\0'};
    char **endptr;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT MIN(`Title ID`) from `Book Titles`");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_SUCCESS;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        (void) system("clear");
        printf("\n");

        return EXIT_SUCCESS;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("\n");
        printf("Query failed to return a line of results in %s", __func__);
        printf("\n\n");
        printf("Count of rows returned: %d", mysql_field_count(conn));
        printf("\n\n");
        fPressEnterToContinue();
        mysql_free_result(res);
        return EXIT_SUCCESS;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

    row = mysql_fetch_row(res);

    mysql_free_result(res);
    return(int) strtol(row[0], endptr, 10);
}

void fShowAttributes(int intTitleID)
{
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT `Title ID`"
                         ", `Title Name`"
                         ", BT.`Author ID`"
                         ", BA.`Author Name`"
                         ", BT.`Source ID`"
                         ", BS.`Source Name`"
                         ", BT.`Series ID`"
                         ", BE.`Series Name`"
                         ", BT.`Genre ID`"
                         ", BG.`Genre Name`"
                         ", BT.`Status ID`"
                         ", BU.`Status Name`"
                         ", BT.`Classification ID`"
                         ", BC.`Classification Name`"
                         ", BT.`Rating ID`"
                         ", BR.`Rating Name`"
                         ", BT.`Start`"
                         ", BT.`Finish`"
                         ", BT.`Comments`"
                         "FROM `Book Titles` BT LEFT JOIN `Book Authors` BA ON BT.`Author ID` = BA.`Author ID`"
                         "LEFT JOIN `Book Sources` BS ON BT.`Source ID` = BS.`Source ID`"
                         "LEFT JOIN `Book Series` BE ON BT.`Series ID` = BE.`Series ID`"
                         "LEFT JOIN `Book Genres` BG ON BT.`Genre ID` = BG.`Genre ID`"
                         "LEFT JOIN `Book Statuses` BU on BT.`Status ID` = BU.`Status ID`"
                         "LEFT JOIN `Book Classifications` BC ON BT.`Classification ID` = BC.`Classification ID`"
                         "LEFT JOIN `Book Ratings` BR ON BT.`Rating ID` = BR.`Rating ID`"
                         "WHERE `Title ID` = %d", intTitleID);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function\n\n%s(): %s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// print each row of results

    row = mysql_fetch_row(res);
    printf("\n");
    printf("Update a Title and Attributes");
    printf("\n\n");
    printf(" 1. (T)itle ID:  %3s  %s", row[0] ? row[0] : "", row[1] ? row[1] : "");
    printf("\n");
    printf(" 2. (A)uthor ID: %3s  %s", row[2] ? row[2] : "", row[3] ? row[3] : "");
    printf("\n");
    printf(" 3. S(o)urce ID: %3s  %s", row[4] ? row[4] : "", row[5] ? row[5] : "");
    printf("\n");
    printf(" 4. S(e)ries ID: %3s  %s", row[6] ? row[6] : "", row[7] ? row[7] : "");
    printf("\n");
    printf(" 5. (G)enre ID:  %3s  %s", row[8] ? row[8] : "", row[9] ? row[9] : "");
    printf("\n");
    printf(" 6. (S)tatus ID: %3s  %s", row[10] ? row[10] : "", row[11] ? row[11] : "");
    printf("\n");
    printf(" 7. (C)lassn ID: %3s  %s", row[12] ? row[12] : "", row[13] ? row[13] : "");
    printf("\n");
    printf(" 8. (R)ating ID: %3s  %s", row[14] ? row[14] : "", row[15] ? row[15] : "");
    printf("\n");
    printf(" 9. Start (D)te:      %s", row[16] ? row[16] : "");
    printf("\n");
    printf("10. (F)insh Dte:      %s", row[17] ? row[17] : "");
    printf("\n");
    printf("11. Co(m)ments:       %s", row[18] ? row[18] : "");
    printf("\n");
    printf("12. Show C(h)aracters");
    printf("\n");
    printf("13. E(x)it");
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowCharacters(int intTitleID, char *strPrgNme)
{
    int *intLengths = NULL;
    int intColCount = 0;
    int intRowsReturned = 0;
    int intRowsPrinted = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Character ID`, `Character Name` "
                    "FROM `Book Characters` "
                    "WHERE `Title ID` = %d", intTitleID);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;

        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("No characters in Title ID: %d", intTitleID);
        printf("\n");
    }

// fetch the max width of each column

    intLengths = (int *)calloc(intColCount, sizeof(int));

    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        for(int k = 0; k < intColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > intLengths[k])
                {
                     intLengths[k] = strlen(row[k]);
                }
            }
        }
    }

// reset the  pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// print each row of results

    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        for(int i = 0; i < intColCount; i++)
        {
            printf("%-*s", intLengths[i] + 1, row[i] ? row[i] : "");
        }
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fSearchTitles(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Title ID`, `Title Name` "
                    "FROM `Book Titles` "
                    "WHERE `Title Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
     }
 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching titles found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fShowAllSources(int *pintDisplayPageLength)
{
    int  intRowCount = 0;
    int  intMaxWidth = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT S.`Source ID`"
                         ", S.`Source Name` "
                         ", COUNT(T.`Title ID`)"
                   " FROM `Book Sources` S"
                   " LEFT JOIN `Book Titles` T ON S.`Source ID` = T.`Source ID`"
                   " GROUP BY S.`Source ID`, S.`Source Name`"
                   " ORDER BY `Source ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// find the maximum width of a Source Name

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Sources and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        printf("%3s  %-*s %3s", row[0], intMaxWidth, row[1], row[2]);
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            intRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllGenres(int *pintDisplayPageLength)
{
    int  intRowCount = 0;
    int  intMaxWidth = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT G.`Genre ID`"
                         ", G. `Genre Name` "
                         ", COUNT(T.`Title ID`)"
                   " FROM `Book Genres` G"
                   " LEFT JOIN `Book Titles` T ON G.`Genre ID` = T.`Genre ID`"
                   " GROUP BY G.`Genre ID`, G.`Genre Name`"
                   " ORDER BY `Genre ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// find the maximum width of a Genre Name

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Genres and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        printf("%3s  %-*s %3s", row[0], intMaxWidth, row[1], row[2]);
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            intRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllStatuses(int *pintDisplayPageLength)
{
    int  intRowCount = 0;
    int  intMaxWidth = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT S.`Status ID`"
                        ", S.`Status Name`"
                        ", COUNT(T.`Title ID`)"
                   " FROM `Book Statuses` S"
                   " LEFT JOIN `Book Titles` T ON S.`Status ID` = T.`Status ID`"
                   " GROUP BY S.`Status ID`, S.`Status Name`"
                   " ORDER BY `Status ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// find the maximum width of a Status Name

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Statuses and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        printf("%3s  %-*s %3s", row[0], intMaxWidth, row[1], row[2]);
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            intRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllClassifications(int *pintDisplayPageLength)
{
    int  intRowCount = 0;
    int  intMaxWidth = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT C.`Classification ID`"
                        " , C.`Classification Name`"
                        " , COUNT(T.`Title ID`)"
                   " FROM `Book Classifications` C"
                   " LEFT JOIN `Book Titles` T ON C.`Classification ID` = T.`Classification ID`"
                   " GROUP BY C.`Classification ID`, C.`Classification Name`"
                    "ORDER BY C.`Classification ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// get the maximum width of a Classification Name

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Classifications and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        printf("%3s  %-*s %3s", row[0], intMaxWidth, row[1], row[2]);
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            intRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllRatings(int  *pintDisplayPageLength)
{
    int  intRowCount = 0;
    int  intMaxWidth = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT R.`Rating ID`"
                        " , R.`Rating Name` "
                        " , COUNT(T.`Title ID`)"
                   " FROM `Book Ratings` R"
                   " LEFT JOIN `Book Titles` T ON R.`Rating ID` = T.`Rating ID`"
                   " GROUP BY R.`Rating ID`, R.`Rating Name`"
                   " ORDER BY `Rating ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// get the maximum width of a Rating Name

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Ratings and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        printf("%3s  %-*s %3s", row[0], intMaxWidth, row[1], row[2]);
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            intRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowMainMenu(void)
{
    printf("\n");
    printf("Main Menu");
    printf("\n\n");
    printf("1. (L)ist Titles");
    printf("\n");
    printf("2. List Autho(r)s");
    printf("\n");
    printf("3. Add a (T)itle");
    printf("\n");
    printf("4. Maintain (A)ttributes");
    printf("\n");
    printf("5. (U)pdate a Title and Attributes");
    printf("\n");
    printf("6. (C)haracters");
    printf("\n");
    printf("7. O(p)tions");
    printf("\n");
    printf("8. E(x)it");
    printf("\n\n");
    printf("Choice: ");

    return;
}

void fShowAttributesMenu(void)
{
    printf("\n");
    printf("Main Menu > Maintain Attributes");
    printf("\n\n");
    printf("1. (A)uthor");
    printf("\n");
    printf("2. (C)lassification");
    printf("\n");
    printf("3. (R)ating");
    printf("\n");
    printf("4. S(e)ries");
    printf("\n");
    printf("5. S(o)urces");
    printf("\n");
    printf("6. (S)tatuses");
    printf("\n");
    printf("7. (G)enres");
    printf("\n");
    printf("8. E(x)it");
    printf("\n\n");
    printf("Choice: ");
}

void fCharacters(char *strPrgNme)
{

    int intTitleID = 0;
    char charExitOrContinue = 'C';
    char *strCharacterToAdd = NULL;
    char *strCharacterToAddEscaped = NULL;
    char *strCharacterToChange = NULL;
    char *strCharacterToChangeEscaped = NULL;
    char *strTitleSearch = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    char charAddCharacterAction = 'X';
    bool bExitCharacters = false;
    bool bTitleFound = false;
    bool bValidChoice = false;
    char charAction = 'A';
    int  intCharID = 0;
    int  intCharacterMaxLength = 0;

    while(bTitleFound == false)
    {
        fRetitleConsole(strPrgNme);

        printf("\n");
        printf("Main Menu > Characters");
        printf("\n\n");

        printf("(T)itle ID, (S)earch or E(x)it: ");
        charAddCharacterAction = GetChar();

        if(toupper(charAddCharacterAction) == 'S')
        {
            printf("\n");
            printf("Search String: ");
            strTitleSearch = GetString();
            fSearchTitles(strTitleSearch);
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            charAddCharacterAction = 'T';
        }
        else if(toupper(charAddCharacterAction) == 'T')
        {
            printf("\n");
            printf("Title ID: ");
            intTitleID = GetInt();
            if(fCheckTitleIDExists(intTitleID) == true)
            {
                bTitleFound = true;
            }
            else
            {
                bTitleFound = false;
            }
            printf("\n");
        }
        else if(toupper(charAddCharacterAction) == 'X')
        {
            fRetitleConsole(strPrgNme);
            return;
        }
        else
        {
            printf("\n");
            printf("Not a valid choice so exiting");
            bTitleFound = true;
            printf("\n\n");
            fPressEnterToContinue();
            fRetitleConsole(strPrgNme);
            return;
        }
    }

    while(bExitCharacters == false)
    {
        while(bValidChoice == false)
        {

            fRetitleConsole(strPrgNme);
            printf("\n");
            printf("Main Menu > Characters");
            printf("\n\n");
            printf("Title ID: %d", intTitleID);
            printf("\n\n");

            fShowTitle(intTitleID, strPrgNme);
            printf("\n");
            fShowCharacters(intTitleID, strPrgNme);

            printf("\n");
            printf("(A)dd, (C)hange, (D)elete or E(x)it Characters: ");
            charAction = toupper(GetChar());
            if(strchr("xX", charAction) != NULL)
            {
                fRetitleConsole(strPrgNme);
                return;
            }
            else if(strchr("aA", charAction) != NULL)
            {
                bValidChoice = true;
                printf("\n");

                intCharacterMaxLength = fGetFieldLength("Book Characters", "Character Name");

                do{
                    printf("Character Name (max %d chars): ", intCharacterMaxLength);
                    strCharacterToAdd = GetString();
                    if(strlen(strCharacterToAdd) > intCharacterMaxLength)
                    {
                        printf("Character Name max %d chars. You entered %d chars", intCharacterMaxLength, (int) strlen(strCharacterToAdd));
                        printf("\n");
                        strCharacterToAdd[0] = '\0';
                    }
                } while(strlen(strCharacterToAdd) == 0);

                strCharacterToAddEscaped = fEscapeSingleQuote(strCharacterToAdd);
                
                sprintf(strSQL, "INSERT INTO `Book Characters` (`Character Name`, `Title ID`) VALUES('%s', %d)", strCharacterToAddEscaped, intTitleID);
                
                free(strCharacterToAddEscaped);
                free(strCharacterToAdd);
            }
                
            else if(strchr("cC", charAction) != NULL)
            {
                bValidChoice = true;
                printf("\n");

                intCharacterMaxLength = fGetFieldLength("Book Characters", "Character Name");

                printf("Character ID: ");
                intCharID = GetInt();
                printf("\n");

                do{
                    printf("Character Name (max %d chars): ", intCharacterMaxLength);
                    strCharacterToChange = GetString();
                    if(strlen(strCharacterToChange) > intCharacterMaxLength)
                    {
                        printf("Character Name max %d chars. You entered %d chars", intCharacterMaxLength, (int) strlen(strCharacterToChange));
                        printf("\n");
                        strCharacterToChange[0] = '\0';
                    }
                } while(strlen(strCharacterToChange) ==0);

                strCharacterToChangeEscaped = fEscapeSingleQuote(strCharacterToChange);
                sprintf(strSQL, "UPDATE `Book Characters` SET `Character Name` = '%s' WHERE `Character ID` = %d" , strCharacterToChangeEscaped, intCharID);
                free(strCharacterToChangeEscaped);
                free(strCharacterToChange);
            }
            else if(strchr("dD", charAction) != NULL)
            {
                bValidChoice = true;
                printf("\n");
                printf("Character ID: ");
                intCharID = GetInt();
                sprintf(strSQL, "DELETE FROM `Book Characters` WHERE `Character ID` = %d" , intCharID);
            }
            else
            {
                bValidChoice = false;
            }
        }

        printf("\n");

// execute the query and check for no result

        if(mysql_query(conn, strSQL) == 0)
        {
            if(charAction == 'A')
            {
                printf("Character Added.");
                printf("\n");
            }
            else if(charAction == 'C')
            {
                printf("Character Changed.");
                printf("\n");
            }
            else if(charAction == 'D')
            {
                printf("Character Deleted.");
                printf("\n");
            }
        }
        else
        {
            printf("Character not added - mysql_query failed");
        }

        printf("\n");
        printf("E(x)it or (C)ontinue): ");
        charExitOrContinue = GetChar();
        if(toupper(charExitOrContinue) == 'X')
        {
            bExitCharacters = true;
        }
        else if(toupper(charExitOrContinue) == 'C')
        {
            bExitCharacters = false;
            bValidChoice =  false;
        }
    }

    fRetitleConsole(strPrgNme);
    return;
}

void fShowTitle(int intTitleID, char *strPrgNme)
{
    int *intLengths = NULL;
    int intColCount = 0;
    int intRowsReturned = 0;
    int intRowsPrinted = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Title Name` "
                    "FROM `Book Titles` "
                    "WHERE `Title ID` = %d", intTitleID);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():i\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// reset the  pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("No Title Name for the Title ID: %d", intTitleID);
        printf("\n");
    }

// fetch the max width of each column

    intLengths = (int *)calloc(intColCount, sizeof(int));

    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        for(int k = 0; k < intColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > intLengths[k])
                {
                     intLengths[k] = strlen(row[k]);
                }
            }
        }
    }

// reset the  pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// print each row of results

    printf("%d ", intTitleID);

    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        for(int i = 0; i < intColCount; i++)
        {
            printf("%-*s", intLengths[i] + 1, row[i] ? row[i] : "");
        }
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

char *fEscapeSingleQuote(char *strQuery)
{

    int intQueryStrLen = 0;
    int i = 0;
    int j = 0;
    int k = 1;

    char *strTemp = NULL;

    strTemp = (char *)calloc(strlen(strQuery), sizeof(char));

    while(strQuery[i] != '\0')
    {
        strTemp[j] = strQuery[i];
        if(strTemp[j] == '\'')
        {
            strTemp = (char *)realloc(strTemp, strlen(strQuery) + k);
            j++;
            k++;
            strTemp[j] = '\'';
        };
        i++;
        j++;
    }
    strTemp = (char *)realloc(strTemp, strlen(strQuery) + k);
    strTemp[j] = '\0';                                                  // terminate the new string
    return(strTemp);
}

bool fCheckTitleIDExists(int intTitleID)
{
    int intRowsReturned = 0;
    char strSQL[SQL_LEN] = {'\0'};
    bool bTitleIDFound = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Title ID` "
                    "FROM `Book Titles` "
                    "WHERE `Title ID` = %d", intTitleID);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        bTitleIDFound = false;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");
        bTitleIDFound = false;
        fPressEnterToContinue();
        return EXIT_FAILURE;
    }

// reset the  pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No Match for Title ID: %d. ", intTitleID);
        printf("\n\n");
        fPressEnterToContinue();
        bTitleIDFound = false;
        printf("\n");
    }
    else
    {
        bTitleIDFound = true;
        printf("\n");
    }

    mysql_free_result(res);
    return bTitleIDFound;
}

void fShowAllAuthors(int *pintDisplayPageLength)
{
    int  intRowCount = 0;
    int  intMaxWidth = 0;                                // Maximum width of Name colum for printing
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;


//    sprintf(strSQL, "SELECT `Author ID`, `Author Name` "
//                    "FROM `Book Authors`"
//                    "ORDER BY `Author ID` ASC");

    sprintf(strSQL, "SELECT A.`Author ID`"
                         ", A.`Author Name` "
                         ", COUNT(T.`Title Name`) "
                    "FROM `Book Authors` A "
                    "LEFT JOIN `Book Titles` T ON A.`Author ID` = T.`Author ID` "
                    "GROUP BY A.`Author ID`, A.`Author Name` "
                    "ORDER BY `Author ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// find the maximum size of an author name

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Authors and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        printf("%3s  %-*s %3s", row[0], intMaxWidth, row[1], row[2]);
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            intRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllSeries(int *pintDisplayPageLength)
{
    int  intRowCount = 0;
    int  intMaxWidth = 0;
    char strSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT E.`Series ID`"
                        " , E.`Series Name` "
                        ",  COUNT(T.`Title ID`)"
                   " FROM `Book Series` E"
                   " LEFT JOIN `Book Titles` T ON E.`Series ID` = T.`Series ID`"
                   " GROUP BY E.`Series ID`, E.`Series Name`"
                   "ORDER BY `Series ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// reset the pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// find the maximimu width of a Series Name

    while(row = mysql_fetch_row(res))
    {
        if(strlen(row[1]) > intMaxWidth)
        {
            intMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Series and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        printf("%3s  %-*s %3s", row[0], intMaxWidth, row[1], row[2]);
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            intRowCount = 0;
        }
    }
    printf("\n");
    mysql_free_result(res);
    return;
}

void fListAuthors(char *strPrgNme, int *pintDisplayPageLength, char *pcharDisplayPageWidth, char *pCharDisplayOrder)
{
    int intColCount = 0;
    int *intLengths = NULL;
    int intRowCount = 0;
    char strOrder[6] = {'D', 'E', 'S', 'C', '\0'};
    char strSQL[SQL_LEN] = {'\0'};
    bool bEndOfPrintBlock = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    if(*pCharDisplayOrder == 'A')
    {
        strcpy(strOrder, "ASC");
    }
    else if(*pCharDisplayOrder == 'D')
    {
        strcpy(strOrder, "DESC");
    }

    sprintf(strSQL, "SELECT A.`Author ID`"
                    ", A.`Author Name` 'Author'"
                    ", COUNT(T.`Title Name`) 'Count'"
                    "FROM risingfast.`Book Authors` A "
                    "LEFT JOIN risingfast.`Book Titles` T ON A.`Author ID` = T.`Author ID`"
                    "GROUP BY A.`Author ID`"
                    "HAVING COUNT(T.`Title Name`) > 0;");

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > List Authors > Authors and Title Counts");
    printf("\n\n");

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// fetch the number of fields in the result

    intColCount = mysql_num_fields(res);

// fetch the max width of each column

    intLengths = (int *)calloc(intColCount, sizeof(int));

    while(row = mysql_fetch_row(res))
    {
        for(int k = 0; k < intColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > intLengths[k])
                {
                     intLengths[k] = strlen(row[k]);
                }
            }
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    while(row = mysql_fetch_row(res))
    {
        intRowCount++;
        bEndOfPrintBlock = false;
        for(int i = 0; i < intColCount; i++)
        if((i==0) || (i==2))
        {
            printf("%*s", intLengths[i] + 1, row[i] ? row[i] : "");
        }
        else
        {
            printf("  %-*s", intLengths[i] + 1, row[i] ? row[i] : "");
        }
        printf("\n");
        if(intRowCount >= *pintDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            bEndOfPrintBlock = true;
            printf("\n");
            intRowCount = 0;
        }
    }

    free(intLengths);

    printf("\n");
    if(bEndOfPrintBlock == false)
    {
        fPressEnterToContinue();
    }

    fRetitleConsole(strPrgNme);
    mysql_free_result(res);
    return;
}

void fSearchAuthors(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Author ID`, `Author Name` "
                    "FROM `Book Authors` "
                    "WHERE `Author Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
     }
 // store the result of the query

     res = mysql_store_result(conn);
     if(res == NULL)
     {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
     }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching authors found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteAuthor(char *strPrgNme)
{

    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intAuthorID = 0;
    bool bAuthorExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Authors > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Author ID: ");
    intAuthorID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(strSQL, "SELECT `Author ID` FROM `Book Authors` "
                    "WHERE `Author ID` = %d", intAuthorID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
     {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Author ID: %d", intAuthorID);
        printf("\n\n");
        fPressEnterToContinue();
        bAuthorExists = false;
    }
    else
    {
        bAuthorExists = true;
    }

    mysql_free_result(res);

// execute a query to check if the author has titles

    sprintf(strSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Author ID` = %d", intAuthorID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Author ID: %d", intAuthorID);
        printf("\n\n");
        printf("Author will not be deleted");
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        bTitlesExist = false;
    }

    mysql_free_result(res);

// execute a query to delete the author if the author exists and has no titles

    if((bTitlesExist == false) && (bAuthorExists == true))
    {
        sprintf(strSQL, "DELETE FROM `Book Authors` "
                        "WHERE `Author ID` = %d", intAuthorID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            return;
        }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            printf("fShowAttributes() -- SQL error");
            printf("\n");

        }

        printf("\n");
        printf("Author ID: %d deleted", intAuthorID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fSearchSeries(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Series ID`, `Series Name` "
                    "FROM `Book Series` "
                    "WHERE `Series Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
     }
 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;

     }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching series found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteSeries(char *strPrgNme)
{

    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intSeriesID = 0;
    bool bSeriesExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Series > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Series ID: ");
    intSeriesID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(strSQL, "SELECT `Series ID` FROM `Book Series` "
                    "WHERE `Series ID` = %d", intSeriesID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }

 // store the result of the query

     res = mysql_store_result(conn);
     if(res == NULL)
     {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
     }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Series ID: %d", intSeriesID);
        printf("\n\n");
        fPressEnterToContinue();
        bSeriesExists = false;
    }
    else
    {
        bSeriesExists = true;
    }

    mysql_free_result(res);

// execute a query to check if the author has titles

    sprintf(strSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Series ID` = %d", intSeriesID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
    }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Series ID: %d", intSeriesID);
        printf("\n\n");
        printf("Series will not be deleted");
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        bTitlesExist = false;
    }

    mysql_free_result(res);

// execute a query to delete the author if the author exists and has no titles

    if((bTitlesExist == false) && (bSeriesExists == true))
    {
        sprintf(strSQL, "DELETE FROM `Book Series` "
                        "WHERE `Series ID` = %d", intSeriesID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            printf("fShowAttributes() -- SQL error");
            printf("\n");

        }

        printf("\n");
        printf("Series ID: %d deleted", intSeriesID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fDeleteGenre(char *strPrgNme)
{

    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intGenreID = 0;
    bool bGenreExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Genre > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Genre ID: ");
    intGenreID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(strSQL, "SELECT `Genre ID` FROM `Book Genres` "
                    "WHERE `Genre ID` = %d", intGenreID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Genre ID: %d", intGenreID);
        printf("\n\n");
        fPressEnterToContinue();
        bGenreExists = false;
    }
    else
    {
        bGenreExists = true;
    }

    mysql_free_result(res);

// execute a query to check if the author has titles

    sprintf(strSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Genre ID` = %d", intGenreID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
    }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Genre ID: %d", intGenreID);
        printf("\n\n");
        printf("Genre will not be deleted");
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        bTitlesExist = false;
    }

    mysql_free_result(res);

// execute a query to delete the author if the author exists and has no titles

    if((bTitlesExist == false) && (bGenreExists == true))
    {
        sprintf(strSQL, "DELETE FROM `Book Genres` "
                        "WHERE `Genre ID` = %d", intGenreID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            return;
        }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            printf("fShowAttributes() -- SQL error");
            printf("\n");

        }

        printf("\n");
        printf("Genre ID: %d deleted", intGenreID);
        printf("\n\n");
        fPressEnterToContinue();
    }

    return;
}

void fSearchGenres(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Genre ID`, `Genre Name` "
                    "FROM `Book Genres` "
                    "WHERE `Genre Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Genre found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteSource(char *strPrgNme)
{

    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intSourceID = 0;
    bool bSourceExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Source > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Source ID: ");
    intSourceID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(strSQL, "SELECT `Source ID` FROM `Book Sources` "
                    "WHERE `Source ID` = %d", intSourceID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
     }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Source ID: %d", intSourceID);
        printf("\n\n");
        fPressEnterToContinue();
        bSourceExists = false;
    }
    else
    {
        bSourceExists = true;
    }

    mysql_free_result(res);

// execute a query to check if the author has titles

    sprintf(strSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Source ID` = %d", intSourceID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Source ID: %d", intSourceID);
        printf("\n\n");
        printf("Source will not be deleted");
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        bTitlesExist = false;
    }

    mysql_free_result(res);

// execute a query to delete the author if the author exists and has no titles

    if((bTitlesExist == false) && (bSourceExists == true))
    {
        sprintf(strSQL, "DELETE FROM `Book Sources` "
                        "WHERE `Source ID` = %d", intSourceID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();

            printf("fShowAttributes() -- SQL error");
            printf("\n");

        }

        printf("\n");
        printf("Source ID: %d deleted", intSourceID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fSearchSources(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Source ID`, `Source Name` "
                    "FROM `Book Sources` "
                    "WHERE `Source Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }
 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Sources found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteRating(char *strPrgNme)
{

    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intRatingID = 0;
    bool bRatingExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Rating > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Rating ID: ");
    intRatingID = GetInt();

// execute the query to check if the Rating ID exists

    sprintf(strSQL, "SELECT `Rating ID` FROM `Book Ratings` "
                    "WHERE `Rating ID` = %d", intRatingID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
    }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Rating ID: %d", intRatingID);
        printf("\n\n");
        fPressEnterToContinue();
        bRatingExists = false;
    }
    else
    {
        bRatingExists = true;
    }

    mysql_free_result(res);

// execute a query to check if the rating has titles

    sprintf(strSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Rating ID` = %d", intRatingID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
    }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Rating ID: %d", intRatingID);
        printf("\n\n");
        printf("Rating will not be deleted");
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        bTitlesExist = false;
    }

    mysql_free_result(res);

// execute a query to delete the author if the author exists and has no titles

    if((bTitlesExist == false) && (bRatingExists == true))
    {
        sprintf(strSQL, "DELETE FROM `Book Ratings` "
                        "WHERE `Rating ID` = %d", intRatingID);
        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            printf("fShowAttributes() -- SQL error");
            printf("\n");

        }

        printf("\n");
        printf("Rating ID: %d deleted", intRatingID);
        printf("\n\n");
        fPressEnterToContinue();
    }

    return;
}

void fSearchRatings(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Rating ID`, `Rating Name` "
                    "FROM `Book Ratings` "
                    "WHERE `Rating Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
     }
 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Ratings found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteStatus(char *strPrgNme)
{

    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intStatusID = 0;
    bool bStatusExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Status > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Status ID: ");
    intStatusID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(strSQL, "SELECT `Status ID` FROM `Book Statuses` "
                    "WHERE `Status ID` = %d", intStatusID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Status ID: %d", intStatusID);
        printf("\n\n");
        fPressEnterToContinue();
        bStatusExists = false;
    }
    else
    {
        bStatusExists = true;
    }

    mysql_free_result(res);

// execute a query to check if the author has titles

    sprintf(strSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Status ID` = %d", intStatusID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Status ID: %d", intStatusID);
        printf("\n\n");
        printf("Status will not be deleted");
        printf("\n\n");
        fPressEnterToContinue();
        while(getchar() != '\n')
        {
            continue;
        }
    }
    else
    {
        bTitlesExist = false;
    }

    mysql_free_result(res);

// execute a query to delete the author if the author exists and has no titles

    if((bTitlesExist == false) && (bStatusExists == true))
    {
        sprintf(strSQL, "DELETE FROM `Book Statuses` "
                        "WHERE `Status ID` = %d", intStatusID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            printf("fShowAttributes() -- SQL error");
            printf("\n");
        }

        printf("\n");
        printf("Status ID: %d deleted", intStatusID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fSearchStatuses(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Status ID`, `Status Name` "
                    "FROM `Book Statuses` "
                    "WHERE `Status Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");

        return;
     }
 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Status found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteClassification(char *strPrgNme)
{

    char *strServer = "192.168.0.13";
    char *strUser = "gjarman";
    char *strPassword = "Mpa4egu$";
    char *strDatabase = "risingfast";
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intClassificationID = 0;
    bool bClassificationExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Classifications > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Classification ID: ");
    intClassificationID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(strSQL, "SELECT `Classification ID` FROM `Book Classifications` "
                    "WHERE `Classification ID` = %d", intClassificationID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
    }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Classification ID: %d", intClassificationID);
        printf("\n\n");
        fPressEnterToContinue();
        bClassificationExists = false;
    }
    else
    {
        bClassificationExists = true;
    }

    mysql_free_result(res);

// execute a query to check if the author has titles

    sprintf(strSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Classification ID` = %d", intClassificationID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();

        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Classification ID: %d", intClassificationID);
        printf("\n\n");
        printf("Classification will not be deleted");
        printf("\n\n");
        fPressEnterToContinue();
    }
    else
    {
        bTitlesExist = false;
    }

    mysql_free_result(res);

// execute a query to delete the author if the author exists and has no titles

    if((bTitlesExist == false) && (bClassificationExists == true))
    {
        sprintf(strSQL, "DELETE FROM `Book Classifications` "
                        "WHERE `Classification ID` = %d", intClassificationID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        }

// test for a SQL error

        if(*mysql_error(conn))
        {
            printf("\n");
            printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
            printf("fShowAttributes() -- SQL error");
            printf("\n");
        }

        printf("\n");
        printf("Classification ID: %d deleted", intClassificationID);
        printf("\n\n");
        fPressEnterToContinue();
    }

    return;
}

void fSearchClassifications(char *strSearchString)
{
    char strSQL[SQL_LEN] = {'\0'};
    int  intColCount = 0;
    int  intRowsReturned = 0;
    int  intRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT  `Classification ID`, `Classification Name` "
                    "FROM `Book Classifications` "
                    "WHERE `Classification Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();

        printf("fShowAttributes() -- SQL error");
        printf("\n");
        return;
     }
 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intColCount = mysql_num_fields(res);
    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Classification found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (intRowsReturned != 0))
    {
        intRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fUpdateAuthor(char *strPrgNme)
{
    char *strAuthorName = NULL;
    char *strEscapedAuthorName = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intAuthorID = 0;
    int  intAuthorMaxLength = 0;
    bool bAuthorExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    intAuthorMaxLength = fGetFieldLength("Book Authors", "Author Name");

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Authors > Change");
    printf("\n\n");

// get an Author ID to change

    printf("Author ID: ");
    intAuthorID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(strSQL, "SELECT `Author ID`"
                         ", `Author Name`"
                    "FROM `Book Authors` "
                    "WHERE `Author ID` = %d", intAuthorID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

    if(*mysql_error(conn))
    {
       printf("\n");
       printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
       printf("\n\n");
       fPressEnterToContinue();
       printf("%s() -- SQL error", __func__);
       printf("\n");

       return;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        mysql_free_result(res);
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Author ID: %d", intAuthorID);
        printf("\n\n");
        fPressEnterToContinue();
        bAuthorExists = false;
    }
    else
    {
        bAuthorExists = true;
        row = mysql_fetch_row(res);
        printf("\n");
        printf("Author Name: %s", row[1]);
        printf("\n\n");
        do {
            printf("Changed Author Name (max %d chars) or E(x)it: ", intAuthorMaxLength);
            strAuthorName = fEscapeSingleQuote(GetString());
            if(strlen(strAuthorName) > intAuthorMaxLength)
            {
                printf("Author Name max %d chars. You entered %d chars", intAuthorMaxLength, (int) strlen(strAuthorName));
                printf("\n");
                strAuthorName[0] = '\0';
            }
        } while (strlen(strAuthorName) == 0);
    }

    if((bAuthorExists == true) && (toupper(strAuthorName[0]) == 'X') && (strlen(strAuthorName) == 1))
    {
        free(strAuthorName);
        return;
    }

// execute a query to change the author if the author exists

    if((bTitlesExist == false) && (bAuthorExists == true))
    {
        sprintf(strSQL, "UPDATE `Book Authors`"
                       " SET `Author Name` = '%s'"
                       " WHERE `Author ID` = %d", strAuthorName, intAuthorID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Author ID: %d updated", intAuthorID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    free(strAuthorName);
    mysql_free_result(res);
    return;
}

void fUpdateClassification(char *strPrgNme)
{
    char *strClassificationName = NULL;
    char *strClassificationNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intClassificationID = 0;
    int  intClassificationMaxLength = 0;
    bool bClassificationExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Classifications > Change");
    printf("\n\n");

// get an Classification ID to delete

    printf("Classification ID: ");
    intClassificationID = GetInt();

// execute the query to check if the Classification ID exists

    sprintf(strSQL, "SELECT `Classification ID`"
                         ", `Classification Name`"
                    "FROM `Book Classifications` "
                    "WHERE `Classification ID` = %d", intClassificationID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("%s() -- SQL error", __func__);
        printf("\n");

        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Classification ID: %d", intClassificationID);
        printf("\n\n");
        fPressEnterToContinue();
        bClassificationExists = false;
    }
    else
    {
        bClassificationExists = true;
        row = mysql_fetch_row(res);
        printf("\n");
        printf("Classification Name: %s", row[1]);
        printf("\n\n");

        intClassificationMaxLength = fGetFieldLength("Book Classifications", "Classification Name");

        do {
            printf("Changed Classification Name (max %d chars): ", intClassificationMaxLength);
            strClassificationName = GetString();
            if(strlen(strClassificationName) > intClassificationMaxLength)
            {
                 printf("Classification Name max %d chars. You entered %d chars", intClassificationMaxLength, (int) strlen(strClassificationName));
                 printf("\n");
                 strClassificationName[0] = '\0';
            }
        } while(strlen(strClassificationName) == 0);
        
        strClassificationNameEscaped = fEscapeSingleQuote(strClassificationName);
    }

    if((bClassificationExists == true) && (toupper(strClassificationName[0]) == 'X') && (strlen(strClassificationName) == 1))
    {
        free(strClassificationName);
        return;
    }

// execute a query to change the classification if the classification exists

    if((bTitlesExist == false) && (bClassificationExists == true))
    {
        sprintf(strSQL, "UPDATE `Book Classifications`"
                       " SET `Classification Name` = '%s'"
                       " WHERE `Classification ID` = %d", strClassificationNameEscaped, intClassificationID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Classification ID: %d updated", intClassificationID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(strClassificationName);
    free(strClassificationNameEscaped);
    return;
}

void fUpdateRating(char *strPrgNme)
{
    char *strRatingName = NULL;
    char *strRatingNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intRatingID = 0;
    int  intRatingMaxLength = 0;
    bool bRatingExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Rating > Change");
    printf("\n\n");

// get an Rating ID to delete

    printf("Rating ID: ");
    intRatingID = GetInt();

// execute the query to check if the Rating ID exists

    sprintf(strSQL, "SELECT `Rating ID`"
                         ", `Rating Name`"
                    "FROM `Book Ratings` "
                    "WHERE `Rating ID` = %d", intRatingID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("%s() -- SQL error", __func__);
        printf("\n");

        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Rating ID: %d", intRatingID);
        printf("\n\n");
        fPressEnterToContinue();
        bRatingExists = false;
    }
    else
    {
        bRatingExists = true;
        row = mysql_fetch_row(res);
        printf("\n");
        printf("Rating Name: %s", row[1]);
        printf("\n\n");

        intRatingMaxLength = fGetFieldLength("Book Ratings", "Rating Name");

        do {
            printf("Changed Rating Name (max %d chars) or E(x)it: ", intRatingMaxLength);
            strRatingName = GetString();
            if(strlen(strRatingName) > intRatingMaxLength)
            {
                 printf("Rating Name max %d chars. You entered %d chars", intRatingMaxLength, (int) strlen(strRatingName));
                 printf("\n");
                 strRatingName[0] = '\0';
            }
        } while(strlen(strRatingName) == 0);

        strRatingNameEscaped = fEscapeSingleQuote(strRatingName);

    }

    if((bRatingExists == true) && (toupper(strRatingName[0]) == 'X') && (strlen(strRatingName) == 1))
    {
        free(strRatingName);
        return;
    }


// execute a query to change the rating if the rating exists

    if((bTitlesExist == false) && (bRatingExists == true))
    {
        sprintf(strSQL, "UPDATE `Book Ratings`"
                       " SET `Rating Name` = '%s'"
                       " WHERE `Rating ID` = %d", strRatingNameEscaped, intRatingID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Rating ID: %d updated", intRatingID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(strRatingName);
    free(strRatingNameEscaped);
    return;
}

void fUpdateSeries(char *strPrgNme)
{
    char *strSeriesName = NULL;
    char *strSeriesNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intSeriesID = 0;
    int  intSeriesMaxLength = 0;
    bool bSeriesExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Series > Change");
    printf("\n\n");

// get an Series ID to delete

    printf("Series ID: ");
    intSeriesID = GetInt();

// execute the query to check if the Series ID exists

    sprintf(strSQL, "SELECT `Series ID`"
                         ", `Series Name`"
                    "FROM `Book Series` "
                    "WHERE `Series ID` = %d", intSeriesID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("%s() -- SQL error", __func__);
        printf("\n");

        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Series ID: %d", intSeriesID);
        printf("\n\n");
        fPressEnterToContinue();
        bSeriesExists = false;
    }
    else
    {
        bSeriesExists = true;
        row = mysql_fetch_row(res);
        printf("\n");
        printf("Series Name: %s", row[1]);
        printf("\n\n");

        intSeriesMaxLength = fGetFieldLength("Book Series", "Series Name");

        do {
            printf("Changed Series Name (max %d chars) or E(x)it: ", intSeriesMaxLength);
            strSeriesName = GetString();
            if (strlen(strSeriesName) > intSeriesMaxLength)
            {
                printf("Series Name max %d chars. You entered %d chars", intSeriesMaxLength, (int) strlen(strSeriesName));
                printf("\n");
                strSeriesName[0] = '\0';
            }
        } while(strlen(strSeriesName) == 0);

        strSeriesNameEscaped = fEscapeSingleQuote(strSeriesName);

    }

    if((bSeriesExists == true) && (toupper(strSeriesName[0]) == 'X') && (strlen(strSeriesName) == 1))
    {
        free(strSeriesName);
        return;
    }

// execute a query to change the series if the series exists

    if((bTitlesExist == false) && (bSeriesExists == true))
    {
        sprintf(strSQL, "UPDATE `Book Series`"
                       " SET `Series Name` = '%s'"
                       " WHERE `Series ID` = %d", strSeriesNameEscaped, intSeriesID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Series ID: %d updated", intSeriesID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(strSeriesName);
    free(strSeriesNameEscaped);
    return;
}

void fUpdateSource(char *strPrgNme)
{
    char *strSourceName = NULL;
    char *strSourceNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intSourceID = 0;
    int  intSourceMaxLength = 0;
    bool bSourceExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Source > Change");
    printf("\n\n");

// get an Source ID to delete

    printf("Source ID: ");
    intSourceID = GetInt();

// execute the query to check if the Source ID exists

    sprintf(strSQL, "SELECT `Source ID`"
                         ", `Source Name`"
                    "FROM `Book Sources` "
                    "WHERE `Source ID` = %d", intSourceID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("%s() -- SQL error", __func__);
        printf("\n");

        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Sourc ID: %d", intSourceID);
        printf("\n\n");
        fPressEnterToContinue();
        bSourceExists = false;
    }
    else
    {
        bSourceExists = true;
        row = mysql_fetch_row(res);
        printf("\n");
        printf("Source Name: %s", row[1]);
        printf("\n\n");

        intSourceMaxLength = fGetFieldLength("Book Sources", "Source Name");

        do {
            printf("Changed Source Name (max %d chars) or E(x)it: ", intSourceMaxLength);
            strSourceName = GetString();
            if (strlen(strSourceName) > intSourceMaxLength)
            {
                 printf("Source Name max %d chars. You entered %d chars", intSourceMaxLength, (int) strlen(strSourceName));
                 printf("\n");
                 strSourceName[0] = '\0';
            }
        } while(strlen(strSourceName) == 0);

        strSourceNameEscaped = fEscapeSingleQuote(strSourceName);

    }

    if((bSourceExists == true) && (toupper(strSourceName[0]) == 'X') && (strlen(strSourceName) == 1))
    {
        free(strSourceName);
        return;
    }

// execute a query to change the source if the source exists

    if((bTitlesExist == false) && (bSourceExists == true))
    {
        sprintf(strSQL, "UPDATE `Book Sources`"
                       " SET `Source Name` = '%s'"
                       " WHERE `Source ID` = %d", strSourceNameEscaped, intSourceID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            printf("Press enter to continue ");
        } else
        {
            printf("\n");
            printf("Source ID: %d updated", intSourceID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(strSourceName);
    free(strSourceNameEscaped);
    return;
}

void fUpdateStatus(char *strPrgNme)
{
    char *strStatusName = NULL;
    char *strStatusNameEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intStatusID = 0;
    int  intStatusMaxLength = 0;
    bool bStatusExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Status > Change");
    printf("\n\n");

// get an Status ID to delete

    printf("Status ID: ");
    intStatusID = GetInt();

// execute the query to check if the Status ID exists

    sprintf(strSQL, "SELECT `Status ID`"
                         ", `Status Name`"
                    "FROM `Book Statuses` "
                    "WHERE `Status ID` = %d", intStatusID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("%s() -- SQL error", __func__);
        printf("\n");

        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");

        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Status ID: %d", intStatusID);
        printf("\n\n");
        fPressEnterToContinue();
        bStatusExists = false;
    }
    else
    {
        bStatusExists = true;
        row = mysql_fetch_row(res);
        printf("\n");
        printf("Status Name: %s", row[1]);
        printf("\n\n");
        
        intStatusMaxLength = fGetFieldLength("Book Statuses", "Status Name");

        do {
            printf("Changed Status Name (max %d chars): ", intStatusMaxLength);
            strStatusName = GetString();
            if (strlen(strStatusName) > intStatusMaxLength)
            {
                 printf("Status Name max %d chars. You entered %d chars", intStatusMaxLength, (int) strlen(strStatusName));
                 printf("\n");
                 strStatusName[0] = '\0';
            }
        } while(strlen(strStatusName) == 0);

        strStatusNameEscaped = fEscapeSingleQuote(strStatusName);
    }

    if((bStatusExists == true) && (toupper(strStatusName[0]) == 'X') && (strlen(strStatusName) == 1))
    {
        free(strStatusName);
        return;
    }

// execute a query to change the Status if the status exists

    if((bTitlesExist == false) && (bStatusExists == true))
    {
        sprintf(strSQL, "UPDATE `Book Statuses`"
                       " SET `Status Name` = '%s'"
                       " WHERE `Status ID` = %d", strStatusNameEscaped, intStatusID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            printf("Press enter to continue ");
        } else
        {
            printf("\n");
            printf("Status ID: %d updated", intStatusID);
            printf("\n\n");
            printf("Press enter to continue ");
        }
    }

    mysql_free_result(res);
    free(strStatusName);
    free(strStatusNameEscaped);
    return;
}

void fUpdateGenre(char *strPrgNme)
{
    char *strGenreName = NULL;
    char *strGenreDesc = NULL;
    char *strGenreNameEscaped = NULL;
    char *strGenreDescEscaped = NULL;
    char strSQL[SQL_LEN] = {'\0'};
    int  intRowsReturned = 0;
    int  intGenreID = 0;
    int  intGenreNameMaxLength = 0;
    int  intGenreDescMaxLength = 0;
    bool bGenreExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(strPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Genres > Change");
    printf("\n\n");

// get an Genre ID to delete

    printf("Genre ID: ");
    intGenreID = GetInt();

// execute the query to check if the Genre ID exists

    sprintf(strSQL, "SELECT `Genre ID`"
                         ", `Genre Name`"
                         ", `Genre Description`"
                    "FROM `Book Genres` "
                    "WHERE `Genre ID` = %d", intGenreID);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return;
    }

// test for a SQL error

     if(*mysql_error(conn))
     {
        printf("\n");
        printf("SQL Error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        printf("%s() -- SQL error", __func__);
        printf("\n");

        return;
     }

 // store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- No results returned", __func__);
        printf("\n");
        fPressEnterToContinue();
        return;
    }

// fetch the number of fields and rows in the result

    intRowsReturned = mysql_num_rows(res);

    if(intRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Genre ID: %d", intGenreID);
        printf("\n\n");
        fPressEnterToContinue();
        bGenreExists = false;
    }
    else
    {
        bGenreExists = true;
        row = mysql_fetch_row(res);
        printf("\n");
        printf("Genre Name: %s", row[1]);
        printf("\n\n");
        printf("Genre Desc: %s", row[2]);
        printf("\n\n");

        intGenreNameMaxLength = fGetFieldLength("Book Genres", "Genre Name");
        intGenreDescMaxLength = fGetFieldLength("Book Genres", "Genre Description");

        do {
            printf("Changed Genre Name (max %d chars) or E(x)it: ", intGenreNameMaxLength);
            strGenreName = GetString();
            if (strlen(strGenreName) > intGenreNameMaxLength)
            {
                 printf("Genre Name max %d chars. You entered %d chars", intGenreNameMaxLength, (int) strlen(strGenreName));
                 printf("\n");
                 strGenreName[0] = '\0';
            }
        } while(strlen(strGenreName) == 0);

        strGenreNameEscaped = fEscapeSingleQuote(strGenreName);

        if((toupper(strGenreName[0]) == 'X') && (strlen(strGenreName) == 1))
        {
            free(strGenreName);
            return;
        }

        do{
            printf("\n");
            printf("Changed Genre Description (max %d chars) or E(x)it: ", intGenreDescMaxLength);
            strGenreDesc = GetString();
            if (strlen(strGenreDesc) > intGenreDescMaxLength)
            {
                printf("Genre Desc max %d chars. You entered %d chars", intGenreDescMaxLength, (int) strlen(strGenreDesc));
                printf("\n");
                strGenreDesc[0] = '\0';
            }
        } while(strlen(strGenreDesc) == 0);

        strGenreDescEscaped = fEscapeSingleQuote(strGenreDesc);

        if((bGenreExists == true) && (toupper(strGenreDesc[0]) == 'X') && (strlen(strGenreDesc) == 1))
        {
            free(strGenreDesc);
            return;
        }

    }

// execute a query to change the genre if the genre exists

    if((bTitlesExist == false) && (bGenreExists == true))
    {
        sprintf(strSQL, "UPDATE `Book Genres`"
                       " SET `Genre Name` = '%s'"
                      ", `Genre Description` = '%s'"
                       " WHERE `Genre ID` = %d", strGenreNameEscaped, strGenreDescEscaped, intGenreID);

        if(mysql_query(conn, strSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Genre ID: %d updated", intGenreID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(strGenreName);
    free(strGenreNameEscaped);
    return;
}

int  fGetFieldLength(char *strTableName, char *strFieldName)
{
    char strSQL[SQL_LEN] = {'\0'};
    int intMaxFieldLength = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(strSQL, "SELECT CHARACTER_MAXIMUM_LENGTH"
                    " FROM information_schema.COLUMNS"
                    " WHERE TABLE_SCHEMA ='risingfast'"
                    " AND TABLE_NAME = '%s'"
                    " AND COLUMN_NAME = '%s'", strTableName, strFieldName);

// execute the query and check for no result

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        fPressEnterToContinue();
        return EXIT_SUCCESS;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");
        fPressEnterToContinue();
        return EXIT_SUCCESS;
    }

    row = mysql_fetch_row(res);
    intMaxFieldLength = (int) atol(row[0]);

    mysql_free_result(res);
    return intMaxFieldLength;
}

void fGetPwdFromConsole(void)
{
    char *sEnteredPwd = NULL;;

    printf("\n");
    do
    {
        printf("Password to connect to mysqlDB (or E(x)it): ");
        sEnteredPwd = GetString();
        if((strlen(sEnteredPwd) == 1) && (strchr("xX", sEnteredPwd[0]) != NULL))
        {
            strcpy(sgPassword, "BadSoExit");
            break;
        }
        else
        {
            conn = mysql_init(NULL);

            if (!mysql_real_connect(conn, sgServer, sgUsername, sEnteredPwd, sgDatabase, 0, NULL, 0))
            {
                printf("\n");
                printf("Failed to connect to MySQL server with entered password");
                printf("\n\n");
                fPressEnterToContinue();
                printf("\n");
            }
            else
            {
                strcpy(sgPassword, sEnteredPwd);
            }
        }
    } while(strcmp(sgPassword, sEnteredPwd) != 0);

    return;
}
