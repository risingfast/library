//  rf50.h -- header file for custom functions
//  Author -- Geoffrey Jarman
//  Started -- 01-Jan-2021
//  Notes
//      03-Jan-2021 started
//      30-May-2021 add comments and toLowerStr()
//  Enhancements (0)

//  Converts an array or heap string to upper case.
//  Should not be used on a stack string which is immutable.

char *toUpperStr(char *);

//  converts an array or heap string to lower case.
//  Should not be used on a stack string which is immutable.

char *toLowerStr(char *);

//  clears the console and prints a title from a parameter

void fRetitleConsole(char *);

//  pauses the console with a message to press enter to continue

void fPressEnterToContinue(void);
