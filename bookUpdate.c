/*  bookUpdate.c -- maintain the contents of the formBook.dat file as a cgi
 *  Author: Geoffrey Jarman
 *  Started: 09/21/2020
 *  Ref:
 *  Log:
 *      21-Sep-2020 start by copying and changing bookShow.c
 *      24-Sep-2020 add GET vs POST logic to update the file
 *      25-Sep-2020 handle %0A url encoded newlines in POST process
 *      15-Sep-2022 add Access-Control-Allow-Header: * CORS http header
 *  Enhancements:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char** argv) {

    FILE *fptr;
    int counter = 0;
    int i = 0;
    int j = 0;
    char cyBuffer[1000] = {'\0'};
    char *strBuffer;

// print the html page content type header and CORS header required for cgi ----------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// identify GET or POST ---------------------------------------------------------------------------

    if (strcmp(getenv("REQUEST_METHOD"), "GET") == 0) {

// process the GET - open formBook.dat for reading ------------------------------------------------

        if ((fptr = fopen("formBook.dat", "r")) == NULL) {
            getcwd(cyBuffer, 1000);
            printf("Cannot open formBook.dat for reading in \"%s\" so terminating\n", cyBuffer);
            return -1;
        };
    
// print the contents of the datafile to stdout ---------------------------------------------------

        while ((strBuffer = fgets(cyBuffer, 1000, fptr)) != NULL) {
            printf("%s", cyBuffer);
        }
    }

// process the POST - write stdin to a file -------------------------------------------------------

    else if (strcmp(getenv("REQUEST_METHOD"), "POST") == 0) {
    
        FILE *fp1 = fopen("formBook.dat", "w");
        char arrWords1[100000] = {'\0'};
        char arrWords2[100000] = {'\0'};

        fgets(arrWords1, 100000, stdin);
        
        for(i = 0, j = 0; i <= strlen(arrWords1); i++, j++) {
            if (arrWords1[i] == '%' && arrWords1[i+1] == '0' && arrWords1[i+2] == 'A') {
                arrWords2[j] = '\n';
                i = i + 2;
            }
            else {
                arrWords2[j] = arrWords1[i];
            }
        }

        fputs(arrWords2, fp1);
        fputs("\n", fp1);
        
        fclose(fp1);
    }
    return 0;
}
