//  rf50.c -- library of functions not provided in any c library
//  Author -- Geoffey Jarman
//  Started -- 03-Jan-2021
//  Notes:
//      Add function definitions to rf50.h as declarations
//      03-Jan-2021 started
//      03-Jan-2021 reviewed all
//      18-Mar-2021 librarycode
//      30-May-2021 rewrite toUppseStr and toLowerStr
//      14-Jun-2021 add fRetitleConsole(char *)
//      14-Jun-2021 add fPressEnterToContinue(void)
//  Enhancements (0)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void fRetitleConsole(char *strPrgNme)
{
    (void) system("clear");
    printf("%s", strPrgNme);
    printf("\n");
    return;
}

char *toUpperStr(char *s)
{
    int i = 0;

    while(s[i] != '\0')
    {
        s[i] = toupper(s[i]);
        i++;
    }
    return s;
}

char *toLowerStr(char *s)
{
    int i = 0;

    while(s[i] != '\0')
    {
        s[i] = tolower(s[i]);
        i++;
    }
    return s;
}

void fPressEnterToContinue(void)
{
    printf("Press enter to continue ");
    while(getchar() != '\n')
    {
        continue;
    }
    return;
}

