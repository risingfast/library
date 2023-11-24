#### Project
    mysql-c-library
#### Description
    Console c program using mysql C api to track books in a personal reading library
#### Author
    Geoffrey Jarman
#### Started
    12-Apr-2022
#### References:
    https://qnaplus.com/how-to-access-mysql-database-from-c-program/
#### Prerequisites:
    None
#### Installation:
    Create a local repository and clone down
    Run ./mysql-c-library from the local directory
#### Compliation:
    make, or
    gcc -g -o mysql-c-library $(mysql_config --cflags) mysql-c-library.c $(mysql_config --libs) cs50.c rf50.c
#### Enhancements: (0)
    Leave Book ID field whem mode change
    Add a plot field to books
    Select an image each time the risingfast site is opened
    Add an app to maintain cornerImage subtitles
#### Log:
    12-Apr-2022 started
    12-Apr-2022 reviewed all
    12-Apr-2022 consolecode
    12-Apr-2022 function to print titles and authors
    13-Apr-2022 function to add attribute - shell outline
    14-Apr-2022 functions to add new attributes
    15-Apr-2022 function to title with unassigned attribute for foreign keys
    15-Apr-2022 title and attribute maintenance start
    16-Apr-2022 attribute descriptions
    17-Apr-2022 restriction on valid title ID range to enter to lookup a title
    18-Apr-2022 function fUpdateTitleAndAttributes() to change attributes
    18-Apr-2022 function to show numbered attributes
    19-Apr-2022 function to show numbered characters on the console
    19-Apr-2022 function to add characters
    20-Apr-2022 function to search title names
    20-Apr-2022 function to clear the screen
    21-Apr-2022 make X a universal exit key
    22-Apr-2022 x exit shortcut for options after 'maintain attributes' on the main menu
    22-Apr-2022 standard menus with character shortcuts
    23-Apr-2022 fShow functions on Attribute updates for classification and rating
    25-Apr-2022 narrow list layout for titles
    25-Apr-2022 start of character maintenance and debugging
    25-Apr-2022 update and delete characters
    25-Apr-2022 option to list titles descending
    25-Apr-2022 character count on lists
    25-Apr-2022 title name on attribute update page
    26-Apr-2022 title name on character update page
    26-Apr-2022 escape the ' character in strings
    26-Apr-2022 check a title exists in title ID queries
    27-Apr-2022 paginate long list: authors and series
    27-Apr-2022 explicit returns on all functions
    27-Apr-2022 show attribute value choices when updating attributes
    27-Apr-2022 escape option from adding attributes
    28-Apr-2022 fix bug entering a bad date on attribute maintenance
    28-Apr-2022 show all attributes and characters for in a single title
    28-Apr-2022 allow nulling out the dates on a title in Attribute Updates
    28-Apr-2022 remove unused option choices
    28-Apr-2022 list authors and title counts
    28-Apr-2022 format SQl in sprintf statements
    03-May-2022 remove double 'Press enter to continue" messages when print block ends
    03-May-2022 remove double prompt for Title search adding characters
    03-May-2022 remove double prompt for Title search updating attributes
    03-May-2022 modify getchar() to clear the buffer
    03-May-2022 search or list authors adding attributes
    04-May-2022 delete for attribute maintenance on authors
    04-May-2022 change add to maintain attributes
    04-May-2022 search or list Series adding attributes
    04-May-2022 add a title count to author listings
    04-May-2022 add a title count to series listings
    05-May-2022 search, delete functions to attribute maintenance for Genres
    05-May-2022 search, delete functions to attribute maintenance for Sources
    05-May-2022 search, delete functions to attribute maintenance for Ratings
    05-May-2022 search, delete functions to attribute maintenance for Statuses
    06-May-2022 search, delete functions to attribute maintenance for Classifications
    06-May-2022 troubleshoot connection statements in functions
    07-May-2022 fEscapeSingleQuotes on adding comments
    07-May-2022 error trap for mysql_query() errors
    08-May-2022 finish adding a change option to attribute maintenance
    09-May-2022 allow nulls start date on new title
    09-May-2022 add status filters to title listing
    09-May-2022 escape changing an author name with an embedded '
    09-May-2022 dynamically size names in listings
    09-May-2022 left-justify number columns in listings past Author listing
    10-May-2022 streamline error printing with __func__
    10-May-2022 streamline error detection blocks
    09-May-2022 remove (null) from the date fields
    11-May-2022 block adding empty attributes by hitting enter without text
    12-May-2022 debug fEscapeSingleQuote() function
    12-May-2022 continue debug of fEscapeSingleQuote() function
    21-May-2022 trouble-shoot fEscapeSingleQuote() function
    21-May-2022 apply fEscapeSingleQuotes() function to updating an existing title name
    22-May-2022 add check for db service connection as fTestDbConnection();
    22-May-2022 add a reporting option to list all books
    22-May-2022 add decodes to option settings
    23-May-2022 function to fetch max field length of a column
    23-May-2022 eliminate bad foratting on trailing EOL's
    23-May-2022 impelement __func__ in printf statements where not already done
    23-May-2022 add length limits updating authors and classification attributes
    24-May-2022 add char limit checks on updating ratings and other text fields
    25-May-2022 add length limit to character maintenance
    25-May-2022 add options to reset all options to default values or print values
    25-May-2022 add char limit on changing characters
    25-May-2022 add Genre Description to change fields in attribute updates
    26-May-2022 add exits for author changes and attribute changes
    27-May-2022 add exits for title and attribute updates
    27-May-2022 add or confirm toupper() check for all E(x)it tests
    27-May-2022 add exits for option choices
    28-May-2022 fix bAttributeQuery equality bug in updating attributes on a title
    08-Jun-2022 replace return 0 or 1 with return EXIT_SUCCESS and return EXIT_FAILURE
    14-Jun-2022 separate functions into rf50.c and rf50.h for retitling console and pausing console
    16-Jun-2022 new makefile
    16-Jun-2022 migrate to global mysql variables
    16-Jun-2022 get password from console
    17-Jun-2022 eliminate duplicate mysql_free_result() calls in fDelete attribute calls
    17-Jun-2022 troubleshoot bug with changing ratings using invalid rating ID
    17-Jun-2022 fix bug on return path addting attributes
    18-Jun-2022 create function for main menu
    18-Jun-2022 create function for attributes maintenance menu
    18-Jun-2022 shorten variable name pefixes
    29-Aug-2022 make clean and rebuild
    17-Dec-2022 update to current builds
    17-Dec-2022 update series
    20-Dec-2022 update series of changes
    31-Dec-2022 backup current version
    07-Jan-2022 backup current version
    11-Feb-2022 backup current version
    03-May-2022 backup current version
    09-May-2022 rename mysql-c-library to library locally and on github
    14-May-2022 backup current version
    20-May-2022 backup current version
    27-May-2022 backup current version
    03-Jun-2022 backup current version
    11-Jun-2022 backup current version
    18-Jun-2022 backup current version
    24-Jun-2022 backup current version
    02-Jul-2022 backup current version
    08-Jul-2022 backup current version
    16-Jul-2022 backup current version
    23-Jul-2022 backup current version
    30-Jul-2022 backup current version
    04-Aug-2022 backup current version
    04-Aug-2022 backup current version - second run
    06-Aug-2022 backup current version
    12-Aug-2022 backup current version
    19-Aug-2022 backup current version
    26-Aug-2022 backup current version
    02-Sep-2022 backup current version
    09-Sep-2022 backup current version
    16-Sep-2022 backup current version
    23-Sep-2022 backup current version
    30-Sep-2022 backup current version
    07-Oct-2022 backup current version
    14-Oct-2022 backup current version
    17-Oct-2022 backup current version
    28-Oct-2022 backup current version
    04-Nov-2022 backup current version
    18-Nov-2022 backup current version
    25-Nov-2022 backup current version
    02-Dec-2022 backup current version
    09-Dec-2022 backup current version
    16-Dec-2022 backup current version
    23-Dec-2022 backup current version
    30-Dec-2022 backup current version
    06-Jan-2023 backup current version
    13-Jan-2023 backup current version
    20-Jan-2023 backup current version
    27-Jan-2023 backup current version
    03-Feb-2023 backup current version
    10-Feb-2023 backup current version
    17-Feb-2023 backup current version
    24-Feb-2023 backup current version
    03-Mar-2023 backup current version
    10-Mar-2023 backup current version
    17-Mar-2023 backup current version
    24-Mar-2023 backup current version
    31-Mar-2023 backup current version
    07-Apr-2023 backup current version
    14-Apr-2023 backup current version
    21-Apr-2023 backup current version
    28-Apr-2023 backup current version
    05-Map-2023 backup current version
    12-Map-2023 backup current version
    19-Map-2023 backup current version
    26-Map-2023 backup current version
    02-Jun-2023 backup current version
    09-Jun-2023 backup current version
    16-Jun-2023 backup current version
    23-Jun-2023 backup current version
    30-Jun-2023 backup current version
    07-Jul-2023 backup current version
    14-Jul-2023 backup current version
    21-Jul-2023 backup current version
    28-Jul-2023 backup current version
    04-Aug-2023 backup current version
    25-Aug-2023 backup current version
    01-Sep-2023 backup current version
    08-Sep-2023 backup current version
    15-Sep-2023 backup current version
    29-Sep-2023 backup current version
    03-Nov-2023 backup current version
    17-Nov-2023 backup current version
    24-Nov-2023 backup current version
