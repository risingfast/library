/*  bookTest.c - test script for book.*.cgi programs
 *  Author: Geoffrey Jarman
 *  Started: 12-Oct-2022
 *  References:
 *      no references
 *  Log:
 *      12-Oct-2022 started
 *  Enhancements:
 *      no enhancements
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

    system("clear");

        unsetenv("QUERY_STRING");
        printf("-- bookVldtStatusId.cgi Test with QUERY_STRING = NULL -- Type any key to continue: ");
        getchar();
        system("./bookVldtStatusId.cgi");

        setenv("QUERY_STRING", "", 1);
        printf("-- bookVldtStatusId.cgi Test with QUERY_STRING = "" -- Type any key to continue: ");
        getchar();
        system("./bookVldtStatusId.cgi");

        setenv("QUERY_STRING", "sourceID=5", 1);
        printf("-- bookVldtStatusId.cgi Test with QUERY_STRING = %s -- Type any key to continue: ", getenv("QUERY_STRING"));
        getchar();
        system("./bookVldtStatusId.cgi");

        return EXIT_SUCCESS;
}
