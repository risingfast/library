/*  bookShow.c -- show the contents of the formBook.dat file as a cgi
 *  Author: Geoffrey Jarman
 *  Started: 09/12/2020
 *  Ref:
 *  Log:
 *      09/12/2020 started
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
    char cyBuffer[1000] = {'\0'};
    char *strBuffer;

// print the html page content type and <head> block ----------------------------------------------

    printf("Content-type: text/html\n\n");
//    printf("<!DOCTYPE html>\n");
//    printf("<html lang=\"en\">\n");
//    printf("<head>\n");
//    printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"/rfstyles.css\">\n");
//    printf("<meta charset=\"UTF-8\">\n");
//    printf("<meta name=\"description\" content=\"CGI to show contents of formBook.dat\n");
//    printf("<meta name=\"keywords\" content=\"html, css, cgi\">\n");
//    printf("<meta name=\"generator\" content=\"vim\">\n");
//    printf("<meta name=\"author\" content=\"risingfast\">\n");
//    printf("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
//    printf("</head>\n");

// open formBook.dat for reading ------------------------------------------------------------------

    if ((fptr = fopen("formBook.dat", "r")) == NULL) {
        getcwd(cyBuffer, 1000);
        printf("Cannot open formBook.dat for reading in \"%s\" so terminating\n", cyBuffer);
        return -1;
    };
    
    while ((strBuffer = fgets(cyBuffer, 1000, fptr)) != NULL) {
        printf("%s", cyBuffer);
    }

    return 0;
}
