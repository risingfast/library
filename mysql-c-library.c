// includes and defines

#define SQL_LEN 5000
#define _GNU_SOURCE

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shared/cs50.h"
#include "../shared/rf50.h"
#include <ctype.h>
#include <stdbool.h>

void fListTitles(char *, int *, char *, char *, char *);                     //show all titles & authors on the console
void fListAuthors(char *, int *, char *, char *);                    //show all authors and title counts on the console
void fMaintainAttributes(char *, int *);                                                   // maintain attribute tables
void fSetOptions(char *, int *, char *, char *, char *);                                    // main menu to set options
int  fAddSeries(char *);                              // add a series, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddAuthor(char *);                             // add an author, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddClassification(char *);                    // add a classifn, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddRating(char *);                              // add a rating, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddSource(char *);                              // add a source, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddStatus(char *);                              // add a status, return EXIT_FAILURE if not added or EXIT_SUCCESS
int  fAddGenre(char *);                                // add a genre, return EXIT_FAILURE if not added or EXIT_SUCCESS
void fAddTitle(char *);                                                          // add a title with default attributes
void fCharacters(char *);                                             // view, add, change delete characters on a title
int  fGetMaxTitleID(void);                                                           // get the max value of Title ID's
int  fGetMinTitleID(void);                                                           // get the min value of Title ID's
void fShowAttributes(int);                                                      // show title attributes on the console
void fShowCharacters(int, char *);                                                    // show characters on the console
void fShowAllAuthors(int *);                                                         // show all authors on the console
void fShowAllSeries(int *);                                                           // show all series on the console
void fShowAllSources(int *);                                                         // show all sources on the console
void fShowAllGenres(int *);                                                           // show all genres on the console
void fShowAllStatuses(int *);                                                       // show all statuses on the console
void fShowAllClassifications(int *);                                         // show all classifications on the console
void fShowAllRatings(int *);                                                         // show all ratings on the console
void fShowMainMenu(void);                                                                         // show the main menu
void fShowAttributesMenu(void);                                                                   // show the main menu
void fShowTitle(int, char *);                                                       // show a title name on the console
char *fEscapeSingleQuote(char *);                                        // escape ' as '' for mysql in string literals
bool fCheckTitleIDExists(int);                                            // check a Title ID and return True if exists
void fDeleteAuthor(char *);                                                          // delete an author with no titles
void fDeleteSeries(char *);                                                           // delete a series with no titles
void fDeleteGenre(char *);                                                             // delete a genre with no titles
void fDeleteSource(char *);                                                           // delete a source with no titles
void fDeleteRating(char *);                                                           // delete a rating with no titles
void fDeleteStatus(char *);                                                           // delete a status with no titles
void fDeleteClassification(char *);                                           // delete a classification with no titles
void fSearchAuthors(char *);                                                    // search authors and show an author ID
void fSearchTitles(char *);                                                        // search titles and show a title ID
void fSearchSeries(char *);                                                       // search series and show a series ID
void fSearchGenres(char *);                                                        // search genres and show a genre ID
void fSearchSources(char *);                                                     // search sources and show a source ID
void fSearchRatings(char *);                                                     // search ratings and show a rating ID
void fSearchStatuses(char *);                                                   // search statuses and show a status ID
void fSearchClassifications(char *);                             // search classifications and show a classification ID
void fUpdateTitleAndAttributes(char *, int *);                       // update attributes of a Title but not characters
void fUpdateAuthor(char *);                                                                         // update an author
void fUpdateClassification(char *);                                                          // update a classification
void fUpdateRating(char *);                                                                          // update a rating
void fUpdateSeries(char *);                                                                          // update a series
void fUpdateSource(char *);                                                                          // update a source
void fUpdateStatus(char *);                                                                          // update a status
void fUpdateGenre(char *);                                                                           // update an genre
bool fTestDbConnection(void);                                                        // test connection to the database
int  fGetFieldLength(char *, char *);                                 // get the max field length of a field in a table
void fGetPwdFromConsole(void);                                                       // get a password from the console

// global declarations

char *sgServer = "192.168.0.13";                                                               //mysqlServer IP address
char *sgUsername = "gjarman";                                                              // mysqlSerer logon username
char sgPassword[20] = {'\0'};                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                // default database name on mysqlserver

MYSQL *conn;

int main(int argc, char** argv)
{
// declarations

    char *sPrgNme = strcat(argv[0] + 2, " -- Library and reading log");                                 // program name
    bool bHelp = false;                                                                  // help flag to show help text
    bool bExitMain = false;                                                                 // flag to exit the program
    char cMainChoice = '0';                                                                         // main menu choice
    int  iDisplayPageLength = 83;                                                      // display page lines for paging
    char cDisplayPageWidth = 'N';                                                  // display page width wide or narrow
    char cDipslayPageFormat = 'T';                                                // display page format (T)ab or (C)SV
    char cDisplayOrder = 'A';                                                  // display order asc or desc on Title ID

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

    fRetitleConsole(sPrgNme);
    fGetPwdFromConsole();
    if(strcmp("BadSoExit", sgPassword) == 0)
    {
        printf("\n");
        return EXIT_FAILURE;
    }
    fRetitleConsole(sPrgNme);

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
        return  EXIT_FAILURE;
    }

// print the main menu

    while(bExitMain == false)
    {
        while(cMainChoice == '0')
        {
            fRetitleConsole(sPrgNme);
            fShowMainMenu();
            cMainChoice = GetChar();
            if(strchr("12345678xXcCpPuUaAlLtTrR", cMainChoice) == NULL)
            {
                cMainChoice = '0';
                fRetitleConsole(sPrgNme);
                printf("\n");
            }
        }

        if(strchr("1lL", cMainChoice) != NULL)
        {
            printf("\n");
            fListTitles(sPrgNme, &iDisplayPageLength, &cDisplayPageWidth, &cDipslayPageFormat, &cDisplayOrder);
            printf("\n");
            cMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("2rR", cMainChoice) != NULL)
        {
            printf("\n");
            fListAuthors(sPrgNme, &iDisplayPageLength, &cDisplayPageWidth, &cDisplayOrder);
            printf("\n");
            cMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("3tT", cMainChoice) != NULL)
        {
            printf("\n");
            fAddTitle(sPrgNme);
            printf("\n");
            cMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("aA4", cMainChoice) != NULL)
        {
            printf("\n");
            fMaintainAttributes(sPrgNme, &iDisplayPageLength);
            printf("\n");
            cMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("uU5", cMainChoice) != NULL)
        {
            printf("\n");
            fUpdateTitleAndAttributes(sPrgNme, &iDisplayPageLength);
            printf("\n");
            cMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("cC6", cMainChoice) != NULL)
        {
            printf("\n");
            fCharacters(sPrgNme);
            printf("\n");
            cMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("pP7", cMainChoice) != NULL)
        {
            printf("\n");
            fSetOptions(sPrgNme, &iDisplayPageLength, &cDisplayPageWidth, &cDipslayPageFormat, &cDisplayOrder);
            printf("\n");
            cMainChoice = '0';
            bExitMain = false;
        }
        else if(strchr("xX8", cMainChoice) != NULL)
        {
            bExitMain = true;
        }
    }

    mysql_close(conn);
    system("clear");
    return EXIT_SUCCESS;
}

