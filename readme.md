Author -- Geoffrey Jarman<br/>
Started -- 12-Apr-2020<br/>
References:<br/>
    https://qnaplus.com/how-to-access-mysql-database-from-c-program/<br/>
Compliation:<br/>
    gcc -g -o mysql-c-library $(mysql_config --cflags) mysql-c-library.c $(mysql_config --libs) cs50.c<br/>
Log:<br/>
    12-Apr-2021 started<br/>
    12-Apr-2021 reviewed all<br/>
    12-Apr-2021 consolecode<br/>
    12-Apr-2021 function to print titles and authors<br/>
    13-Apr-2021 function to add attribute - shell outline<br/>
    14-Apr-2021 functions to add new attributes<br/>
    15-Apr-2021 function to title with unassigned attribute for foreign keys<br/>
    15-Apr-2021 title and attribute maintenance start<br/>
    16-Apr-2021 attribute descriptions<br/>
    17-Apr-2021 restriction on valid title ID range to enter to lookup a title<br/>
    18-Apr-2021 function fUpdateTitleAndAttributes() to change attributes<br/>
    18-Apr-2021 function to show numbered attributes<br/>
    19-Apr-2021 function to show numbered characters on the console<br/>
    19-Apr-2021 function to add characters<br/>
    20-Apr-2021 function to search title names<br/>
    20-Apr-2021 function to clear the screen<br/>
    21-Apr-2021 make X a universal exit key<br/>
    22-Apr-2021 x exit shortcut for options after 'maintain attributes' on the main menu<br/>
    22-Apr-2021 standard menus with character shortcuts<br/>
    23-Apr-2021 fShow functions on Attribute updates for classification and rating<br/>
    25-Apr-2021 narrow list layout for titles<br/>
    25-Apr-2021 start of character maintenance and debugging<br/>
    25-Apr-2021 update and delete characters<br/>
    25-Apr-2021 option to list titles descending<br/>
    25-Apr-2021 character count on lists<br/>
    25-Apr-2021 title name on attribute update page<br/>
    26-Apr-2021 title name on character update page<br/>
    26-Apr-2021 escape the ' character in strings<br/>
    26-Apr-2021 check a title exists in title ID queries<br/>
    27-Apr-2021 paginate long list: authors and series<br/>
    27-Apr-2021 explicit returns on all functions<br/>
    27-Apr-2021 show attribute value choices when updating attributes<br/>
    27-Apr-2021 escape option from adding attributes<br/>
    28-Apr-2021 fix bug entering a bad date on attribute maintenance<br/>
    28-Apr-2021 show all attributes and characters for in a single title<br/>
    28-Apr-2021 allow nulling out the dates on a title in Attribute Updates<br/>
    28-Apr-2021 remove unused option choices<br/>
    28-Apr-2021 list authors and title counts<br/>
    28-Apr-2021 format SQl in sprintf statements<br/>
    03-May-2021 remove double 'Press enter to continue" messages when print block ends<br/>
    03-May-2021 remove double prompt for Title search adding characters<br/>
    03-May-2021 remove double prompt for Title search updating attributes<br/>
    03-May-2021 modify getchar() to clear the buffer<br/>
    03-May-2021 search or list authors adding attributes<br/>
    04-May-2021 delete for attribute maintenance on authors<br/>
    04-May-2021 change add to maintain attributes<br/>
    04-May-2021 search or list Series adding attributes<br/>
    04-May-2021 add a title count to author listings<br/>
    04-May-2021 add a title count to series listings<br/>
    05-May-2021 search, delete functions to attribute maintenance for Genres<br/>
    05-May-2021 search, delete functions to attribute maintenance for Sources<br/>
    05-May-2021 search, delete functions to attribute maintenance for Ratings<br/>
    05-May-2021 search, delete functions to attribute maintenance for Statuses<br/>
    06-may-2021 search, delete functions to attribute maintenance for Classifications<br/>
    06-May-2021 troubleshoot connection statements in functions<br/>
    07-May-2021 fEscapeSingleQuotes on adding comments<br/>
    07-May-2021 error trap for mysql_query() errors<br/>
    08-May-2021 finish adding a change option to attribute maintenance<br/>
    09-May-2021 allow nulls start date on new title<br/>
    09-May-2021 add status filters to title listing<br/>
    09-May-2021 escape changing an author name with an embedded '<br/>
    09-May-2021 dynamically size names in listings<br/>
    09-May-2021 left-justify number columns in listings past Author listing<br/>
    10-May-2021 streamline error printing with __func__<br/>
    10-May-2021 streamline error detection blocks<br/>
    11-May-2021 block adding empty attributes by hitting enter without text<br/>
    12-May-2021 debug fEscapeSingleQuote() function<br/>
    12-May-2021 continue debug of fEscapeSingleQuote() function<br/>
    21-May-2021 trouble-shoot fEscapeSingleQuote() function<br/>
    21-May-2021 apply fEscapeSingleQuotes() function to updating an existing title name<br/>
    22-May-2021 add check for db service connection as fTestDbConnection();<br/>
    22-May-2021 add a reporting option to list all books<br/>
    22-May-2021 add decodes to option settings<br/>
    23-May-2021 function to fetch max field length of a column<br/>
    23-May-2021 eliminate bad foratting on trailing EOL's<br/>
    23-May-2021 impelement __func__ in printf statements where not already done<br/>
    23-May-2021 add length limits updating authors and classification attributes<br/>
    24-May-2021 add char limit checks on updating ratings and other text fields<br/>
    25-May-2021 add length limit to character maintenance<br/>
    25-May-2021 add options to reset all options to default values or print values<br/>
    25-May-2021 add char limit on changing characters<br/>
    25-May-2021 add Genre Description to change fields in attribute updates<br/>
    26-May-2021 add exits for author changes and attribute changes<br/>
    27-May-2021 add exits for title and attribute updates<br/>
    27-May-2021 add or confirm toupper() check for all E(x)it tests<br/>
    27-May-2021 add exits for option choices<br/>
    28-May-2021 fix bAttributeQuery equality bug in updating attributes on a title<br/>
    08-Jun-2021 replace return 0 or 1 with return EXIT_SUCCESS and return EXIT_FAILURE<br/>
Enhancements:<br/>
<br/>