void fListTitles(char *sPrgNme, int *piDisplayPageLength, char *pcDisplayPageWidth, char *pcDipslayPageFormat, char *pcDisplayOrder)
{
    int iColCount = 0;
    int *iLengths = NULL;
    int iRowCount = 0;
    int  iStatusFilter = 0;
    char cStatusFilterchoice = '0';
    char caOrder[6] = {'D', 'E', 'S', 'C', '\0'};
    char caSQL[SQL_LEN] = {'\0'};
    bool bEndOfPrintBlock = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    if(*pcDisplayOrder == 'A')
    {
        strcpy(caOrder, "ASC");
    }
    else if(*pcDisplayOrder == 'D')
    {
        strcpy(caOrder, "DESC");
    }

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > List Titles > Titles and Authors and Character Counts");
    printf("\n\n");
    printf("(A)ll, (U)nassigned, (N)ot Started, (S)tarted, (P)aused, (F)inished or E(x)it");
    printf("\n\n");
    while(strchr("aAuUnNsSfFpPxX", cStatusFilterchoice) == NULL)
    {
        printf("Choice: ");
        cStatusFilterchoice = toupper(GetChar());
    }
    if(toupper(cStatusFilterchoice) == 'X')
    {
        return;
    }
    else if(cStatusFilterchoice == 'A')
    {
        iStatusFilter = 0;
    }
    else if(cStatusFilterchoice == 'U')
    {
        iStatusFilter = 6;
    }
    else if(cStatusFilterchoice == 'N')
    {
        iStatusFilter = 1;
    }
    else if(cStatusFilterchoice == 'S')
    {
        iStatusFilter = 2;
    }
    else if(cStatusFilterchoice == 'P')
    {
        iStatusFilter = 5;
    }
    else if(cStatusFilterchoice == 'F')
    {
        iStatusFilter = 4;
    }

    if(iStatusFilter == 0)
    {
        sprintf(caSQL, "SELECT T.`Title ID` , T.`Title Name` 'Title' , A.`Author Name` 'Author', "
                        "COUNT(C.`Character Name`) 'Chars', T.`Start`, T.`Finish`,"
                        "T.`Comments` FROM risingfast.`Book Titles` T "
                        "LEFT JOIN risingfast.`Book Authors` A ON T.`Author ID` = A.`Author ID`"
                        "LEFT JOIN risingfast.`Book Characters` C on T.`Title ID` = C.`Title ID`"
                        "GROUP BY T.`Title ID`"
                        "ORDER BY T.`Title ID` %s", caOrder);
    }
    else
    {
        sprintf(caSQL, "SELECT T.`Title ID` , T.`Title Name` 'Title' , A.`Author Name` 'Author', "
                        "COUNT(C.`Character Name`) 'Chars', T.`Start`, T.`Finish`,"
                        "T.`Comments` FROM risingfast.`Book Titles` T "
                        "LEFT JOIN risingfast.`Book Authors` A ON T.`Author ID` = A.`Author ID`"
                        "LEFT JOIN risingfast.`Book Characters` C on T.`Title ID` = C.`Title ID`"
                        "WHERE T.`Status ID` = %d "
                        "GROUP BY T.`Title ID`"
                        "ORDER BY T.`Title ID` %s", iStatusFilter, caOrder);
    }

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    if(*pcDisplayPageWidth == 'W')
    {
        iColCount = mysql_num_fields(res);
    }
    else if(*pcDisplayPageWidth == 'N')
    {
        iColCount = mysql_num_fields(res) - 3;
    }

// fetch the max width of each column

    iLengths = (int *)calloc(iColCount, sizeof(int));

    while(row = mysql_fetch_row(res))
    {
        for (int k = 0; k < iColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > iLengths[k])
                {
                     iLengths[k] = strlen(row[k]);
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
        if(*pcDipslayPageFormat == 'T')
        {
            bEndOfPrintBlock = false;
            iRowCount++;
            for(int i = 0; i < iColCount; i++)
            {
                if((i==3) || (i==0))
                {
                    printf("%*s", iLengths[i] + 1, row[i] ? row[i] : "");
                }
                else
                {
                    printf("  %-*s", iLengths[i] + 1, row[i] ? row[i] : "");
                }
            }
            printf("\n");
            if(iRowCount >= *piDisplayPageLength)
            {
                printf("\n");
                fPressEnterToContinue();
                printf("\n");
                iRowCount = 0;
            }
        }
        else
        {
            for(int i = 0; i < iColCount; i++)
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

    fRetitleConsole(sPrgNme);
    free(iLengths);
    mysql_free_result(res);
    return;
}

void fMaintainAttributes(char *sPrgNme, int *piDisplayPageLength)
{

    char cAttributeChoice = '0';
    char cAddChangeDeleteListSearchExitChoice = '0';
    char *sAuthorSearch = NULL;
    char *sSeriesSearch = NULL;
    char *sGenreSearch = NULL;
    char *sSourceSearch = NULL;
    char *sRatingSearch = NULL;
    char *sStatusSearch = NULL;
    char *sClassificationSearch = NULL;
    bool bExitAttributes = false;
    bool bAttributeAdded = false;

    fRetitleConsole(sPrgNme);

    while(bExitAttributes == false)
    {
        while(cAttributeChoice == '0')
        {
            bAttributeAdded = false;
            fShowAttributesMenu();
            cAttributeChoice = GetChar();
            if(strchr("12345678AaCcRrEeOoSsGgXx", cAttributeChoice) == NULL)
            {
                cAttributeChoice = 0;
            }
            printf("\n");
        }
        if(strchr("1Aa", cAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(sPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Authors");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                cAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(cAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddAuthor(sPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    cAttributeChoice = '0';
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateAuthor(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteAuthor(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Authors > List");
                    printf("\n\n");
                    fShowAllAuthors(piDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Authors > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    sAuthorSearch = GetString();
                    if(strstr("xX", sAuthorSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchAuthors(sAuthorSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    cAttributeChoice = '0';
                }
            }
        }
        else if(strchr("2Cc", cAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(sPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Classifications");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                cAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(cAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddClassification(sPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    cAttributeChoice = '0';
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateClassification(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteClassification(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Classifications > List");
                    printf("\n\n");
                    fShowAllClassifications(piDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Classifications > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    sClassificationSearch = GetString();
                    if(strstr("xX", sClassificationSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchClassifications(sClassificationSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    cAttributeChoice = '0';
                }
            }
        }
        else if(strchr("3Rr", cAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(sPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Ratings");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                cAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(cAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddRating(sPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    cAttributeChoice = '0';
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateRating(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteRating(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Ratings > List");
                    printf("\n\n");
                    fShowAllRatings(piDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Ratings > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    sRatingSearch = GetString();
                    if(strstr("xX", sRatingSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchRatings(sRatingSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    cAttributeChoice = '0';
                }
            }
        }
        else if(strchr("4Ee", cAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(sPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Series");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                cAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(cAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddSeries(sPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    cAttributeChoice = '0';
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateSeries(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteSeries(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Series > List");
                    printf("\n\n");
                    fShowAllSeries(piDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Series > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    sSeriesSearch = GetString();
                    if(strstr("xX", sSeriesSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchSeries(sSeriesSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    cAttributeChoice = '0';
                }
            }
        }
        else if(strchr("5Oo", cAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(sPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attribues > Sources");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                cAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(cAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddSource(sPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    cAttributeChoice = '0';
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateSource(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'D')
                {
                    printf("\n");
                    fDeleteSource(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Sources > List");
                    printf("\n\n");
                    fShowAllSources(piDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Sources > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    sSourceSearch = GetString();
                    if(strstr("xX", sSourceSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchSources(sSourceSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'X')
                {
                     bAttributeAdded = true;
                     cAttributeChoice = '0';
                }
            }
        }
        else if(strchr("6Ss", cAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(sPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Statuses");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                cAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(cAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddStatus(sPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    cAttributeChoice = '0';
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateStatus(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'D')
                {
                     printf("\n");
                     fDeleteStatus(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Statuses > List");
                    printf("\n\n");
                    fShowAllStatuses(piDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Statuses > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    sStatusSearch = GetString();
                    if(strstr("xX", sStatusSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchStatuses(sStatusSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    cAttributeChoice = '0';
                }
            }
        }
        else if(strchr("7Gg", cAttributeChoice) != NULL)
        {
            while(bAttributeAdded == false)
            {
                fRetitleConsole(sPrgNme);
                printf("\n");
                printf("Main Menu > Maintain Attributes > Genres");
                printf("\n\n");
                printf("(A)dd, (C)hange, (D)elete, (L)ist, (S)earch or E(x)it: ");
                cAddChangeDeleteListSearchExitChoice = toupper(GetChar());
                if(cAddChangeDeleteListSearchExitChoice == 'A')
                {
                    printf("\n");
                    if(fAddGenre(sPrgNme) == 0)
                    {
                        bAttributeAdded = false;
                    }
                    else
                    {
                        bAttributeAdded = true;
                    }
                    cAttributeChoice = '0';
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'C')
                {
                    printf("\n");
                    fUpdateGenre(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'D')
                {
                    fDeleteGenre(sPrgNme);
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'L')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Genres > List");
                    printf("\n\n");
                    fShowAllGenres(piDisplayPageLength);
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'S')
                {
                    printf("\n");
                    fRetitleConsole(sPrgNme);
                    printf("\n");
                    printf("Main Menu > Maintain Attributes > Genres > Search");
                    printf("\n\n");
                    printf("Search String or E(x)it: ");
                    sGenreSearch = GetString();
                    if(strstr("xX", sGenreSearch) != NULL)
                    {
                        continue;
                    }
                    fSearchGenres(sGenreSearch);
                    printf("\n");
                    fPressEnterToContinue();
                }
                else if(cAddChangeDeleteListSearchExitChoice == 'X')
                {
                    bAttributeAdded = true;
                    cAttributeChoice = '0';
                }
            }
        }
        else if(strchr("8Xx", cAttributeChoice) != NULL)
        {
            bExitAttributes = true;
        }

        fRetitleConsole(sPrgNme);
    }

    return;
}

void fSetOptions(char *sPrgNme, int *piDisplayPageLength, char *pcDisplayPageWidth, char *pcDipslayPageFormat, char *pcDisplayOrder)
{

    int  iOptionChoice = '0';
    bool bExitOptions = false;
    bool bValidOptionChoice = false;

    fRetitleConsole(sPrgNme);

    while(bExitOptions == false)
    {
        while(iOptionChoice == '0')
        {
            printf("\n");
            printf("Main Menu > Set Options");
            printf("\n");
            printf("\n");
            printf("1. Display (L)ength: %9d", *piDisplayPageLength);
            printf("\n");
            printf("2. Display (W)idth: %10s", (*pcDisplayPageWidth == 'N') ? "Narrow" : "Wide");
            printf("\n");
            printf("3. List (F)ormat: %12s", (*pcDipslayPageFormat == 'T') ? "Tab" : "CSV");
            printf("\n");
            printf("4. List (O)rder: %13s", (*pcDisplayOrder == 'A') ? "Ascending" : "Descending");
            printf("\n");
            printf("5. (P)rint Options");
            printf("\n");
            printf("6. (R)eset Options");
            printf("\n");
            printf("7. E(x)it");
            printf("\n\n");
            printf("Choice: ");
            iOptionChoice = GetChar();
            if(strchr("1234567LloOpPrRxX", iOptionChoice) == NULL)
            {
                iOptionChoice = '0';
            }
        }
        if(strchr("1lL", iOptionChoice) != NULL)
        {
            printf("\n");
            printf("Display Length: ");
            *piDisplayPageLength = GetInt();
            printf("\n");
            iOptionChoice = '0';
        }
        else if(strchr("2wW", iOptionChoice) != NULL)
        {
            printf("\n");
            while(bValidOptionChoice == false)
            {
                printf("Display Width ((W)ide or (N)arrow) or E(x)it: ");
                *pcDisplayPageWidth = toupper(GetChar());
                if(*pcDisplayPageWidth == 'W')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcDisplayPageWidth == 'N')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcDisplayPageWidth == 'X')
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
            iOptionChoice = '0';
        }
        else if(strchr("3fF", iOptionChoice) != NULL)
        {
            printf("\n");
            while(bValidOptionChoice == false)
            {
                printf("Display Format ((T)ab or (C)SV) or E(x)it: ");
                *pcDipslayPageFormat = toupper(GetChar());
                if(*pcDipslayPageFormat == 'T')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcDipslayPageFormat == 'C')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcDipslayPageFormat == 'X')
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
            iOptionChoice = '0';
        }
        else if(strchr("4oO", iOptionChoice) != NULL)
        {
            printf("\n");
            while(bValidOptionChoice == false)
            {
                printf("Display Order ((A)sc or (D)esc or E(x)it): ");
                *pcDisplayOrder = toupper(GetChar());
                if(*pcDisplayOrder == 'A')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcDisplayOrder == 'D')
                {
                    bValidOptionChoice = true;
                }
                else if(*pcDisplayOrder == 'X')
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
            iOptionChoice = '0';
        }
        else if(strchr("5pP", iOptionChoice) != NULL)
        {
            *pcDipslayPageFormat = 'C';
            *pcDisplayOrder = 'A';
            *piDisplayPageLength = 1000;
            *pcDisplayPageWidth = 'W';
            bValidOptionChoice = false;
            iOptionChoice = '0';
        }
        else if(strchr("6rR", iOptionChoice) != NULL)
        {
            *pcDipslayPageFormat = 'T';
            *pcDisplayOrder = 'A';
            *piDisplayPageLength = 80;
            *pcDisplayPageWidth = 'N';
            bValidOptionChoice = false;
            iOptionChoice = '0';
        }
        else if(strchr("7xX", iOptionChoice) != NULL)
        {
            bExitOptions = true;
        }

        fRetitleConsole(sPrgNme);
    }

    return;
}

int fAddSeries(char *sPrgNme)
{
    char *sSeriesName = NULL;
    char *sSeriesNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iSeriesMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Series > Add");
    printf("\n\n");

    iSeriesMaxLength = fGetFieldLength("Book Series", "Series Name");

    do{
        printf("Series Name or E(x)it (max %d chars): ", iSeriesMaxLength);
        sSeriesName = GetString();
        if(strlen(sSeriesName) > iSeriesMaxLength)
        {
            printf("Series Name max %d chars. You entered %d chars", iSeriesMaxLength, (int) strlen(sSeriesName));
            printf("\n");
            sSeriesName[0] = '\0';
        }
    } while(strlen(sSeriesName) == 0);

    sSeriesNameEscaped = fEscapeSingleQuote(sSeriesName);

    if(strcmp(sSeriesName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(caSQL, "INSERT INTO `Book Series` (`Series Name`) VALUES('%s')", sSeriesNameEscaped);

// initialize a connection and connect to the database

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        printf("Series Added: %s", sSeriesName);
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

    fRetitleConsole(sPrgNme);

    free(sSeriesName);
    free(sSeriesNameEscaped);
    return EXIT_SUCCESS;
}

int fAddAuthor(char *sPrgNme)
{
    char *sAuthorName = NULL;
    char *sEscapedAuthorName = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iAuthorMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Authors > Add");
    printf("\n\n");

// get the author name

    iAuthorMaxLength = fGetFieldLength("Book Authors", "Author Name");
    do {
        printf("Author Name or E(x)it (max %d chars): ", iAuthorMaxLength);
        sAuthorName = GetString();
        if(strlen(sAuthorName) > iAuthorMaxLength)
        {
            printf("Author Name max %d chars. You entered %d chars", iAuthorMaxLength, (int) strlen(sAuthorName));
            printf("\n");
            sAuthorName[0] = '\0';
        }
    }while (strlen(sAuthorName) == 0);

    sEscapedAuthorName = fEscapeSingleQuote(sAuthorName);

    if(strcmp(sAuthorName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(caSQL, "INSERT INTO `Book Authors` (`Author Name`) VALUES('%s')", sEscapedAuthorName);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        printf("Author Added: %s", sAuthorName);
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

    free(sEscapedAuthorName);
    free(sAuthorName);
    fRetitleConsole(sPrgNme);
    return EXIT_SUCCESS;
}

int fAddClassification(char *sPrgNme)
{
    char *sClassificationName = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    char *sClassificationNameEscaped = NULL;
    int  iClassificationMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Classification > Add");
    printf("\n\n");

    iClassificationMaxLength = fGetFieldLength("Book Classifications", "Classification Name");

    do
    {
        printf("Classification Name or E(x)it (max %d chars): ", iClassificationMaxLength);
        sClassificationName = GetString();
        if(strlen(sClassificationName) > iClassificationMaxLength)
        {
            printf("Classification Name max %d chars. You entered %d chars", iClassificationMaxLength, (int) strlen(sClassificationName));
            printf("\n");
            sClassificationName[0] = '\0';
        }
    } while(strlen(sClassificationName) == 0);

    sClassificationNameEscaped = fEscapeSingleQuote(sClassificationName);

    if(strcmp(sClassificationName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");

    sprintf(caSQL, "INSERT INTO `Book Classifications` (`Classification Name`) VALUES('%s')", sClassificationNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        printf("Classification Added: %s", sClassificationName);
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

    fRetitleConsole(sPrgNme);
    free(sClassificationNameEscaped);
    free(sClassificationName);
    return EXIT_SUCCESS;
}

int fAddRating(char *sPrgNme)
{
    char *sRatingName = NULL;
    char *sRatingNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRatingMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Rating > Add");
    printf("\n\n");

    iRatingMaxLength = fGetFieldLength("Book Ratings", "Rating Name");

    do{
        printf("Rating Name or E(x)it (max %d chars): ", iRatingMaxLength);
        sRatingName = GetString();
        if(strlen(sRatingName) > iRatingMaxLength)
        {
            printf("Rating Name max %d chars. You entered %d chars", iRatingMaxLength, (int) strlen(sRatingName));
            printf("\n");
            sRatingName[0] = '\0';
        }
    } while(strlen(sRatingName) == 0);

    sRatingNameEscaped = fEscapeSingleQuote(sRatingName);

    if(strcmp(sRatingName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(caSQL, "INSERT INTO `Book Ratings` (`Rating Name`) VALUES('%s')", sRatingNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        printf("Rating Added: %s", sRatingName);
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

    fRetitleConsole(sPrgNme);
    free(sRatingName);
    free(sRatingNameEscaped);
    return EXIT_SUCCESS;
}

int fAddSource(char *sPrgNme)
{
    char *sSourceName = NULL;
    char *sSourceNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iSourceMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Source > Add");
    printf("\n\n");

    iSourceMaxLength = fGetFieldLength("Book Sources", "Source Name");

    do {
        printf("Source Name or E(x)it (max %d chars): ", iSourceMaxLength);
        sSourceName = GetString();
        if (strlen(sSourceName) > iSourceMaxLength)
        {
            printf("Source Name max %d chars. You entered %d chars", iSourceMaxLength, (int) strlen(sSourceName));
            printf("\n");
            sSourceName[0] = '\0';
        }
    } while(strlen(sSourceName) == 0);

    sSourceNameEscaped = fEscapeSingleQuote(sSourceName);

    if(strcmp(sSourceName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(caSQL, "INSERT INTO `Book Sources` (`Source Name`) VALUES('%s')", sSourceNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        printf("Source Added: %s", sSourceName);
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

    fRetitleConsole(sPrgNme);

    free(sSourceName);
    free(sSourceNameEscaped);
    return EXIT_SUCCESS;
}

int fAddStatus(char *sPrgNme)
{
    char *sStatusName = NULL;
    char *sStatusNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iStatusMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Status > Add");
    printf("\n\n");

    iStatusMaxLength = fGetFieldLength("Book Statuses", "Status Name");

    do {
        printf("Status Name or E(x)it (max %d chars): ", iStatusMaxLength);
        sStatusName = GetString();
        if (strlen(sStatusName) > iStatusMaxLength)
        {
             printf("Status Name max %d chars. You entered %d chars", iStatusMaxLength, (int) strlen(sStatusName));
             printf("\n");
             sStatusName[0] = '\0';
        }
    } while(strlen(sStatusName) == 0);

    sStatusNameEscaped = fEscapeSingleQuote(sStatusName);

    if(strcmp(sStatusName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }
    printf("\n");
    sprintf(caSQL, "INSERT INTO `Book Statuses` (`Status Name`) VALUES('%s')", sStatusNameEscaped);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        printf("Status Added: %s", sStatusName);
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

    fRetitleConsole(sPrgNme);
    free(sStatusName);
    free(sStatusNameEscaped);
    return EXIT_SUCCESS;
}

int fAddGenre(char *sPrgNme)
{
    char *sGenreName = NULL;
    char *sGenreDesc = NULL;
    char *sGenreNameEscaped = NULL;
    char *sGenreDescEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iGenreNameMaxLength = 0;
    int  iGenreDescMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Genre > Add:");
    printf("\n\n");

    iGenreNameMaxLength = fGetFieldLength("Book Genres", "Genre Name");
    iGenreDescMaxLength = fGetFieldLength("Book Genres", "Genre Description");

    do{
        printf("Genre Name or E(x)it (max %d chars): ", iGenreNameMaxLength);
        sGenreName = GetString();
        if (strlen(sGenreName) > iGenreNameMaxLength)
        {
             printf("Genre Name max %d chars. You entered %d chars", iGenreNameMaxLength, (int) strlen(sGenreName));
             printf("\n");
             sGenreName[0] = '\0';
        }
    } while(strlen(sGenreName) == 0);

    sGenreNameEscaped = fEscapeSingleQuote(sGenreName);

    if(strcmp(sGenreName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }

    do{
        printf("Genre Description or E(x)it (max %d chars): ", iGenreDescMaxLength);
        sGenreDesc = GetString();
        if (strlen(sGenreDesc) > iGenreDescMaxLength)
        {
             printf("Genre Desc max %d chars. You entered %d chars", iGenreDescMaxLength, (int) strlen(sGenreDesc));
             printf("\n");
             sGenreDesc[0] = '\0';
        }
    } while(strlen(sGenreDesc) == 0);

    sGenreDescEscaped = fEscapeSingleQuote(sGenreDesc);

    if(strcmp(sGenreDesc, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return EXIT_SUCCESS;
    }

    printf("\n");
    sprintf(caSQL, "INSERT INTO `Book Genres` (`Genre Name`, `Genre Description`) VALUES('%s', '%s')", sGenreNameEscaped, sGenreDescEscaped);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        printf("Genre Added: %s, %s", sGenreName, sGenreDesc);
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

    fRetitleConsole(sPrgNme);
    free(sGenreName);
    free(sGenreNameEscaped);
    free(sGenreDesc);
    free(sGenreDescEscaped);
    return EXIT_SUCCESS;
}

void fAddTitle(char *sPrgNme)
{
    char *sTitleName = NULL;
    char *sTitleNameEscaped = NULL;
    char *sStartDate;
    char caSQL[SQL_LEN] = {'\0'};
    int  iTitleMaxLength = 0;

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Add Title");
    printf("\n\n");

// Get the title name and start date from the console

    iTitleMaxLength = fGetFieldLength("Book Titles", "Title Name");
    do {
        printf("Title Name or E(x)it (max %d chars): ", iTitleMaxLength);
        sTitleName = GetString();
        if(strlen(sTitleName) > iTitleMaxLength)
        {
            printf("Title Name max %d chars. You entered %d chars", iTitleMaxLength, (int) strlen(sTitleName));
            printf("\n");
            sTitleName[0] = '\0';
        }
    } while(strlen(sTitleName) == 0);

    sTitleNameEscaped = fEscapeSingleQuote(sTitleName);

    if(strcmp(sTitleName, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return;
    }

    printf("Start Date (YYYY-MM-DD) or E(x)it: ");
    sStartDate = GetString();
    if(strcmp(sStartDate, "x") == 0)
    {
        fRetitleConsole(sPrgNme);
        return;
    }

    printf("\n");
    if(strlen(sStartDate) == 0)
    {
        sprintf(caSQL, "INSERT INTO `Book Titles` (`Title Name`, `Author ID`, `Source ID`, `Series ID`"
                        ", `Genre ID`, `Status ID`, `Classification ID`,"
                        "`Rating ID`, `Start`) VALUES ('%s', 67, 10, 57, 17, 6, 3, 6, null)", sTitleNameEscaped);
    }
    else
    {
        sprintf(caSQL, "INSERT INTO `Book Titles` (`Title Name`, `Author ID`, `Source ID`, `Series ID`"
                        ", `Genre ID`, `Status ID`, `Classification ID`,"
                        "`Rating ID`, `Start`) VALUES ('%s', 67, 10, 57, 17, 6, 3, 6, '%s')", sTitleNameEscaped, sStartDate);
    }

// execute the query and check for no result

    if(mysql_query(conn, caSQL) == 0)
    {
        if(strlen(sStartDate) > 0)
        {
            printf("Title Added: %s starting %s", sTitleName, sStartDate);
            printf("\n\n");
            fPressEnterToContinue();
        }
        else
        {
            printf("Title Added: %s", sTitleName);
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

    free(sTitleNameEscaped);
    free(sTitleName);
    fRetitleConsole(sPrgNme);
    return;
}

void fUpdateTitleAndAttributes(char *sPrgNme, int *piDisplayPageLength)
{
    int  iTitleID = 0;
    int  iMaxTitleID = 0;
    int  iMinTitleID = 0;
    int  iNewAuthorID = 0;
    int  iNewSourceID = 0;
    int  iNewSeriesID = 0;
    int  iNewGenreID = 0;
    int  iNewStatusID = 0;
    int  iNewClassnID = 0;
    int  iNewRatingID = 0;
    bool bExitAttributeChoice = false;
    bool bTitleFound = false;
    bool bAttributeQueryIsValid = false;
    char caSQL[SQL_LEN] = {'\0'};
    char *sTitleName = NULL;
    char *sNewStartDate = NULL;
    char *sNewFinishDate = NULL;
    char *sNewComments = NULL;
    char *sNewCommentsEscaped = NULL;
    char cUpdateTitleAndAttributesAction = 'X';
    char *sTitleSearch = NULL;
    char *sAttribChoice = "00";                                                      // choice of attribute to change

    while(bTitleFound == false)
    {
        fRetitleConsole(sPrgNme);

        printf("\n");
        printf("Main Menu > Update a Title and Attributes");
        printf("\n\n");

        printf("(T)itle ID, (S)earch or E(x)it: ");
        cUpdateTitleAndAttributesAction = GetChar();

        if(toupper(cUpdateTitleAndAttributesAction) == 'S')
        {
            printf("\n");
            printf("Search String: ");
            sTitleSearch = GetString();
            fSearchTitles(sTitleSearch);
            printf("\n");
            fPressEnterToContinue();
        }
        else if(toupper(cUpdateTitleAndAttributesAction) == 'T')
        {
            printf("\n");
            printf("Title ID: ");
            iTitleID = GetInt();
            if(fCheckTitleIDExists(iTitleID) == true)
            {
                bTitleFound = true;
            }
            else
            {
                bTitleFound = false;
            }
            printf("\n");
        }
        else if(toupper(cUpdateTitleAndAttributesAction) == 'X')
        {
            fRetitleConsole(sPrgNme);
            return;
        }
        else
        {
            printf("\n");
            printf("Not a valid choice so exiting");
            bTitleFound = true;
            printf("\n\n");
            fPressEnterToContinue();
            fRetitleConsole(sPrgNme);
            return;
        }
    }

    iMaxTitleID = fGetMaxTitleID();
    iMinTitleID = fGetMinTitleID();

    while(bExitAttributeChoice == false)
    {
        while((iTitleID < iMinTitleID) || (iTitleID > iMaxTitleID + 1))
        {
            printf("Title ID (%d - %d): ", iMinTitleID, iMaxTitleID);
            iTitleID = GetInt();
        }

        fRetitleConsole(sPrgNme);
        printf("\n");
        fShowTitle(iTitleID, sPrgNme);
        fShowAttributes(iTitleID);

        while((strstr("12345678910111213tTaAoOeEgGsScCrRdDfFmMxXhH", sAttribChoice) == NULL))
        {
            printf("\n");
            printf("Attribute Number (1-13): ");
            sAttribChoice = GetString();
            printf("\n");
        }

        if(strstr("1tT", sAttribChoice) != NULL)
        {
            printf("New Title Name or E(x)it: ");
            sTitleName = fEscapeSingleQuote(GetString());
            if((toupper(sTitleName[0]) == 'X') && (strlen(sTitleName) == 1))
            {
                strcpy(sAttribChoice, "00");
                bExitAttributeChoice == false;
                bAttributeQueryIsValid = false;
            }
            else
            {
                sprintf(caSQL, "UPDATE `Book Titles` SET `Title Name` = '%s' WHERE `Title ID` = %d", sTitleName, iTitleID);
                strcpy(sAttribChoice, "00");
                bAttributeQueryIsValid = true;
            }
        }
        else if(strstr("2aA", sAttribChoice) != NULL)
        {
            fShowAllAuthors(piDisplayPageLength);
            printf("New Author ID: ");
            iNewAuthorID = GetInt();
            sprintf(caSQL, "UPDATE `Book Titles` SET `Author ID` = %d WHERE `Title ID` = %d", iNewAuthorID, iTitleID);
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("3Oo", sAttribChoice) != NULL)
        {
            fShowAllSources(piDisplayPageLength);
            printf("New Source ID: ");
            iNewSourceID = GetInt();
            sprintf(caSQL, "UPDATE `Book Titles` SET `Source ID` = %d WHERE `Title ID` = %d", iNewSourceID, iTitleID);
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("4eE", sAttribChoice) != NULL)
        {
            fShowAllSeries(piDisplayPageLength);
            printf("New Series ID: ");
            iNewSeriesID = GetInt();
            sprintf(caSQL, "UPDATE `Book Titles` SET `Series ID` = %d WHERE `Title ID` = %d", iNewSeriesID, iTitleID);
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("5gG", sAttribChoice) != NULL)
        {
            fShowAllGenres(piDisplayPageLength);
            printf("New Genre ID: ");
            iNewGenreID = GetInt();
            sprintf(caSQL, "UPDATE `Book Titles` SET `Genre ID` = %d WHERE `Title ID` = %d", iNewGenreID, iTitleID);
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("6sS", sAttribChoice) != NULL)
        {
            fShowAllStatuses(piDisplayPageLength);
            printf("New Status ID: ");
            iNewStatusID = GetInt();
            sprintf(caSQL, "UPDATE `Book Titles` SET `Status ID` = %d WHERE `Title ID` = %d", iNewStatusID, iTitleID);
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("7cC", sAttribChoice) != NULL)
        {
            fShowAllClassifications(piDisplayPageLength);
            printf("New Classn ID: ");
            iNewClassnID = GetInt();
            sprintf(caSQL, "UPDATE `Book Titles` SET `Classification ID` = %d WHERE `Title ID` = %d", iNewClassnID, iTitleID);
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("8rR", sAttribChoice) != NULL)
        {
            fShowAllRatings(piDisplayPageLength);
            printf("New Rating ID: ");
            iNewRatingID = GetInt();
            sprintf(caSQL, "UPDATE `Book Titles` SET `Rating ID` = %d WHERE `Title ID` = %d", iNewRatingID, iTitleID);
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("9dD", sAttribChoice) != NULL)
        {
            printf("New Start Date (YYYY-MM-DD): ");
            sNewStartDate = GetString();
            if(strlen(sNewStartDate) == 0)
            {
                sprintf(caSQL, "UPDATE `Book Titles` SET `Start` = NULL WHERE `Title ID` = %d", iTitleID);
            }
            else
            {
                sprintf(caSQL, "UPDATE `Book Titles` SET `Start` = '%s' WHERE `Title ID` = %d", sNewStartDate, iTitleID);
            }
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("10fF", sAttribChoice) != NULL)
        {
            printf("New Finish Date (YYYY-MM-DD): ");
            sNewFinishDate = GetString();
            if(strlen(sNewFinishDate) == 0)
            {
                sprintf(caSQL, "UPDATE `Book Titles` SET `Finish` = NULL WHERE `Title ID` = %d", iTitleID);
            }
            else
            {
                sprintf(caSQL, "UPDATE `Book Titles` SET `Finish` = '%s' WHERE `Title ID` = %d", sNewFinishDate, iTitleID);
            }
            bAttributeQueryIsValid = true;
            strcpy(sAttribChoice, "00");
        }
        else if(strstr("11mM", sAttribChoice) != NULL)
        {
            printf("New Comments or E(x)it: ");
            sNewComments = fEscapeSingleQuote(GetString());
            if((toupper(sNewComments[0]) == 'X') && (strlen(sNewComments) == 1))
            {
                strcpy(sAttribChoice, "00");
                bExitAttributeChoice == false;
                bAttributeQueryIsValid = false;
            }
            else
            {
                sprintf(caSQL, "UPDATE `Book Titles` SET `Comments` = '%s' WHERE `Title ID` = %d", sNewComments, iTitleID);
                free(sNewComments);
                strcpy(sAttribChoice, "00");
                bAttributeQueryIsValid = true;
            }
        }
        else if(strstr("12hH", sAttribChoice) != NULL)
        {
            printf("Show Characters: ");
            printf("\n\n");
            fShowCharacters(iTitleID, sPrgNme);
            sprintf(caSQL, "UPDATE `Book Titles` SET `Comments` = '%s' WHERE `Title ID` = %d", sNewComments, iTitleID);
            strcpy(sAttribChoice, "00");
            printf("\n");
            fPressEnterToContinue();
        }
        else if(strstr("13xX", sAttribChoice) != NULL)
        {
            bExitAttributeChoice = true;
        }

// execute the query and check for no result

        if(mysql_query(conn, caSQL) != 0)
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
            fRetitleConsole(sPrgNme);
            strcpy(sAttribChoice, "00");
        }
        fRetitleConsole(sPrgNme);
    }

    fRetitleConsole(sPrgNme);
    return;
}

int  fGetMaxTitleID(void)
{
    char caSQL[SQL_LEN] = {'\0'};
    char **endptr;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT MAX(`Title ID`) from `Book Titles`");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
    char caSQL[SQL_LEN] = {'\0'};
    char **endptr;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT MIN(`Title ID`) from `Book Titles`");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

void fShowAttributes(int iTitleID)
{
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT `Title ID`"
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
                         "WHERE `Title ID` = %d", iTitleID);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

void fShowCharacters(int iTitleID, char *sPrgNme)
{
    int *iLengths = NULL;
    int iColCount = 0;
    int iRowsReturned = 0;
    int iRowsPrinted = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Character ID`, `Character Name` "
                    "FROM `Book Characters` "
                    "WHERE `Title ID` = %d", iTitleID);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("No characters in Title ID: %d", iTitleID);
        printf("\n");
    }

// fetch the max width of each column

    iLengths = (int *)calloc(iColCount, sizeof(int));

    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        for(int k = 0; k < iColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > iLengths[k])
                {
                     iLengths[k] = strlen(row[k]);
                }
            }
        }
    }

// reset the  pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// print each row of results

    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        for(int i = 0; i < iColCount; i++)
        {
            printf("%-*s", iLengths[i] + 1, row[i] ? row[i] : "");
        }
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fSearchTitles(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Title ID`, `Title Name` "
                    "FROM `Book Titles` "
                    "WHERE `Title Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching titles found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fShowAllSources(int *piDisplayPageLength)
{
    int  iRowCount = 0;
    int  iMaxWidth = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT S.`Source ID`"
                         ", S.`Source Name` "
                         ", COUNT(T.`Title ID`)"
                   " FROM `Book Sources` S"
                   " LEFT JOIN `Book Titles` T ON S.`Source ID` = T.`Source ID`"
                   " GROUP BY S.`Source ID`, S.`Source Name`"
                   " ORDER BY `Source ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Sources and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        printf("%3s  %-*s %3s", row[0], iMaxWidth, row[1], row[2]);
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            iRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllGenres(int *piDisplayPageLength)
{
    int  iRowCount = 0;
    int  iMaxWidth = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT G.`Genre ID`"
                         ", G. `Genre Name` "
                         ", COUNT(T.`Title ID`)"
                   " FROM `Book Genres` G"
                   " LEFT JOIN `Book Titles` T ON G.`Genre ID` = T.`Genre ID`"
                   " GROUP BY G.`Genre ID`, G.`Genre Name`"
                   " ORDER BY `Genre ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Genres and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        printf("%3s  %-*s %3s", row[0], iMaxWidth, row[1], row[2]);
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            iRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllStatuses(int *piDisplayPageLength)
{
    int  iRowCount = 0;
    int  iMaxWidth = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT S.`Status ID`"
                        ", S.`Status Name`"
                        ", COUNT(T.`Title ID`)"
                   " FROM `Book Statuses` S"
                   " LEFT JOIN `Book Titles` T ON S.`Status ID` = T.`Status ID`"
                   " GROUP BY S.`Status ID`, S.`Status Name`"
                   " ORDER BY `Status ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Statuses and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        printf("%3s  %-*s %3s", row[0], iMaxWidth, row[1], row[2]);
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            iRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllClassifications(int *piDisplayPageLength)
{
    int  iRowCount = 0;
    int  iMaxWidth = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT C.`Classification ID`"
                        " , C.`Classification Name`"
                        " , COUNT(T.`Title ID`)"
                   " FROM `Book Classifications` C"
                   " LEFT JOIN `Book Titles` T ON C.`Classification ID` = T.`Classification ID`"
                   " GROUP BY C.`Classification ID`, C.`Classification Name`"
                    "ORDER BY C.`Classification ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Classifications and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        printf("%3s  %-*s %3s", row[0], iMaxWidth, row[1], row[2]);
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            iRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllRatings(int  *piDisplayPageLength)
{
    int  iRowCount = 0;
    int  iMaxWidth = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT R.`Rating ID`"
                        " , R.`Rating Name` "
                        " , COUNT(T.`Title ID`)"
                   " FROM `Book Ratings` R"
                   " LEFT JOIN `Book Titles` T ON R.`Rating ID` = T.`Rating ID`"
                   " GROUP BY R.`Rating ID`, R.`Rating Name`"
                   " ORDER BY `Rating ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Ratings and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        printf("%3s  %-*s %3s", row[0], iMaxWidth, row[1], row[2]);
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            iRowCount = 0;
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

void fCharacters(char *sPrgNme)
{
    int  iTitleID = 0;
    char cExitOrContinue = 'C';
    char *sCharacterToAdd = NULL;
    char *sCharacterToAddEscaped = NULL;
    char *sCharacterToChange = NULL;
    char *sCharacterToChangeEscaped = NULL;
    char *sTitleSearch = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    char cAddCharacterAction = 'X';
    bool bExitCharacters = false;
    bool bTitleFound = false;
    bool bValidChoice = false;
    char cAction = 'A';
    int  iCharID = 0;
    int  iCharacterMaxLength = 0;

    while(bTitleFound == false)
    {
        fRetitleConsole(sPrgNme);

        printf("\n");
        printf("Main Menu > Characters");
        printf("\n\n");

        printf("(T)itle ID, (S)earch or E(x)it: ");
        cAddCharacterAction = GetChar();

        if(toupper(cAddCharacterAction) == 'S')
        {
            printf("\n");
            printf("Search String: ");
            sTitleSearch = GetString();
            fSearchTitles(sTitleSearch);
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            cAddCharacterAction = 'T';
        }
        else if(toupper(cAddCharacterAction) == 'T')
        {
            printf("\n");
            printf("Title ID: ");
            iTitleID = GetInt();
            if(fCheckTitleIDExists(iTitleID) == true)
            {
                bTitleFound = true;
            }
            else
            {
                bTitleFound = false;
            }
            printf("\n");
        }
        else if(toupper(cAddCharacterAction) == 'X')
        {
            fRetitleConsole(sPrgNme);
            return;
        }
        else
        {
            printf("\n");
            printf("Not a valid choice so exiting");
            bTitleFound = true;
            printf("\n\n");
            fPressEnterToContinue();
            fRetitleConsole(sPrgNme);
            return;
        }
    }

    while(bExitCharacters == false)
    {
        while(bValidChoice == false)
        {

            fRetitleConsole(sPrgNme);
            printf("\n");
            printf("Main Menu > Characters");
            printf("\n\n");
            printf("Title ID: %d", iTitleID);
            printf("\n\n");

            fShowTitle(iTitleID, sPrgNme);
            printf("\n");
            fShowCharacters(iTitleID, sPrgNme);

            printf("\n");
            printf("(A)dd, (C)hange, (D)elete or E(x)it Characters: ");
            cAction = toupper(GetChar());
            if(strchr("xX", cAction) != NULL)
            {
                fRetitleConsole(sPrgNme);
                return;
            }
            else if(strchr("aA", cAction) != NULL)
            {
                bValidChoice = true;
                printf("\n");

                iCharacterMaxLength = fGetFieldLength("Book Characters", "Character Name");

                do{
                    printf("Character Name (max %d chars): ", iCharacterMaxLength);
                    sCharacterToAdd = GetString();
                    if(strlen(sCharacterToAdd) > iCharacterMaxLength)
                    {
                        printf("Character Name max %d chars. You entered %d chars", iCharacterMaxLength, (int) strlen(sCharacterToAdd));
                        printf("\n");
                        sCharacterToAdd[0] = '\0';
                    }
                } while(strlen(sCharacterToAdd) == 0);

                sCharacterToAddEscaped = fEscapeSingleQuote(sCharacterToAdd);
                
                sprintf(caSQL, "INSERT INTO `Book Characters` (`Character Name`, `Title ID`) VALUES('%s', %d)", sCharacterToAddEscaped, iTitleID);
                
                free(sCharacterToAddEscaped);
                free(sCharacterToAdd);
            }
                
            else if(strchr("cC", cAction) != NULL)
            {
                bValidChoice = true;
                printf("\n");

                iCharacterMaxLength = fGetFieldLength("Book Characters", "Character Name");

                printf("Character ID: ");
                iCharID = GetInt();
                printf("\n");

                do{
                    printf("Character Name (max %d chars): ", iCharacterMaxLength);
                    sCharacterToChange = GetString();
                    if(strlen(sCharacterToChange) > iCharacterMaxLength)
                    {
                        printf("Character Name max %d chars. You entered %d chars", iCharacterMaxLength, (int) strlen(sCharacterToChange));
                        printf("\n");
                        sCharacterToChange[0] = '\0';
                    }
                } while(strlen(sCharacterToChange) ==0);

                sCharacterToChangeEscaped = fEscapeSingleQuote(sCharacterToChange);
                sprintf(caSQL, "UPDATE `Book Characters` SET `Character Name` = '%s' WHERE `Character ID` = %d" , sCharacterToChangeEscaped, iCharID);
                free(sCharacterToChangeEscaped);
                free(sCharacterToChange);
            }
            else if(strchr("dD", cAction) != NULL)
            {
                bValidChoice = true;
                printf("\n");
                printf("Character ID: ");
                iCharID = GetInt();
                sprintf(caSQL, "DELETE FROM `Book Characters` WHERE `Character ID` = %d" , iCharID);
            }
            else
            {
                bValidChoice = false;
            }
        }

        printf("\n");

// execute the query and check for no result

        if(mysql_query(conn, caSQL) == 0)
        {
            if(cAction == 'A')
            {
                printf("Character Added.");
                printf("\n");
            }
            else if(cAction == 'C')
            {
                printf("Character Changed.");
                printf("\n");
            }
            else if(cAction == 'D')
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
        cExitOrContinue = GetChar();
        if(toupper(cExitOrContinue) == 'X')
        {
            bExitCharacters = true;
        }
        else if(toupper(cExitOrContinue) == 'C')
        {
            bExitCharacters = false;
            bValidChoice =  false;
        }
    }

    fRetitleConsole(sPrgNme);
    return;
}

void fShowTitle(int iTitleID, char *sPrgNme)
{
    int *iLengths = NULL;
    int iColCount = 0;
    int iRowsReturned = 0;
    int iRowsPrinted = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Title Name` "
                    "FROM `Book Titles` "
                    "WHERE `Title ID` = %d", iTitleID);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("No Title Name for the Title ID: %d", iTitleID);
        printf("\n");
    }

// fetch the max width of each column

    iLengths = (int *)calloc(iColCount, sizeof(int));

    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        for(int k = 0; k < iColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > iLengths[k])
                {
                     iLengths[k] = strlen(row[k]);
                }
            }
        }
    }

// reset the  pointer in the result set to zero before fetching rows

    mysql_data_seek(res, 0);

// print each row of results

    printf("%d ", iTitleID);

    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        for(int i = 0; i < iColCount; i++)
        {
            printf("%-*s", iLengths[i] + 1, row[i] ? row[i] : "");
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
    strTemp[j] = '\0';                                                                     // terminate the new string
    return(strTemp);
}

bool fCheckTitleIDExists(int iTitleID)
{
    int iRowsReturned = 0;
    char caSQL[SQL_LEN] = {'\0'};
    bool bTitleIDFound = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Title ID` "
                    "FROM `Book Titles` "
                    "WHERE `Title ID` = %d", iTitleID);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No Match for Title ID: %d. ", iTitleID);
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

void fShowAllAuthors(int *piDisplayPageLength)
{
    int  iRowCount = 0;
    int  iMaxWidth = 0;                                                   // Maximum width of Name colum for printing
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;


//    sprintf(caSQL, "SELECT `Author ID`, `Author Name` "
//                    "FROM `Book Authors`"
//                    "ORDER BY `Author ID` ASC");

    sprintf(caSQL, "SELECT A.`Author ID`"
                         ", A.`Author Name` "
                         ", COUNT(T.`Title Name`) "
                    "FROM `Book Authors` A "
                    "LEFT JOIN `Book Titles` T ON A.`Author ID` = T.`Author ID` "
                    "GROUP BY A.`Author ID`, A.`Author Name` "
                    "ORDER BY `Author ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Authors and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        printf("%3s  %-*s %3s", row[0], iMaxWidth, row[1], row[2]);
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            iRowCount = 0;
        }
    }
    printf("\n");

    mysql_free_result(res);
    return;
}

void fShowAllSeries(int *piDisplayPageLength)
{
    int  iRowCount = 0;
    int  iMaxWidth = 0;
    char caSQL[SQL_LEN] = {'\0'};

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT E.`Series ID`"
                        " , E.`Series Name` "
                        ",  COUNT(T.`Title ID`)"
                   " FROM `Book Series` E"
                   " LEFT JOIN `Book Titles` T ON E.`Series ID` = T.`Series ID`"
                   " GROUP BY E.`Series ID`, E.`Series Name`"
                   "ORDER BY `Series ID` ASC");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
        if(strlen(row[1]) > iMaxWidth)
        {
            iMaxWidth = strlen(row[1]);
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    printf("Title Series and Title Counts");
    printf("\n\n");
    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        printf("%3s  %-*s %3s", row[0], iMaxWidth, row[1], row[2]);
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            printf("\n");
            iRowCount = 0;
        }
    }
    printf("\n");
    mysql_free_result(res);
    return;
}

void fListAuthors(char *sPrgNme, int *piDisplayPageLength, char *pcDisplayPageWidth, char *pcDisplayOrder)
{
    int iColCount = 0;
    int *iLengths = NULL;
    int iRowCount = 0;
    char caOrder[6] = {'D', 'E', 'S', 'C', '\0'};
    char caSQL[SQL_LEN] = {'\0'};
    bool bEndOfPrintBlock = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    if(*pcDisplayOrder == 'A')
    {
        strcpy(caOrder, "ASC");
    }
    else if(*pcDisplayOrder == 'D')
    {
        strcpy(caOrder, "DESC");
    }

    sprintf(caSQL, "SELECT A.`Author ID`"
                    ", A.`Author Name` 'Author'"
                    ", COUNT(T.`Title Name`) 'Count'"
                    "FROM risingfast.`Book Authors` A "
                    "LEFT JOIN risingfast.`Book Titles` T ON A.`Author ID` = T.`Author ID`"
                    "GROUP BY A.`Author ID`"
                    "HAVING COUNT(T.`Title Name`) > 0;");

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > List Authors > Authors and Title Counts");
    printf("\n\n");

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);

// fetch the max width of each column

    iLengths = (int *)calloc(iColCount, sizeof(int));

    while(row = mysql_fetch_row(res))
    {
        for(int k = 0; k < iColCount; k++)
        {
            if(row[k] != NULL)
            {
                if(strlen(row[k]) > iLengths[k])
                {
                     iLengths[k] = strlen(row[k]);
                }
            }
        }
    }

    mysql_data_seek(res, 0);

// print each row of results

    while(row = mysql_fetch_row(res))
    {
        iRowCount++;
        bEndOfPrintBlock = false;
        for(int i = 0; i < iColCount; i++)
        if((i==0) || (i==2))
        {
            printf("%*s", iLengths[i] + 1, row[i] ? row[i] : "");
        }
        else
        {
            printf("  %-*s", iLengths[i] + 1, row[i] ? row[i] : "");
        }
        printf("\n");
        if(iRowCount >= *piDisplayPageLength)
        {
            printf("\n");
            fPressEnterToContinue();
            bEndOfPrintBlock = true;
            printf("\n");
            iRowCount = 0;
        }
    }

    free(iLengths);

    printf("\n");
    if(bEndOfPrintBlock == false)
    {
        fPressEnterToContinue();
    }

    fRetitleConsole(sPrgNme);
    mysql_free_result(res);
    return;
}

void fSearchAuthors(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Author ID`, `Author Name` "
                    "FROM `Book Authors` "
                    "WHERE `Author Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching authors found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteAuthor(char *sPrgNme)
{

    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iAuthorID = 0;
    bool bAuthorExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Authors > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Author ID: ");
    iAuthorID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(caSQL, "SELECT `Author ID` FROM `Book Authors` "
                    "WHERE `Author ID` = %d", iAuthorID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Author ID: %d", iAuthorID);
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

    sprintf(caSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Author ID` = %d", iAuthorID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Author ID: %d", iAuthorID);
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
        sprintf(caSQL, "DELETE FROM `Book Authors` "
                        "WHERE `Author ID` = %d", iAuthorID);

        if(mysql_query(conn, caSQL) != 0)
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
        printf("Author ID: %d deleted", iAuthorID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fSearchSeries(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Series ID`, `Series Name` "
                    "FROM `Book Series` "
                    "WHERE `Series Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching series found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteSeries(char *sPrgNme)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iSeriesID = 0;
    bool bSeriesExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Series > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Series ID: ");
    iSeriesID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(caSQL, "SELECT `Series ID` FROM `Book Series` "
                    "WHERE `Series ID` = %d", iSeriesID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Series ID: %d", iSeriesID);
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

    sprintf(caSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Series ID` = %d", iSeriesID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Series ID: %d", iSeriesID);
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
        sprintf(caSQL, "DELETE FROM `Book Series` "
                        "WHERE `Series ID` = %d", iSeriesID);

    if(mysql_query(conn, caSQL) != 0)
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
        printf("Series ID: %d deleted", iSeriesID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fDeleteGenre(char *sPrgNme)
{

    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iGenreID = 0;
    bool bGenreExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Genre > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Genre ID: ");
    iGenreID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(caSQL, "SELECT `Genre ID` FROM `Book Genres` "
                    "WHERE `Genre ID` = %d", iGenreID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Genre ID: %d", iGenreID);
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

    sprintf(caSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Genre ID` = %d", iGenreID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Genre ID: %d", iGenreID);
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
        sprintf(caSQL, "DELETE FROM `Book Genres` "
                        "WHERE `Genre ID` = %d", iGenreID);

        if(mysql_query(conn, caSQL) != 0)
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
        printf("Genre ID: %d deleted", iGenreID);
        printf("\n\n");
        fPressEnterToContinue();
    }

    return;
}

void fSearchGenres(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Genre ID`, `Genre Name` "
                    "FROM `Book Genres` "
                    "WHERE `Genre Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Genre found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteSource(char *sPrgNme)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iSourceID = 0;
    bool bSourceExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Source > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Source ID: ");
    iSourceID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(caSQL, "SELECT `Source ID` FROM `Book Sources` "
                    "WHERE `Source ID` = %d", iSourceID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Source ID: %d", iSourceID);
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

    sprintf(caSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Source ID` = %d", iSourceID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Source ID: %d", iSourceID);
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
        sprintf(caSQL, "DELETE FROM `Book Sources` "
                        "WHERE `Source ID` = %d", iSourceID);

    if(mysql_query(conn, caSQL) != 0)
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
        printf("Source ID: %d deleted", iSourceID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fSearchSources(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Source ID`, `Source Name` "
                    "FROM `Book Sources` "
                    "WHERE `Source Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Sources found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteRating(char *sPrgNme)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iRatingID = 0;
    bool bRatingExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Rating > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Rating ID: ");
    iRatingID = GetInt();

// execute the query to check if the Rating ID exists

    sprintf(caSQL, "SELECT `Rating ID` FROM `Book Ratings` "
                    "WHERE `Rating ID` = %d", iRatingID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Rating ID: %d", iRatingID);
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

    sprintf(caSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Rating ID` = %d", iRatingID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Rating ID: %d", iRatingID);
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
        sprintf(caSQL, "DELETE FROM `Book Ratings` "
                        "WHERE `Rating ID` = %d", iRatingID);
        if(mysql_query(conn, caSQL) != 0)
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
        printf("Rating ID: %d deleted", iRatingID);
        printf("\n\n");
        fPressEnterToContinue();
    }

    return;
}

void fSearchRatings(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Rating ID`, `Rating Name` "
                    "FROM `Book Ratings` "
                    "WHERE `Rating Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Ratings found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteStatus(char *sPrgNme)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iStatusID = 0;
    bool bStatusExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Status > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Status ID: ");
    iStatusID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(caSQL, "SELECT `Status ID` FROM `Book Statuses` "
                    "WHERE `Status ID` = %d", iStatusID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Status ID: %d", iStatusID);
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

    sprintf(caSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Status ID` = %d", iStatusID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Status ID: %d", iStatusID);
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
        sprintf(caSQL, "DELETE FROM `Book Statuses` "
                        "WHERE `Status ID` = %d", iStatusID);

        if(mysql_query(conn, caSQL) != 0)
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
        printf("Status ID: %d deleted", iStatusID);
        printf("\n\n");
        fPressEnterToContinue();
     }

    return;
}

void fSearchStatuses(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Status ID`, `Status Name` "
                    "FROM `Book Statuses` "
                    "WHERE `Status Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Status found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fDeleteClassification(char *sPrgNme)
{
    char *sServer = "192.168.0.13";
    char *sUser = "gjarman";
    char *sPassword = "Mpa4egu$";
    char *sDatabase = "risingfast";
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iClassificationID = 0;
    bool bClassificationExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Classifications > Delete");
    printf("\n\n");

// get an Author ID to delete

    printf("Classification ID: ");
    iClassificationID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(caSQL, "SELECT `Classification ID` FROM `Book Classifications` "
                    "WHERE `Classification ID` = %d", iClassificationID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Classification ID: %d", iClassificationID);
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

    sprintf(caSQL, "SELECT `Title ID` FROM `Book Titles` "
                    "WHERE `Classification ID` = %d", iClassificationID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned > 0)
    {
        bTitlesExist = true;
        printf("\n");
        printf("Titles exists for Classification ID: %d", iClassificationID);
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
        sprintf(caSQL, "DELETE FROM `Book Classifications` "
                        "WHERE `Classification ID` = %d", iClassificationID);

        if(mysql_query(conn, caSQL) != 0)
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
        printf("Classification ID: %d deleted", iClassificationID);
        printf("\n\n");
        fPressEnterToContinue();
    }

    return;
}

void fSearchClassifications(char *strSearchString)
{
    char caSQL[SQL_LEN] = {'\0'};
    int  iColCount = 0;
    int  iRowsReturned = 0;
    int  iRowsPrinted = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT  `Classification ID`, `Classification Name` "
                    "FROM `Book Classifications` "
                    "WHERE `Classification Name` LIKE '%%%s%%'", strSearchString);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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

    iColCount = mysql_num_fields(res);
    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Classification found");
    }

// print each row of results

    printf("\n");
    while((row = mysql_fetch_row(res)) && (iRowsReturned != 0))
    {
        iRowsPrinted++;
        printf("%-4s  %s", row[0], row[1]);
        printf("\n");
    }

    mysql_free_result(res);
    return;
}

void fUpdateAuthor(char *sPrgNme)
{
    char *sAuthorName = NULL;
    char *sEscapedAuthorName = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iAuthorID = 0;
    int  iAuthorMaxLength = 0;
    bool bAuthorExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

    iAuthorMaxLength = fGetFieldLength("Book Authors", "Author Name");

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Authors > Change");
    printf("\n\n");

// get an Author ID to change

    printf("Author ID: ");
    iAuthorID = GetInt();

// execute the query to check if the Author ID exists

    sprintf(caSQL, "SELECT `Author ID`"
                         ", `Author Name`"
                    "FROM `Book Authors` "
                    "WHERE `Author ID` = %d", iAuthorID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Author ID: %d", iAuthorID);
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
            printf("Changed Author Name (max %d chars) or E(x)it: ", iAuthorMaxLength);
            sAuthorName = fEscapeSingleQuote(GetString());
            if(strlen(sAuthorName) > iAuthorMaxLength)
            {
                printf("Author Name max %d chars. You entered %d chars", iAuthorMaxLength, (int) strlen(sAuthorName));
                printf("\n");
                sAuthorName[0] = '\0';
            }
        } while (strlen(sAuthorName) == 0);
    }

    if((bAuthorExists == true) && (toupper(sAuthorName[0]) == 'X') && (strlen(sAuthorName) == 1))
    {
        free(sAuthorName);
        return;
    }

// execute a query to change the author if the author exists

    if((bTitlesExist == false) && (bAuthorExists == true))
    {
        sprintf(caSQL, "UPDATE `Book Authors`"
                       " SET `Author Name` = '%s'"
                       " WHERE `Author ID` = %d", sAuthorName, iAuthorID);

        if(mysql_query(conn, caSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Author ID: %d updated", iAuthorID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    free(sAuthorName);
    mysql_free_result(res);
    return;
}

void fUpdateClassification(char *sPrgNme)
{
    char *sClassificationName = NULL;
    char *sClassificationNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iClassificationID = 0;
    int  iClassificationMaxLength = 0;
    bool bClassificationExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Classifications > Change");
    printf("\n\n");

// get an Classification ID to delete

    printf("Classification ID: ");
    iClassificationID = GetInt();

// execute the query to check if the Classification ID exists

    sprintf(caSQL, "SELECT `Classification ID`"
                         ", `Classification Name`"
                    "FROM `Book Classifications` "
                    "WHERE `Classification ID` = %d", iClassificationID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Classification ID: %d", iClassificationID);
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

        iClassificationMaxLength = fGetFieldLength("Book Classifications", "Classification Name");

        do {
            printf("Changed Classification Name (max %d chars): ", iClassificationMaxLength);
            sClassificationName = GetString();
            if(strlen(sClassificationName) > iClassificationMaxLength)
            {
                 printf("Classification Name max %d chars. You entered %d chars", iClassificationMaxLength, (int) strlen(sClassificationName));
                 printf("\n");
                 sClassificationName[0] = '\0';
            }
        } while(strlen(sClassificationName) == 0);
        
        sClassificationNameEscaped = fEscapeSingleQuote(sClassificationName);
    }

    if((bClassificationExists == true) && (toupper(sClassificationName[0]) == 'X') && (strlen(sClassificationName) == 1))
    {
        free(sClassificationName);
        return;
    }

// execute a query to change the classification if the classification exists

    if((bTitlesExist == false) && (bClassificationExists == true))
    {
        sprintf(caSQL, "UPDATE `Book Classifications`"
                       " SET `Classification Name` = '%s'"
                       " WHERE `Classification ID` = %d", sClassificationNameEscaped, iClassificationID);

        if(mysql_query(conn, caSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Classification ID: %d updated", iClassificationID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(sClassificationName);
    free(sClassificationNameEscaped);
    return;
}

void fUpdateRating(char *sPrgNme)
{
    char *sRatingName = NULL;
    char *sRatingNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iRatingID = 0;
    int  iRatingMaxLength = 0;
    bool bRatingExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Rating > Change");
    printf("\n\n");

// get an Rating ID to delete

    printf("Rating ID: ");
    iRatingID = GetInt();

// execute the query to check if the Rating ID exists

    sprintf(caSQL, "SELECT `Rating ID`"
                         ", `Rating Name`"
                    "FROM `Book Ratings` "
                    "WHERE `Rating ID` = %d", iRatingID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Rating ID: %d", iRatingID);
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

        iRatingMaxLength = fGetFieldLength("Book Ratings", "Rating Name");

        do {
            printf("Changed Rating Name (max %d chars) or E(x)it: ", iRatingMaxLength);
            sRatingName = GetString();
            if(strlen(sRatingName) > iRatingMaxLength)
            {
                 printf("Rating Name max %d chars. You entered %d chars", iRatingMaxLength, (int) strlen(sRatingName));
                 printf("\n");
                 sRatingName[0] = '\0';
            }
        } while(strlen(sRatingName) == 0);

        sRatingNameEscaped = fEscapeSingleQuote(sRatingName);

    }

    if((bRatingExists == true) && (toupper(sRatingName[0]) == 'X') && (strlen(sRatingName) == 1))
    {
        free(sRatingName);
        return;
    }


// execute a query to change the rating if the rating exists

    if((bTitlesExist == false) && (bRatingExists == true))
    {
        sprintf(caSQL, "UPDATE `Book Ratings`"
                       " SET `Rating Name` = '%s'"
                       " WHERE `Rating ID` = %d", sRatingNameEscaped, iRatingID);

        if(mysql_query(conn, caSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Rating ID: %d updated", iRatingID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(sRatingName);
    free(sRatingNameEscaped);
    return;
}

void fUpdateSeries(char *sPrgNme)
{
    char *sSeriesName = NULL;
    char *sSeriesNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iSeriesID = 0;
    int  iSeriesMaxLength = 0;
    bool bSeriesExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Series > Change");
    printf("\n\n");

// get an Series ID to delete

    printf("Series ID: ");
    iSeriesID = GetInt();

// execute the query to check if the Series ID exists

    sprintf(caSQL, "SELECT `Series ID`"
                         ", `Series Name`"
                    "FROM `Book Series` "
                    "WHERE `Series ID` = %d", iSeriesID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Series ID: %d", iSeriesID);
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

        iSeriesMaxLength = fGetFieldLength("Book Series", "Series Name");

        do {
            printf("Changed Series Name (max %d chars) or E(x)it: ", iSeriesMaxLength);
            sSeriesName = GetString();
            if (strlen(sSeriesName) > iSeriesMaxLength)
            {
                printf("Series Name max %d chars. You entered %d chars", iSeriesMaxLength, (int) strlen(sSeriesName));
                printf("\n");
                sSeriesName[0] = '\0';
            }
        } while(strlen(sSeriesName) == 0);

        sSeriesNameEscaped = fEscapeSingleQuote(sSeriesName);

    }

    if((bSeriesExists == true) && (toupper(sSeriesName[0]) == 'X') && (strlen(sSeriesName) == 1))
    {
        free(sSeriesName);
        return;
    }

// execute a query to change the series if the series exists

    if((bTitlesExist == false) && (bSeriesExists == true))
    {
        sprintf(caSQL, "UPDATE `Book Series`"
                       " SET `Series Name` = '%s'"
                       " WHERE `Series ID` = %d", sSeriesNameEscaped, iSeriesID);

        if(mysql_query(conn, caSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Series ID: %d updated", iSeriesID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(sSeriesName);
    free(sSeriesNameEscaped);
    return;
}

void fUpdateSource(char *sPrgNme)
{
    char *sSourceName = NULL;
    char *sSourceNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iSourceID = 0;
    int  iSourceMaxLength = 0;
    bool bSourceExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Source > Change");
    printf("\n\n");

// get an Source ID to delete

    printf("Source ID: ");
    iSourceID = GetInt();

// execute the query to check if the Source ID exists

    sprintf(caSQL, "SELECT `Source ID`"
                         ", `Source Name`"
                    "FROM `Book Sources` "
                    "WHERE `Source ID` = %d", iSourceID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Sourc ID: %d", iSourceID);
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

        iSourceMaxLength = fGetFieldLength("Book Sources", "Source Name");

        do {
            printf("Changed Source Name (max %d chars) or E(x)it: ", iSourceMaxLength);
            sSourceName = GetString();
            if (strlen(sSourceName) > iSourceMaxLength)
            {
                 printf("Source Name max %d chars. You entered %d chars", iSourceMaxLength, (int) strlen(sSourceName));
                 printf("\n");
                 sSourceName[0] = '\0';
            }
        } while(strlen(sSourceName) == 0);

        sSourceNameEscaped = fEscapeSingleQuote(sSourceName);

    }

    if((bSourceExists == true) && (toupper(sSourceName[0]) == 'X') && (strlen(sSourceName) == 1))
    {
        free(sSourceName);
        return;
    }

// execute a query to change the source if the source exists

    if((bTitlesExist == false) && (bSourceExists == true))
    {
        sprintf(caSQL, "UPDATE `Book Sources`"
                       " SET `Source Name` = '%s'"
                       " WHERE `Source ID` = %d", sSourceNameEscaped, iSourceID);

        if(mysql_query(conn, caSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            printf("Press enter to continue ");
        } else
        {
            printf("\n");
            printf("Source ID: %d updated", iSourceID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(sSourceName);
    free(sSourceNameEscaped);
    return;
}

void fUpdateStatus(char *sPrgNme)
{
    char *sStatusName = NULL;
    char *sStatusNameEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iStatusID = 0;
    int  iStatusMaxLength = 0;
    bool bStatusExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Status > Change");
    printf("\n\n");

// get an Status ID to delete

    printf("Status ID: ");
    iStatusID = GetInt();

// execute the query to check if the Status ID exists

    sprintf(caSQL, "SELECT `Status ID`"
                         ", `Status Name`"
                    "FROM `Book Statuses` "
                    "WHERE `Status ID` = %d", iStatusID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Status ID: %d", iStatusID);
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
        
        iStatusMaxLength = fGetFieldLength("Book Statuses", "Status Name");

        do {
            printf("Changed Status Name (max %d chars): ", iStatusMaxLength);
            sStatusName = GetString();
            if (strlen(sStatusName) > iStatusMaxLength)
            {
                 printf("Status Name max %d chars. You entered %d chars", iStatusMaxLength, (int) strlen(sStatusName));
                 printf("\n");
                 sStatusName[0] = '\0';
            }
        } while(strlen(sStatusName) == 0);

        sStatusNameEscaped = fEscapeSingleQuote(sStatusName);
    }

    if((bStatusExists == true) && (toupper(sStatusName[0]) == 'X') && (strlen(sStatusName) == 1))
    {
        free(sStatusName);
        return;
    }

// execute a query to change the Status if the status exists

    if((bTitlesExist == false) && (bStatusExists == true))
    {
        sprintf(caSQL, "UPDATE `Book Statuses`"
                       " SET `Status Name` = '%s'"
                       " WHERE `Status ID` = %d", sStatusNameEscaped, iStatusID);

        if(mysql_query(conn, caSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            printf("Press enter to continue ");
        } else
        {
            printf("\n");
            printf("Status ID: %d updated", iStatusID);
            printf("\n\n");
            printf("Press enter to continue ");
        }
    }

    mysql_free_result(res);
    free(sStatusName);
    free(sStatusNameEscaped);
    return;
}

void fUpdateGenre(char *sPrgNme)
{
    char *sGenreName = NULL;
    char *sGenreDesc = NULL;
    char *sGenreNameEscaped = NULL;
    char *sGenreDescEscaped = NULL;
    char caSQL[SQL_LEN] = {'\0'};
    int  iRowsReturned = 0;
    int  iGenreID = 0;
    int  iGenreNameMaxLength = 0;
    int  iGenreDescMaxLength = 0;
    bool bGenreExists = false;
    bool bTitlesExist = false;

    MYSQL_RES *res;
    MYSQL_ROW row;

// retitle the console

    fRetitleConsole(sPrgNme);
    printf("\n");
    printf("Main Menu > Maintain Attributes > Genres > Change");
    printf("\n\n");

// get an Genre ID to delete

    printf("Genre ID: ");
    iGenreID = GetInt();

// execute the query to check if the Genre ID exists

    sprintf(caSQL, "SELECT `Genre ID`"
                         ", `Genre Name`"
                         ", `Genre Description`"
                    "FROM `Book Genres` "
                    "WHERE `Genre ID` = %d", iGenreID);

    if(mysql_query(conn, caSQL) != 0)
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

    iRowsReturned = mysql_num_rows(res);

    if(iRowsReturned == 0)
    {
        printf("\n");
        printf("No matching Genre ID: %d", iGenreID);
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

        iGenreNameMaxLength = fGetFieldLength("Book Genres", "Genre Name");
        iGenreDescMaxLength = fGetFieldLength("Book Genres", "Genre Description");

        do {
            printf("Changed Genre Name (max %d chars) or E(x)it: ", iGenreNameMaxLength);
            sGenreName = GetString();
            if (strlen(sGenreName) > iGenreNameMaxLength)
            {
                 printf("Genre Name max %d chars. You entered %d chars", iGenreNameMaxLength, (int) strlen(sGenreName));
                 printf("\n");
                 sGenreName[0] = '\0';
            }
        } while(strlen(sGenreName) == 0);

        sGenreNameEscaped = fEscapeSingleQuote(sGenreName);

        if((toupper(sGenreName[0]) == 'X') && (strlen(sGenreName) == 1))
        {
            free(sGenreName);
            return;
        }

        do{
            printf("\n");
            printf("Changed Genre Description (max %d chars) or E(x)it: ", iGenreDescMaxLength);
            sGenreDesc = GetString();
            if (strlen(sGenreDesc) > iGenreDescMaxLength)
            {
                printf("Genre Desc max %d chars. You entered %d chars", iGenreDescMaxLength, (int) strlen(sGenreDesc));
                printf("\n");
                sGenreDesc[0] = '\0';
            }
        } while(strlen(sGenreDesc) == 0);

        sGenreDescEscaped = fEscapeSingleQuote(sGenreDesc);

        if((bGenreExists == true) && (toupper(sGenreDesc[0]) == 'X') && (strlen(sGenreDesc) == 1))
        {
            free(sGenreDesc);
            return;
        }

    }

// execute a query to change the genre if the genre exists

    if((bTitlesExist == false) && (bGenreExists == true))
    {
        sprintf(caSQL, "UPDATE `Book Genres`"
                       " SET `Genre Name` = '%s'"
                      ", `Genre Description` = '%s'"
                       " WHERE `Genre ID` = %d", sGenreNameEscaped, sGenreDescEscaped, iGenreID);

        if(mysql_query(conn, caSQL) != 0)
        {
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            fPressEnterToContinue();
        } else
        {
            printf("\n");
            printf("Genre ID: %d updated", iGenreID);
            printf("\n\n");
            fPressEnterToContinue();
        }
    }

    mysql_free_result(res);
    free(sGenreName);
    free(sGenreNameEscaped);
    return;
}

int  fGetFieldLength(char *strTableName, char *strFieldName)
{
    char caSQL[SQL_LEN] = {'\0'};
    int iMaxFieldLength = 0;

    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(caSQL, "SELECT CHARACTER_MAXIMUM_LENGTH"
                    " FROM information_schema.COLUMNS"
                    " WHERE TABLE_SCHEMA ='risingfast'"
                    " AND TABLE_NAME = '%s'"
                    " AND COLUMN_NAME = '%s'", strTableName, strFieldName);

// execute the query and check for no result

    if(mysql_query(conn, caSQL) != 0)
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
    iMaxFieldLength = (int) atol(row[0]);

    mysql_free_result(res);
    return iMaxFieldLength;
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
