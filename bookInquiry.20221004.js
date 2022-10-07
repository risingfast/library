// bookInquiry.js -- script to implement js of bookInquiry.html
// Author: Geoff Jarman
// Started: 01-Nov-2021
// Log:
//    01-Nov-2021 started x
//    04-Nov-2-21 add a Book option
//    05-Nov-2021 add fGetBookDetails function
//    06-Nov-2021 add context to Books form modes
//    06-Nov-2021 clean up comments and code layout
//    08-Nov-2021 clear fields with mode changes
//    10-Nov-2021 add function to fetch book characters
//    10-Nov-2021 add characters text box
//    10-Nov-2021 reorganize the roles of fSetTopic and fSetMode
//    12-Nov-2021 add logic for topic divisions
//    13-Nov-2021 add logic to mode buttongs for all actions
//    14-Nov-2021 debug Fetch code for attributes
//    15-Nov-2021 add unreads logic
//    17-Nov-2021 implement book details
//    18-Nov-2021 fix the Clear button to revert to the Choose topic
//    19-Nov-2021 implement add on authors
//    19-Nov-2021 implement add on classifications
//    20-Nov-2021 implement add on ratings
//    21-Nov-2021 implement add on series
//    21-Nov-2021 implement add on sources
//    21-Nov-2021 implement add on genres
//    21-Nov-2021 implement add on statuses
//    22-Nov-2021 message area added
//    22-Nov-2021 book ID lookup on character listing
//    24-Nov-2021 add fSetElement() function
//    25-Nov-2021 extent fSetElement() function
//    25-Nov-2021 add fEnableSubmitIfNotNull()
//    28-Nov-2021 clear book inputs between book modes
//    28-Nov-2021 disable attribute areas on fetch
//    29-Nov-2021 add characters for a book
//    03-Dec-2021 change element naming
//    04-Dec-2021 add delete character
//    07-Dec-2021 add update character
//    07-Dec-2021 add delete rating
//    11-Dec-2021 add delete genre
//    12-Dec-2021 add delete status
//    16-Dec-2021 add update sources
//    17-Dec-2021 add update series
//    17-Dec-2021 add update ratings
//    17-Dec-2021 add update classifications
//    19-Dec-2021 add update authors
//    19-Dec-2021 add author to series listing
//    20-Dec-2021 fix bug adding characters
//    20-Dec-2021 add ID's to book details
//    26-Dec-2021 clear bookdiv elements
//    27-Dec-2021 fix add character bug
//    27-Dec-2021 add escape for '
//    27-Dec-2021 catalog functions
//    29-Dec-2021 consolidate _updt_id()
//    30-Dec-2021 replace ' with '' in char:add-name
//    30-Dec-2021 escape ' in updates
//    02-Jan-2022 fix character filer
//    04-Jan-2022 implement attribute LOV's
//    07-Jan-2022 add book
//    07-Jan-2022 return Title ID on add book
//    10-Jan-2022 protect ' character in book name and comments for adding a book
//    06-May-2022 make queries immediate
//    07-May-2022 definefUnwrapAllText()
//    08-May-2022 refresh LOV's()
//    09-May-2022 clear (null) from start and finish date and comments if empty in books
//    29-May-2022 make bookcomments editable in update mode
//    31-May-2022 set cornerimage rotation
//    06-Jun-2022 add fSetFocusOnSubmit()
//    18-Jun-2022 move fSetCornerImage() to common.js
//    02-Aug-2022 set focus on filter fields or book id field when new mode is chosen
//    08-Sep-2022 add fPrintBook()
//    09-Sep-2022 add fPrintBookText()
//    09-Sep-2022 remove fPrintBook() as no longer needed
//    16-Sep-2022 change www.risingfast.com to gjarman2020.com
//    22-Sep-2022 disable the submit button before adding a book
// Functions
//    fSetTopic() - set the current topic (Books, Titles, Recents etc) {
//    fSetMode(sNewMode) - set the current mode (Fetch, Query, Add, Update, Delete)
//    fonclick_submit_submit() -  fetch results from the server when Submit button is clicked
//    fonclick_chars_vldt_bk_id() - fetch book results from the server when Validate button is clicked
//    fonclick_chars_vldt_char_id() - fetch results from the server when Validate character is clicked
//    fGetBookDetails() - fetch a book detail record
//    fGetBookCharacters() - fetch book characters
//    fGetBookCharacters2() - fetch book characters and display in charactersArea
//    fGetBookTitle() - fetch book title
//    function fWrapText() -  set wrap on textarea
//    function fUnwrapAllText() -  set wrap off on all textarea
//    fEnableSubmitButton01() - validate Title ID entered before fetching a book record
//    fonKeyUp_chars_book_id() - enable the validate or submit buttons if an entered book ID is a valid format
//    fonKeyUp_chars_add_name() - to enable the validate or submit buttons if an entered book ID is a valid format
//    fonKeyUp_chars_updtd_name() - enable the submit button when a changed character is entered
//    fonKeyUp_class_updtd_name() - enable the submit button when a classification is changed
//    fonKeyUp_rating_updtd_name() - enable the submit button when a changed rating is entered
//    fonKeyUp_series_updtd_name() - enable the submit button when a changed series is entered
//    fonKeyUp_sources_updtd_name() - enable the submit button when a changed source is entered
//    fonKeyUp_genres_updtd_name() - enable the submit button when a changed genre is entered
//    fonKeyUp_statuses_updtd_name() - enable the submit button when a changed status is entered
//    fonKeyUp_chars_del_id() - enable the submit button if an entered character ID is a valid format
//    fonKeyUp_author_updtd_name() - enable the submit button when a changed author is entered
//    fonKeyUp_author_del_id() - enable the submit button if an entered classification ID is a valid format
//    fonKeyUp_class_del_id() - enable the submit button if an entered classification ID is a valid format
//    fonKeyUp_ratings_del_id() - enable the submit button if an entered rating ID (for deletion) is a valid format
//    fonKeyUp_series_del_id() - enable the submit button if an entered series ID is a valid format
//    fonKeyUp_sources_del_id() - enable the submit button if an entered source ID is a valid format
//    fonKeyUp_genres_del_id() - enable the submit button if an entered genre ID is a valid format
//    fonKeyUp_statuses_del_id() - enable the submit button if an entered status ID is a valid format
//    fEnableAllModeButtons() - enable all the mode buttons
//    fDisableBookFields() - disable book fields
//    fEnableBookFields() - enable book fields
//    fSetElement() - unhide, hide or disable a single HTML element
//    fUnhideMultiple() - unhide multiple elements
//    fClearPageToChoose() - clear the page and set the topic to 'Choose' to put the page in the initial state 
//    fDisableModeButton() - disable one mode button and enable all the other mode buttons
//    fEnableSubmitIfNotNull() - enable the submit button if a field is not empty
//    fClearBookElements() - clear all elements in books:div
//    fonKeyUp_any_updt_id() - enable the validate button if an entered ID is a valid format for validating an update
//    fonKeyUp_any_vldt_id() = validate an id for a single topic value and return the value
//    fFetchTopicList() -- fetch a list from the database for a single topic
//    fPrintBook() -- print a book
//    fPrintBook() -- print a book's text: title, author, abstract, characters

// define globals for URI's .............................................................................................

const uri01 = "http://gjarman2020.com/cgi-bin/bookInquiry.cgi";
const uri02 = "http://gjarman2020.com/cgi-bin/bookDetails2.cgi";
const uri03 = "http://gjarman2020.com/cgi-bin/bookCharacters.cgi";
const uri04 = "http://gjarman2020.com/cgi-bin/bookAddAuthor.cgi";
const uri05 = "http://gjarman2020.com/cgi-bin/bookAddClassification.cgi";
const uri06 = "http://gjarman2020.com/cgi-bin/bookAddRating.cgi";
const uri07 = "http://gjarman2020.com/cgi-bin/bookAddSeries.cgi";
const uri08 = "http://gjarman2020.com/cgi-bin/bookAddSource.cgi";
const uri09 = "http://gjarman2020.com/cgi-bin/bookAddGenre.cgi";
const uri10 = "http://gjarman2020.com/cgi-bin/bookAddStatus.cgi";
const uri11 = "http://gjarman2020.com/cgi-bin/bookTitleID.cgi";
const uri12 = "http://gjarman2020.com/cgi-bin/bookAddCharacter.cgi";
const uri13 = "http://gjarman2020.com/cgi-bin/bookDelCharacter.cgi";
const uri14 = "http://gjarman2020.com/cgi-bin/bookVldtCharId.cgi";
const uri15 = "http://gjarman2020.com/cgi-bin/bookChgCharNme.cgi";
const uri16 = "http://gjarman2020.com/cgi-bin/bookDelClassification.cgi";
const uri17 = "http://gjarman2020.com/cgi-bin/bookDelRating.cgi";
const uri18 = "http://gjarman2020.com/cgi-bin/bookDelSeries.cgi";
const uri19 = "http://gjarman2020.com/cgi-bin/bookDelSource.cgi";
const uri20 = "http://gjarman2020.com/cgi-bin/bookDelGenre.cgi";
const uri21 = "http://gjarman2020.com/cgi-bin/bookDelStatus.cgi";
const uri22 = "http://gjarman2020.com/cgi-bin/bookDelAuthor.cgi";
const uri23 = "http://gjarman2020.com/cgi-bin/bookVldtStatusId.cgi";
const uri24 = "http://gjarman2020.com/cgi-bin/bookChgStatusNme.cgi";
const uri25 = "http://gjarman2020.com/cgi-bin/bookVldtGenreId.cgi";
const uri26 = "http://gjarman2020.com/cgi-bin/bookChgGenreNme.cgi";
const uri27 = "http://gjarman2020.com/cgi-bin/bookVldtSourceId.cgi";
const uri28 = "http://gjarman2020.com/cgi-bin/bookChgSourceNme.cgi";
const uri29 = "http://gjarman2020.com/cgi-bin/bookVldtSeriesId.cgi";
const uri30 = "http://gjarman2020.com/cgi-bin/bookChgSeriesNme.cgi";
const uri31 = "http://gjarman2020.com/cgi-bin/bookVldtRatingId.cgi";
const uri32 = "http://gjarman2020.com/cgi-bin/bookChgRatingNme.cgi";
const uri33 = "http://gjarman2020.com/cgi-bin/bookVldtClassId.cgi";
const uri34 = "http://gjarman2020.com/cgi-bin/bookChgClassNme.cgi";
const uri35 = "http://gjarman2020.com/cgi-bin/bookVldtAuthorId.cgi";
const uri36 = "http://gjarman2020.com/cgi-bin/bookChgAuthorNme.cgi";
const uri37 = "http://gjarman2020.com/cgi-bin/bookAddBook.cgi";
const uri38 = "http://gjarman2020.com/cgi-bin/bookDelBook.cgi";
const uri39 = "http://gjarman2020.com/cgi-bin/bookUpdtBook.cgi";

// define globals for mode and topic variables .........................................................................

let sMode = '';
let sTopic = '';

// define globals for LOV arrays for book attributes....................................................................

let arrAuthorNames = [];
let arrSourceNames = [];
let arrSeriesNames = [];
let arrGenreNames = [];
let arrStatusNames = [];
let arrClassificationNames = [];
let arrRatingNames = [];

// populate global lists of value array for book attributes.............................................................

fPopulateLOV('authors');
fPopulateLOV('sources');
fPopulateLOV('series');
fPopulateLOV('genres');
fPopulateLOV('statuses');
fPopulateLOV('classifications');
fPopulateLOV('ratings');

// set the topic .......................................................................................................

function fSetTopic() {

    let tc = document.getElementById("topics-select");

    if (tc.value === "choose") {
        sTopic = "choose";
    } else if (tc.value === "books") {
        fUnhideMultiple("books-div", "modes-div", "submit-div");
        sTopic = "books";
    } else if (tc.value === "titles") {
        fUnhideMultiple("titles-div", "modes-div", "submit-div");
        sTopic = "titles";
    } else if (tc.value === "characters") {
        fUnhideMultiple("characters-div", "modes-div", "submit-div");
        sTopic = "characters";
    } else if (tc.value === "authors") {
        sTopic = "authors";
    } else if (tc.value === "recents") {
        fUnhideMultiple("recents-div", "modes-div", "submit-div");
        sTopic = "recents";
    } else if (tc.value === "unreads") {
        fUnhideMultiple("unreads-div", "modes-div", "submit-div");
        sTopic = "unreads";
    } else if (tc.value === "classifications") {
        fUnhideMultiple("classifications-div", "modes-div", "submit-div");
        sTopic = "classifications";
    } else if (tc.value === "ratings") {
        fUnhideMultiple("ratings-div", "modes-div", "submit-div");
        sTopic = "ratings";
    } else if (tc.value === "series") {
        fUnhideMultiple("series-div", "modes-div", "submit-div");
        sTopic = "series";
    } else if (tc.value === "sources") {
        fUnhideMultiple("sources-div", "modes-div", "submit-div");
        sTopic = "sources";
    } else if (tc.value === "genres") {
        fUnhideMultiple("genres-div", "modes-div", "submit-div");
        sTopic = "genres";
    } else if (tc.value === "statuses") {
        fUnhideMultiple("statuses-div", "modes-div", "submit-div");
        sTopic = "statuses";
    }

    // set the initial mode for any change to another topic change .....................................................

    fSetMode("fetch");
    fonclick_submit_submit();
}

// functon to set the mode which enables fields and behaviours .........................................................

function fSetMode(sNewMode) {

    sTopic = document.getElementById("topics-select").value;
    sMode = sNewMode;

    fClearPage();

    if (sTopic === "choose") {
        ;
    } else if (sTopic === 'books') {

        fUnhideMultiple("modes-div", "submit-div");
        fSetElement("Disable", "submit-button");
        fClearBookDivElements();
        fSetElement("Enable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("booksid-input").style.width = "35px";

            //  disable all book fields except Book ID .................................................................

            fDisableBookFields("booksid-input", "booksname-input", "booksauthor-input", "booksauthor-select", "bookssource-input", "bookssource-select"
                             , "booksseries-input", "booksseries-select", "booksgenre-input", "booksgenre-select", "booksstatus-input", "booksstatus-select"
                             , "booksclassification-input", "booksclassification-select", "booksrating-input", "booksrating-select", "booksstart-input", "booksfinish-input"
                             , "booksabstract-textarea", "bookscomments-textarea", "bookscharacters-textarea");
            fSetElement("Unhide", "books-div"); 

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter the Book ID and 'Submit'";
            document.getElementById("booksstage-input").value = 'Nothing Fetched';

            //  enable the ID field and color light yellow as a required field .........................................

            fSetElement("Clear", "booksid-input");

            let dt = document.getElementById("booksid-input");
            dt.disabled = false;
            dt.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color
            dt.focus();

        } else if (sMode === 'query') {

            //  disable the 'query' mode button and color it green .....................................................

            fDisableModeButton("modesquery-button");
            document.getElementById("mode-label").innerHTML = "query mode";

            //  enable all book fields for 'query' mode except for the books:TitleId and books:chrs fields ................

            fEnableBookFields("booksid-input", "booksname-input", "booksauthor-select", "bookssource-select", "booksseries-select", "booksgenre-select"
                            , "booksstatus-select", "booksclassification-select", "booksrating-select", "booksstart-input", "booksfinish-input"
                            , "booksabstract-textarea", "bookscomments-textarea", "bookscharacters-textarea");
            fSetElement("Disable", "booksid-input");
            fSetElement("Disable", "bookscharacters-textarea");
            fSetElement("Unhide", "books-div"); 
            let dt = document.getElementById("booksid-input");
            dt.style.backgroundColor = "rgba(230,239,239, 0.3)";                                          // white color

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter query values in any editable fields and 'Submit'";

        } else if (sMode === 'add') {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "add mode";

            //  enable all book fields for 'add' mode ..................................................................

            fEnableBookFields("booksid-input", "booksname-input", "booksauthor-select", "bookssource-select", "booksseries-select", "booksgenre-select"
                            , "booksstatus-select", "booksclassification-select", "booksrating-select", "booksstart-input", "booksfinish-input"
                            , "booksabstract-textarea", "booksabstract-textarea", "bookscomments-textarea", "bookscharacters-textarea");
            fSetElement("Unhide", "books-div"); 
            
            //  enable all book fields for 'query' mode except for the books:titleId and books:chrs fields ................

            fEnableBookFields("booksid-input", "booksname-input", "booksauthor-select", "bookssource-select", "booksseries-select", "booksgenre-select", "booksstatus-select", "booksclassification-select", "booksrating-select", "booksstart-input", "booksfinish-input", "booksabstract-textarea", "booksabstract-textarea", "bookscomments-textarea", "bookscharacters-textarea");
            fSetElement("Disable", "booksid-input");
            fSetElement("Disable", "bookscharacters-textarea");
            fSetElement("Unhide", "books-div"); 

            fSetElement("Clear", "booksid-input");
            
            fSetElement("Enable", "submit-button");
            document.getElementById("booksabstract-textarea").readOnly = false;
            document.getElementById("booksabstract-textarea").disable = false;
            document.getElementById("bookscomments-textarea").readOnly = false;
            document.getElementById("bookscomments-textarea").disable = false;
            let dt = document.getElementById("booksid-input");
            dt.style.backgroundColor = "rgba(239,239,239, 0.3)";                                     // light grey color

            // disable the submit button

            fSetElement("Disable", "submit-button");

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter values for a new book and 'Submit'";

            // set the initial value of book attributes to 'Unassigned' and get the matching ID

            document.getElementById("booksauthor-select").value = 'Unassigned';
            fPopulateLOVId('authors');
            
            document.getElementById("bookssource-select").value = 'Unassigned';
            fPopulateLOVId('sources');
            
            document.getElementById("booksseries-select").value = 'Unassigned';
            fPopulateLOVId('series');

            document.getElementById("booksgenre-select").value = 'Unassigned';
            fPopulateLOVId('genres');

            document.getElementById("booksstatus-select").value = 'Unassigned';
            fPopulateLOVId('statuses');

            document.getElementById("booksclassification-select").value = 'Unassigned';
            fPopulateLOVId('classifications');

            document.getElementById("booksrating-select").value = 'Unassigned';
            fPopulateLOVId('ratings');

            // set focus on the booksname-input field

            bookname = document.getElementById("booksname-input");
            bookname.focus();

        } else if (sMode === 'update') {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "update mode";

            //  disable all book fields except Book ID for initial fetch ...............................................

            fDisableBookFields("booksid-input", "booksname-input", "booksauthor-input", "booksauthor-select", "bookssource-input", "bookssource-select"
                             , "booksseries-input", "booksseries-select", "booksgenre-input", "booksgenre-select", "booksstatus-input", "booksstatus-select"
                             , "booksclassification-input", "booksclassification-select", "booksrating-input", "booksrating-select", "booksstart-input", "booksfinish-input"
                             , "booksabstract-textarea", "bookscomments-textarea", "bookscharacters-textarea");
            fSetElement("Unhide", "books-div"); 

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("booksstage-input").value = 'Nothing Fetched';
            document.getElementById("submit-message").value = "Enter the Book ID and 'Submit' to fetch book to update";

            //  enable the ID field and color light yellow as a required field .........................................

            fSetElement("Clear", "booksid-input");

            let dt = document.getElementById("booksid-input");
            dt.disabled = false;
            dt.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color
            dt.focus();

        } else if (sMode === 'delete') {

            //  disable all book fields except Book ID .................................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "delete mode";

            //  disable and unhide all book fields .....................................................................

            fDisableBookFields("booksname-input", "booksauthor-select", "bookssource-select", "booksseries-select", "booksgenre-select", "booksstatus-select"
                             , "booksclassification-select", "booksrating-select", "booksstart-input", "booksfinish-input"
                             , "booksabstract-textarea", "bookscomments-textarea", "bookscharacters-textarea");
            fSetElement("Enable", "booksid-input");
            fSetElement("Unhide", "books-div"); 
            let dt = document.getElementById("booksid-input");
            dt.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color
            dt.focus();
            
            // clear the books:titleId field ...........................................................................

            fSetElement("Clear", "booksid-input");

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter the Title ID and 'submit' to delete the book";
        }

    } else if (sTopic === "titles") {

        fUnhideMultiple("modes-div", "submit-div", "titles-div", "titlesarea-textarea");
        fSetElement("Clear", "titlesfilter-input");
        fSetElement("UnhideInline", "titlesfilter-input");
        fSetElement("Clear", "titlesarea-textarea");
        fSetElement("Disable", "submit-button");
        fSetElement("Disable", "printButton");
        fSetElement("Enable", "titlesfilter-input");
        document.getElementById("titlesfilter-input").focus();

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Enable", "submit-button");
            fSetElement("Disable", "modesadd-button");
            fSetElement("Disable", "modesupdate-button");
            fSetElement("Disable", "modesdelete-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "'Submit' to fetch titles (or set a filter, then 'Submit')";
            fUnwrapAllText();

        }

    } else if (sTopic === "recents") {

        fUnhideMultiple("modes-div", "submit-div", "recents-div", "recentsarea-textarea");
        fSetElement("Clear", "recentsfilter-input");
        fSetElement("UnhideInline", "recentsfilter-input");
        document.getElementById("recentsfilter-input").focus();
        fSetElement("Clear", "recentsarea-textarea");
        fSetElement("Disable", "submit-button");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Enable", "submit-button");
            fSetElement("Disable", "modesadd-button");
            fSetElement("Disable", "modesupdate-button");
            fSetElement("Disable", "modesdelete-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then click 'Submit')";
            fUnwrapAllText();
        }

    } else if (sTopic === "unreads") {

        fUnhideMultiple("modes-div", "submit-div", "unreads-div", "unreadsarea-textarea");
        fSetElement("Clear", "unreadsfilter-input");
        document.getElementById("unreadsfilter-input").focus();
        fSetElement("Clear", "unreadsarea-textarea");
        fSetElement("Disable", "submit-button");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Clear", "unreadsfilter-input");
            fSetElement("Clear", "unreadsarea-textarea");
            fSetElement("Enable", "submit-button");
            fSetElement("Disable", "modesadd-button");
            fSetElement("Disable", "modesupdate-button");
            fSetElement("Disable", "modesdelete-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then click 'Submit')";
            fUnwrapAllText();
        }

    } else if (sTopic === "characters") {

        fUnhideMultiple("modes-div", "submit-div", "characters-div", "charactersarea-textarea");
        fSetElement("Clear", "charactersfilter-input");
        fSetElement("UnhideInline", "charactersfilter-input");
        document.getElementById("charactersfilter-input").focus();
        fSetElement("Clear", "charactersarea-textarea");
        fSetElement("Disable", "submit-button");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fSetElement("Unhide", "submit-div");
            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "charactersvalidatebook-button");
            fSetElement("Unhide", "charactersfilter-div");
            fSetElement("Unhide", "charactersbook-div");
            fSetElement("Hide", "charactersdelete-div");
            fSetElement("Hide", "charactersadd-div");
            fSetElement("Hide", "charactersupdate-div");
            fSetElement("UnhideInline", "charactersvalidatebook-button");
            document.getElementById("charactersfilter-input").disabled = false;
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Enter the Book ID and click 'Submit' to fetch " + sTopic + " for a book";
            fUnwrapAllText();

            //  enable the Book ID field and color light yellow as a required field ....................................

            fSetElement("Clear", "charactersbookid-input");
            fSetElement("Clear", "charactersbooktitle-input");
            let bi = document.getElementById("charactersbookid-input");
            bi.disabled = false;
            bi.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color
            bi.style.borderWidth = "thin";

            // set focus on the Book ID field

            bi.focus();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "charactersfilter-div");
            fSetElement("Hide", "charactersdelete-div");
            fSetElement("Hide", "charactersupdate-div");
            fSetElement("Unhide", "charactersbook-div");
            fSetElement("Unhide", "charactersadd-div");
            fSetElement("UnhideInline", "charactersvalidatebook-button");
            fSetElement("Disable", "charactersadd-input");
            fSetElement("Clear", "charactersbookid-input");
            fSetElement("Clear", "charactersbooktitle-input");
            document.getElementById("charactersbookid-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter the Book ID and 'Validate' to enter characters for a book";

            // set focus on the Book ID field for quick entry

            let AddCharBookID = document.getElementById("charactersbookid-input");
            AddCharBookID.focus();

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Hide", "charactersdelete-div");
            fSetElement("Hide", "charactersbook-div");
            fSetElement("Hide", "charactersadd-div");
            fSetElement("Hide", "charactersfilter-div");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "charactersupdatename-input");
            fSetElement("Disable", "charactersupdatedname-input");
//            fSetElement("Disable", "chars:vldt-char-id");
            fSetElement("Unhide", "charactersupdate-div");
            fSetElement("Clear", "charactersupdateid-input");
            fSetElement("Clear", "charactersupdatename-input");
            document.getElementById("charactersupdateid-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("charactersupdateid-input").style.borderWidth = "thin";
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("submit-message").value = "Enter the Book ID and 'Validate' to update characters for a book";

            let UpdtCharID = document.getElementById("charactersupdateid-input");
            UpdtCharID.focus();


        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fSetElement("Hide", "charactersdelete-div");
            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Unhide", "charactersfilter-div");
            fSetElement("Hide", "charactersadd-div");
            fSetElement("Hide", "charactersbook-div");
            fSetElement("Hide", "charactersfilter-div");
            fSetElement("Hide", "charactersupdate-div");
            fSetElement("Unhide", "charactersdelete-div");
            fSetElement("Clear", "charactersdeleteid-input");
            let ci = document.getElementById("charactersdeleteid-input");
            ci.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color
            ci.style.borderWidth = "thin";
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("submit-message").value = "Enter the Character ID and 'Submit'";

            // set focus on the Book ID field for quick entry

            let DelCharID = document.getElementById("charactersdeleteid-input");
            DelCharID.focus();
        }

    } else if (sTopic === "authors") {

        fUnhideMultiple("modes-div", "submit-div", "authors-div", "authorslist-textarea");
        fSetElement("Clear", "authorsfilter-input");
        fSetElement("UnhideInline", "authorsfilter-input");
        document.getElementById("authorsfilter-input").focus();
        fSetElement("Clear", "authorslist-textarea");
        fSetElement("Unhide", "authorsadd-div");
        fSetElement("Clear", "seriesadd-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "authorsfilter-input");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Unhide", "authorsfilter-div");
            fSetElement("Hide", "authorsadd-div");
            fSetElement("Hide", "authorsupdate-div");
            fSetElement("Hide", "authorsdelete-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch authors (or set a filter, then 'Submit')";
            sInputFilter = "authorsfilter-input";
            sTextAreaResults = "authorslist-textarea";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "authorsfilter-input");
            fSetElement("Unhide", "authorsadd-div");
            fSetElement("Hide", "authorsfilter-div");
            fSetElement("Hide", "authorsupdate-div");
            fSetElement("Hide", "authorsdelete-div");
            document.getElementById("submit-message").value = "Enter the new author and click 'Submit'";
            document.getElementById("authorsadd-input").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("authorsadd-input").style.borderWidth = "thin";
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("authorsadd-input").focus();

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "authorsupdatename-input");
            fSetElement("Disable", "authorsupdated-input");
            fSetElement("Hide", "authorsfilter-div");
            fSetElement("Hide", "authorsadd-div");
            fSetElement("Unhide", "authorsupdate-div");
            fSetElement("Hide", "authorsdelete-div");
            fSetElement("Disable", "authorsfilter-input");
            document.getElementById("authorsupdateid-input").value = '';
            document.getElementById("authorsupdatename-input").value = '';
            document.getElementById("authorsupdateid-input").style.borderWidth = "thin";
            document.getElementById("authorsupdateid-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("submit-message").value = "Enter the existing author and click 'Submit'";
            document.getElementById("authorsupdateid-input").focus();

        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Unhide", "authorsdelete-div");
            fSetElement("Hide", "authorsfilter-div");
            fSetElement("Hide", "authorsadd-div");
            fSetElement("Hide", "authorsupdate-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("authorsdelete-input").value = '';
            document.getElementById("authorsdelete-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            document.getElementById("authorsdelete-input").style.borderWidth = "thin";
            document.getElementById("submit-message").value = "Enter the Author ID to delete and 'Submit'";
            document.getElementById("authorsdelete-input").focus();
        }

    } else if (sTopic === "classifications") {

        fUnhideMultiple("modes-div", "submit-div", "classifications-div", "classificationslist-textarea");
        fSetElement("Clear", "classificationsfilter-input");
        fSetElement("UnhideInline", "classificationsfilter-input");
        document.getElementById("classificationsfilter-input").focus();
        fSetElement("Clear", "classificationslist-textarea");
        fSetElement("Unhide", "classificationsadd-div");
        fSetElement("UnhideInline", "classificationsadd-input");
        fSetElement("Clear", "classificationsadd-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "classificationsfilter-input");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "classificationsadd-div");
            fSetElement("Hide", "classificationsdelete-div");
            fSetElement("Enable", "submit-button");
            fSetElement("Unhide", "classificationsfilter-div");
            fSetElement("Hide", "classificationsupdate-div");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch classification (or set a filter, then 'Submit')";
            sTextAreaResults = "classificationslist-textarea";
            sInputFilter = "classificationsfilter-input";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "classificationsfilter-div");
            fSetElement("Hide", "classificationsdelete-div");
            fSetElement("Hide", "classificationsupdate-div");
            document.getElementById("classificationsadd-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("classificationsadd-input").style.borderWidth = "thin";
            document.getElementById("classificationsadd-input").focus();
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter the new classification and click 'Submit'";


        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "classificationsadd-div");
            fSetElement("Unhide", "classificationsupdate-div");
            fSetElement("Hide", "classificationsdelete-div");
            fSetElement("Hide", "classificationsfilter-div");
            fSetElement("Disable", "classificationsupdatename-input");
            fSetElement("Disable", "classificationsupdated-input");
            document.getElementById("classificationsupdateid-input").value = '';
            document.getElementById("classificationsupdatename-input").value = '';
            document.getElementById("classificationsupdateid-input").style.borderWidth = "thin";
            document.getElementById("classificationsupdateid-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("classificationsupdateid-input").focus();
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("submit-message").value = "Enter the existing classification and click 'Submit'";


        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "classificationsadd-div");
            fSetElement("Disable", "classificationsfilter-input");
            fSetElement("Unhide", "classificationsdelete-div");
            fSetElement("Hide", "classificationsfilter-div");
            fSetElement("Hide", "classificationsupdate-div");
            document.getElementById("classificationsdelete-input").value = '';
            document.getElementById("classificationsdelete-input").style.backgroundColor = "rgb(255,255,224)";          // light yellow
            document.getElementById("classificationsdelete-input").style.borderWidth = "thin";
            document.getElementById("classificationsdelete-input").focus();
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("submit-message").value = "Enter the classification ID to delete and 'Submit'";
        }

    } else if (sTopic === "ratings") {

        fUnhideMultiple("modes-div", "submit-div", "ratings-div", "ratingslist-textarea");
        fSetElement("Clear", "ratingsfilter-div");
        fSetElement("UnhideInline", "ratingsfilter-div");
        document.getElementById("ratingsfilter-input").focus();
        fSetElement("Clear", "ratingslist-textarea");
        fSetElement("Unhide", "ratingsadd-div");
        fSetElement("Clear", "ratingsadd-input");
        fSetElement("UnhideInline", "ratingsadd-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "ratingsfilter-div");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "ratingsadd-div");
            fSetElement("Hide", "ratingsupdate-div");
            fSetElement("Hide", "ratingsdelete-div");
            fSetElement("Unhide", "ratingsfilter-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch ratings (or set a filter, then click 'Submit')";
            sTextAreaResults = "ratingslist-textarea";
            sInputFilter = "ratingsfilter-div";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Hide", "ratingsfilter-div");
            fSetElement("Hide", "ratingsupdate-div");
            fSetElement("Hide", "ratingsdelete-div");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "ratingsfilter-div");
            document.getElementById("ratingsadd-input").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("ratingsadd-input").style.borderWidth = "thin";
            document.getElementById("ratingsadd-input").focus();
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter the new rating and click 'Submit'";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "ratingsfilter-div");
            fSetElement("Hide", "ratingsfilter-div");
            fSetElement("Hide", "ratingsadd-div");
            fSetElement("Unhide", "ratingsupdate-div");
            fSetElement("Disable", "ratingsupdated-input");
            fSetElement("Hide", "ratingsdelete-div");
            document.getElementById("ratingsupdateid-input").value = '';
            document.getElementById("ratingsupdateid-input").focus();
            document.getElementById("ratingsupdatename-input").value = '';
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("ratingsupdateid-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("ratingsupdateid-input").style.borderWidth = "thin";
            document.getElementById("ratingsupdatename-input").style.borderWidth = "thin";
            document.getElementById("submit-message").value = "Enter the existing rating and click 'Submit'";

        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "ratingsadd-div");
            fSetElement("Unhide", "ratingsdelete-div");
            fSetElement("Hide", "ratingsfilter-div");
            fSetElement("Hide", "ratingsadd-div");
            fSetElement("Hide", "ratingsupdate-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("ratingsdelete-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            document.getElementById("ratingsdelete-input").style.borderWidth = "thin";
            document.getElementById("ratingsdelete-input").value = '';
            document.getElementById("ratingsdelete-input").focus();
            document.getElementById("submit-message").value = "Enter the rating ID to delete and 'Submit'";
        }

    } else if (sTopic === "series") {

        fUnhideMultiple("modes-div", "submit-div", "series-div", "serieslist-textarea");
        fSetElement("Clear", "seriesfilter-input");
        fSetElement("UnhideInline", "seriesfilter-input");
        document.getElementById("seriesfilter-input").focus();
        fSetElement("Clear", "serieslist-textarea");
        fSetElement("Clear", "seriesadd-input");
        fSetElement("UnhideInline", "seriesadd-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "seriesfilter-input");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Unhide", "seriesfilter-div");
            fSetElement("Hide", "seriesadd-div");
            fSetElement("Hide", "seriesupdate-div");
            fSetElement("Hide", "seriesdelete-div");
            fSetElement("Unhide", "serieslist-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch series (or set a filter, then 'Submit')";
            sTextAreaResults = "serieslist-textarea";
            sInputFilter = "seriesfilter-input";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "seriesfilter-div");
            fSetElement("Unhide", "seriesadd-div");
            fSetElement("Hide", "seriesupdate-div");
            fSetElement("Hide", "seriesdelete-div");
            fSetElement("unHide", "serieslist-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter the new series and click 'Submit'";
            document.getElementById("seriesadd-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("seriesadd-input").focus();
            document.getElementById("seriesadd-input").style.borderWidth = "thin";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "seriesfilter-div");
            fSetElement("Hide", "seriesadd-div");
            fSetElement("Hide", "seriesdelete-div");
            fSetElement("Unhide", "seriesupdate-div");
            fSetElement("Unhide", "serieslist-div");
            fSetElement("UnhideInline", "seriesupdated-input");
            fSetElement("Disable", "seriesupdated-input");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("seriesupdateid-input").value = '';
            document.getElementById("seriesupdateid-input").focus();
            document.getElementById("seriesupdatename-input").value = '';
            document.getElementById("seriesupdateid-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("seriesupdateid-input").style.borderWidth = "thin";
            document.getElementById("seriesupdatename-input").style.borderWidth = "thin";
            document.getElementById("submit-message").value = "Enter the existing series ID and click 'Submit'";

        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "seriesfilter-div");
            fSetElement("Hide", "seriesadd-div");
            fSetElement("Hide", "seriesupdate-div");
            fSetElement("Unhide", "seriesdelete-div");
            fSetElement("Unhide", "serieslist-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("seriesdelete-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("seriesdelete-input").style.borderWidth = "thin";
            document.getElementById("seriesdelete-input").value = '';
            document.getElementById("seriesdelete-input").focus();
            document.getElementById("submit-message").value = "Enter the Series ID to delete and 'Submit'";
        }

    } else if (sTopic === "sources") {

        fUnhideMultiple("modes-div", "submit-div", "sources-div", "sourceslist-textarea");
        fSetElement("Clear", "sourcesfilter-input");
        fSetElement("UnhideInline", "sourcesfilter-input");
        document.getElementById("sourcesfilter-input").focus();
        fSetElement("Clear", "sourceslist-textarea");
        fSetElement("Unhide", "sourcesadd-div");
        fSetElement("Clear", "sourcesadd-input");
        fSetElement("UnhideInline", "sourcesadd-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "sourcesfilter-input");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Unhide", "sourcesfilter-div");
            fSetElement("Hide", "sourcesadd-div");
            fSetElement("Hide", "sourcesdelete-div");
            fSetElement("Hide", "sourcesupdate-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then click 'Submit')";
            sTextAreaResults = "sourceslist-textarea";
            sInputFilter = "sourcesfilter-input";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "sourcesfilter-input");
            fSetElement("Hide", "sourcesfilter-div");
            fSetElement("Unhide", "sourcesadd-div");
            fSetElement("Hide", "sourcesupdate-div");
            fSetElement("Hide", "sourcesdelete-div");
            fSetElement("Unhide", "sourceslist-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("sourcesadd-input").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("sourcesadd-input").style.borderWidth = "thin";
            document.getElementById("sourcesadd-input").focus();
            document.getElementById("sourcesadd-input").value = '';
            document.getElementById("submit-message").value = "Enter the new source and click 'Submit'";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "sourcesfilter-div");
            fSetElement("Hide", "sourcesadd-div");
            fSetElement("Hide", "sourcesdelete-div");
            fSetElement("Unhide", "sourceslist-div");
            fSetElement("Unhide", "sourcesupdate-div");
            fSetElement("UnhideInline", "sourcesupdated-input");
            fSetElement("Disable", "sourcesupdated-input");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("sourcesupdateid-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("sourcesupdateid-input").style.borderWidth = "thin";
            document.getElementById("sourcesupdatename-input").value = '';
            document.getElementById("sourcesupdateid-input").value = '';
            document.getElementById("sourcesupdateid-input").focus();
            document.getElementById("submit-message").value = "Enter the existing source and 'Submit'";
            
        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "sourcesfilter-div");
            fSetElement("Hide", "sourcesadd-div");
            fSetElement("Hide", "sourcesupdate-div");
            fSetElement("Unhide", "sourcesdelete-div");
            fSetElement("Unhide", "sourceslist-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("sourcesdelete-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            document.getElementById("sourcesdelete-input").style.borderWidth = "thin";
            document.getElementById("sourcesdelete-input").value = '';
            document.getElementById("sourcesdelete-input").focus();
            document.getElementById("submit-message").value = "Enter the Source ID to delete and 'Submit'";
        }

    } else if (sTopic === "genres") {

        fUnhideMultiple("modes-div", "submit-div", "genres-div", "genreslist-textarea");
        fSetElement("Clear", "genresfilter-input");
        fSetElement("UnhideInline", "genresfilter-input");
        document.getElementById("genresfilter-input").focus();
        fSetElement("Clear", "genreslist-textarea");
        fSetElement("Unhide", "genresadd-div");
        fSetElement("Clear", "genresadd-input");
        fSetElement("UnhideInline", "genresadd-input");
        fSetElement("Enable", "genresfilter-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "genresadd-div");
            fSetElement("Hide", "genresupdate-div");
            fSetElement("Hide", "genresdelete-div");
            fSetElement("Enable", "submit-button");
            fSetElement("Unhide", "genresfilter-div");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then 'Submit')";
            sTextAreaResults = "genreslist-textarea";
            sInputFilter = "genresfilter-input";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'new' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "genresfilter-input");
            fSetElement("Hide", "genresfilter-div");
            fSetElement("Unhide", "genresadd-div");
            fSetElement("Hide", "genresupdate-div");
            fSetElement("Hide", "genresdelete-div");
            fSetElement("Unhide", "genreslist-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("genresadd-input").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("genresadd-input").style.borderWidth = "thin";
            document.getElementById("genresadd-input").focus();
            document.getElementById("genresadd-input").value = '';
            document.getElementById("submit-message").value = "Enter a new genre and 'Submit'";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "genresfilter-div");
            fSetElement("Hide", "genresadd-div");
            fSetElement("Disable", "genresupdated-input");
            fSetElement("Unhide", "genresupdate-div");
            fSetElement("Hide", "genresdelete-div");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("genresupdateid-input").focus();
            document.getElementById("genresupdateid-input").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("genresupdateid-input").style.borderWidth = "thin";
            document.getElementById("genresupdateid-input").value = '';
            document.getElementById("genresupdatename-input").value = '';
            document.getElementById("genresupdated-input").value = '';
            document.getElementById("submit-message").value = "Enter the existing genre and 'Submit'";

        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "genresfilter-input");
            fSetElement("Hide", "genresadd-div");
            fSetElement("Hide", "genresupdate-div");
            fSetElement("Unhide", "genresdelete-div");
            fSetElement("Hide", "genresfilter-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("genresdelete-input").value = '';
            document.getElementById("genresdelete-input").style.borderWidth = "thin";
            document.getElementById("genresdelete-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("genresdelete-input").focus();
            document.getElementById("submit-message").value = "Enter the existing Genre ID and 'Submit'";

        }
    } else if (sTopic === "statuses") {

        fUnhideMultiple("modes-div", "submit-div", "statuses-div", "statuseslist-textarea");
        fSetElement("Clear", "statusesfilter-input");
        fSetElement("UnhideInline", "statusesfilter-input");
        document.getElementById("statusesfilter-input").focus();
        fSetElement("Clear", "statuseslist-textarea");
        fSetElement("Unhide", "statusesadd-div");
        fSetElement("Clear", "statusesadd-input");
        fSetElement("UnhideInline", "statusesadd-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Disable", "printButton");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Enable", "statusesfilter-input");
            fSetElement("Enable", "submit-button");
            fSetElement("Unhide", "statusesfilter-div");
            fSetElement("Hide", "statusesadd-div");
            fSetElement("Hide", "statusesupdate-div");
            fSetElement("Hide", "statusesdelete-div");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then 'Submit')";
            sTextAreaResults = "statuseslist-textarea";
            sInputFilter = "statusesfilter-input";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "statusesfilter-div");
            fSetElement("Unhide", "statusesadd-div");
            fSetElement("Hide", "statusesupdate-div");
            fSetElement("Hide", "statusesdelete-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter a new Status Name and 'Submit'";
            document.getElementById("statusesadd-input").style.backgroundColor = "rgb(255,255,224)";     // light yellow
            document.getElementById("statusesadd-input").style.borderWidth = "thin";
            document.getElementById("statusesadd-input").focus();
            document.getElementById("statusesadd-input").value = '';

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "statusesfilter-div");
            fSetElement("Hide", "statusesadd-div");
            fSetElement("Unhide", "statusesupdate-div");
            fSetElement("Disable", "statusesupdated-input");
            fSetElement("Hide", "statusesdelete-div");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("statusesupdateid-input").focus();
            document.getElementById("statusesupdateid-input").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("statusesupdateid-input").style.borderWidth = "thin";
            document.getElementById("statusesupdateid-input").value = '';
            document.getElementById("statusesupdatename-input").value = '';
            document.getElementById("statusesupdated-input").value = '';
            document.getElementById("submit-message").value = "Enter the existing Status ID and 'Submit'";


        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "statusesfilter-div");
            fSetElement("Hide", "statusesadd-div");
            fSetElement("Hide", "statusesupdate-div");
            fSetElement("Unhide", "statusesdelete-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("statusesdelete-input").value = '';
            document.getElementById("statusesdelete-input").focus();
            document.getElementById("statusesdelete-input").style.borderWidth = "thin";
            document.getElementById("statusesdelete-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("submit-message").value = "Enter the existing Status ID and 'Submit'";
        }
    }
}

// function to fetch results from the server when Submit button is clicked .............................................

async function fonclick_submit_submit() {

    const aTopicsToFetch = ["titles", "authors", "recents", "unreads", "classifications", "ratings", "series", "sources", "genres", "statuses"];

    let sTopic = document.getElementById("topics-select").value;
    let sInputFilter = '';
    let sTextAreaResults = '';

    // fetch list results is the topic is a list-type topic ............................................................

    if ((aTopicsToFetch.includes(sTopic)) && sMode === "fetch") {

        if (sTopic === "titles") {
            sTextAreaResults = "titlesarea-textarea";
            sInputFilter = "titlesfilter-input";
        } else if (sTopic === "recents") {
            sTextAreaResults = "recentsarea-textarea";
            sInputFilter = "recentsfilter-input";
        } else if (sTopic === "unreads") {
            sTextAreaResults = "unreadsarea-textarea";
            sInputFilter = "unreadsfilter-input";
        } else if (sTopic === "characters") {
            sTextAreaResults = "charactersarea-textarea";
            sInputFilter = "charactersfilter-input";
        } else if (sTopic === "authors") {
            sTextAreaResults = "authorslist-textarea";
            sInputFilter = "authorsfilter-input";
        } else if (sTopic === "classifications") {
            sTextAreaResults = "classificationslist-textarea";
            sInputFilter = "classificationsfilter-input";
        } else if (sTopic === "ratings") {
            sTextAreaResults = "ratingslist-textarea";
            sInputFilter = "ratingsfilter-input";
        } else if (sTopic === "series") {
            sTextAreaResults = "serieslist-textarea";
            sInputFilter = "seriesfilter-input";
        } else if (sTopic === "sources") {
            sTextAreaResults = "sourceslist-textarea";
            sInputFilter = "sourcesfilter-input";
        } else if (sTopic === "genres") {
            sTextAreaResults = "genreslist-textarea";
            sInputFilter = "genresfilter-input";
        } else if (sTopic === "statuses") {
            sTextAreaResults = "statuseslist-textarea";
            sInputFilter = "statusesfilter-input";
        }

        fFetchTopicList(sInputFilter, sTextAreaResults);

    } else if ((sTopic === 'books') && sMode === "fetch") {

        fPopulateLOV('authors');
        fPopulateLOV('sources');
        fPopulateLOV('series');
        fPopulateLOV('genres');
        fGetBookDetails();
        document.getElementById("submit-message").value=sTopic.substring(0, 4) + " details fetched";
        document.getElementById("booksstage-input").value = 'Book Fetch Submitted';
        let w = document.getElementById("wrapButton");
        w.disabled=false;
    } else if ((sTopic === 'books') && sMode === "add") {
        let sBookName = document.getElementById("booksname-input").value;
        let sBookAuthorId = document.getElementById("booksauthor-input").value;
        let sBookSourceId = document.getElementById("bookssource-input").value;
        let sBookSeriesId = document.getElementById("booksseries-input").value;
        let sBookGenreId = document.getElementById("booksgenre-input").value;
        let sBookStatusId = document.getElementById("booksstatus-input").value;
        let sBookClsfnId = document.getElementById("booksclassification-input").value;
        let sBookRatingId = document.getElementById("booksrating-input").value;
        let sBookStart = document.getElementById("booksstart-input").value;
        let sBookFinish = document.getElementById("booksfinish-input").value;
        let sBookAbstract = document.getElementById("booksabstract-textarea").value;
        let sBookCmnts = document.getElementById("bookscomments-textarea").value;
        let sRequest = uri37 + '?' + 'bookName=' + sBookName.replace(/'/g, "''") + 
                               '&' + 'authorId=' + sBookAuthorId + 
                               '&' + 'sourceId=' + sBookSourceId + 
                               '&' + 'seriesId=' + sBookSeriesId + 
                               '&' + 'genreId=' + sBookGenreId + 
                               '&' + 'statusId=' + sBookStatusId + 
                               '&' + 'clsfnId=' + sBookClsfnId + 
                               '&' + 'ratingId=' + sBookRatingId + 
                               '&' + 'startDte=' + sBookStart + 
                               '&' + 'finishDte=' + sBookFinish + 
                               '&' + 'abstract=' + sBookAbstract.replace(/'/g, "''") + 
                               '&' + 'cmnts=' + sBookCmnts.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("booksid-input").value = text;
            document.getElementById("submit-message").value = 'Book added';
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "booksid-input");
            fSetElement("Disable", "booksname-input");
            fSetElement("Disable", "booksauthor-select");
            fSetElement("Disable", "bookssource-select");
            fSetElement("Disable", "booksseries-select");
            fSetElement("Disable", "booksgenre-select");
            fSetElement("Disable", "booksstatus-select");
            fSetElement("Disable", "booksclassification-select");
            fSetElement("Disable", "booksrating-select");
            fSetElement("Disable", "booksstart-input");
            fSetElement("Disable", "booksfinish-input");
            fSetElement("Disable", "booksabstract-textarea");
            fSetElement("Disable", "booksabstract-textarea");
            fSetElement("Disable", "bookscomments-textarea");
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'books') && sMode === "update" && document.getElementById("booksstage-input").value == 'Nothing Fetched') {

        fPopulateLOV('authors');
        fPopulateLOV('sources');
        fPopulateLOV('series');
        fPopulateLOV('genres');
        fSetElement("Disable", "booksid-input");
        fGetBookDetails();
        document.getElementById("booksstage-input").value = 'Book Fetch Submitted';
        document.getElementById("submit-message").value=sTopic.substring(0, 4) + " details fetched so make updates and submit";
        fSetElement("Disable", "booksstage-input");
        fSetElement("Disable", "booksid-input");
        fSetElement("Enable", "booksname-input");
        fSetElement("Enable", "booksauthor-select");
        fSetElement("Enable", "bookssource-select");
        fSetElement("Enable", "booksseries-select");
        fSetElement("Enable", "booksgenre-select");
        fSetElement("Enable", "booksstatus-select");
        fSetElement("Enable", "booksclassification-select");
        fSetElement("Enable", "booksrating-select");
        fSetElement("Enable", "booksstart-input");
        fSetElement("Enable", "booksfinish-input");
        fSetElement("Enable", "booksabstract-textarea");
        document.getElementById("booksabstract-textarea").readOnly = false;
        document.getElementById("booksname-input").focus();
        fSetElement("Enable", "bookscomments-textarea");
        document.getElementById("bookscomments-textarea").readOnly = false;

    } else if ((sTopic === 'books') && sMode === "update" && document.getElementById("booksstage-input").value == 'Book Fetch Submitted') {

        let sBookId = document.getElementById("booksid-input").value;
        let sBookName = document.getElementById("booksname-input").value;
        let sBookAuthorId = document.getElementById("booksauthor-input").value;
        let sBookSourceId = document.getElementById("bookssource-input").value;
        let sBookSeriesId = document.getElementById("booksseries-input").value;
        let sBookGenreId = document.getElementById("booksgenre-input").value;
        let sBookStatusId = document.getElementById("booksstatus-input").value;
        let sBookClsfnId = document.getElementById("booksclassification-input").value;
        let sBookRatingId = document.getElementById("booksrating-input").value;
        let sBookStart = document.getElementById("booksstart-input").value;
        let sBookFinish = document.getElementById("booksfinish-input").value;
        let sBookAbstract = document.getElementById("booksabstract-textarea").value;
        let sBookCmnts = document.getElementById("bookscomments-textarea").value;
        let sRequest = uri39 + '?' + 'bookId=' + sBookId + 
                               '&' + 'bookName=' + sBookName.replace(/'/g, "''") + 
                               '&' + 'authorId=' + sBookAuthorId + 
                               '&' + 'sourceId=' + sBookSourceId + 
                               '&' + 'seriesId=' + sBookSeriesId + 
                               '&' + 'genreId=' + sBookGenreId + 
                               '&' + 'statusId=' + sBookStatusId + 
                               '&' + 'clsfnId=' + sBookClsfnId + 
                               '&' + 'ratingId=' + sBookRatingId + 
                               '&' + 'startDte=' + sBookStart + 
                               '&' + 'finishDte=' + sBookFinish + 
                               '&' + 'abstract=' + sBookAbstract.replace(/'/g, "''") + 
                               '&' + 'cmnts=' + sBookCmnts.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("booksid-input").value = text;
            document.getElementById("submit-message").value = 'Book added';
            fSetElement("Disable", "booksstage-input");
            fSetElement("Disable", "booksid-input");
            fSetElement("Disable", "booksname-input");
            fSetElement("Disable", "booksauthor-select");
            fSetElement("Disable", "bookssource-select");
            fSetElement("Disable", "booksseries-select");
            fSetElement("Disable", "booksgenre-select");
            fSetElement("Disable", "booksstatus-select");
            fSetElement("Disable", "booksclassification-select");
            fSetElement("Disable", "booksrating-select");
            fSetElement("Disable", "booksstart-input");
            fSetElement("Disable", "booksfinish-input");
            fSetElement("Disable", "booksabstract-textarea");
            fSetElement("Disable", "bookscomments-textarea");
            document.getElementById("submit-message").value = " Book Updated";
        }
    } else if ((sTopic === 'books') && sMode === "delete") {

            let sTitleID = document.getElementById("booksid-input").value;
            let sRequest = uri38 + '?' + "titleID=" + sTitleID;
            let response = await fetch(sRequest);
            if (response.ok) {
                let text = await response.text();
                document.getElementById("submit-message").value = text;
            } else {
                alert("HttpError: " + response.status);
            }
    } else if ((sTopic === 'characters') && sMode === "fetch") {
        let iTitleID = document.getElementById("charactersbookid-input").value;
        let sTitleName = await fGetBookTitle(iTitleID);
        document.getElementById("charactersbooktitle-input").value = sTitleName;
        if (sTitleName === "No title found") {
            document.getElementById("submit-message").value = "Invalid Book ID. No titles exist for this ID";
            document.getElementById("charactersarea-textarea").value = '';
        } else {
            fGetBookCharacters2();
        }
    } else if ((sTopic === 'characters') && sMode === "add") {
        let sTitleID=  document.getElementById("charactersbookid-input").value;
        let sCharacterName = document.getElementById("charactersadd-input").value;
        let sRequest = uri12 + '?' + "TitleID=" + sTitleID + "&CharacterName=" + sCharacterName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("charactersadd-input").value = '';
            fGetBookCharacters2();
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'characters') && sMode === "update") {
        let sCharacterID =  document.getElementById("charactersupdateid-input").value;
        let sCharacterName = document.getElementById("charactersupdatedname-input").value;
        let sRequest = uri15 + '?' + "CharacterID=" + sCharacterID + "&CharacterName=" + sCharacterName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("charactersupdatedname-input").value = '';
            document.getElementById("charactersupdatedname-input").style.backgroundColor = "rgb(255,255,255)";             // white
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'characters') && sMode === "delete") {
        let sCharID = document.getElementById("charactersdeleteid-input").value;
        let sRequest = uri13 + '?' + "CharID=" + sCharID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'authors') && sMode === "add") {
        sAddAuthorEncoded = encodeURIComponent(document.getElementById("authorsadd-input").value);
        sAddAuthorEncoded1 = sAddAuthorEncoded.replace(/'/g, "''");
        let sRequest = uri04 + '?' + "author=" + sAddAuthorEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("authorsadd-input").value = '';
            fPopulateLOV('authors');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'authors') && sMode === "update") {
        let sAuthorId =  document.getElementById("authorsupdateid-input").value;
        let sAuthorName = document.getElementById("authorsupdated-input").value;
        let sRequest = uri36 + '?' + "authorID=" + sAuthorId + "&authorName=" + sAuthorName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("authorsupdated-input").value = '';
            document.getElementById("authorsupdated-input").style.backgroundColor = "rgb(255,255,255)";           // white
            fPopulateLOV('authors');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'authors') && sMode === "delete") {
        let sAuthorID = document.getElementById("authorsdelete-input").value;
        let sRequest = uri22 + '?' + "authorID=" + sAuthorID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            fPopulateLOV('authors');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'classifications') && sMode === "add") {
        sAddClassificationsEncoded = encodeURIComponent(document.getElementById("classificationsadd-input").value);
        sAddClassificationsEncoded1 = sAddClassificationsEncoded.replace(/'/g, "''");
        let sRequest = uri05 + '?' + "classification=" + sAddClassificationsEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("classificationsadd-input").value = '';
            fPopulateLOV('classifications');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'classifications') && sMode === "update") {
        let sClassId =  document.getElementById("classificationsupdateid-input").value;
        let sClassName = document.getElementById("classificationsupdated-input").value;
        let sRequest = uri34 + '?' + "classID=" + sClassId + "&className=" + sClassName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("classificationsupdated-input").value = '';
            document.getElementById("classificationsupdated-input").style.backgroundColor = "rgb(255,255,255)";             // white
            fPopulateLOV('classifications');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'classifications') && sMode === "delete") {
        let sClassID = document.getElementById("classificationsdelete-input").value;
        let sRequest = uri16 + '?' + "ClassID=" + sClassID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            fPopulateLOV('classifications');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'ratings') && sMode === "add") {
        sAddRatingsEncoded = encodeURIComponent(document.getElementById("ratingsadd-input").value);
        sAddRatingsEncoded1 = sAddRatingsEncoded.replace(/'/g, "''");
        let sRequest = uri06 + '?' + "rating=" + sAddRatingsEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("ratingsadd-input").value = '';
            fPopulateLOV('ratings');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'ratings') && sMode === "update") {
        let sRatingId =  document.getElementById("ratingsupdateid-input").value;
        let sRatingName = document.getElementById("ratingsupdated-input").value;
        let sRequest = uri32 + '?' + "ratingID=" + sRatingId + "&ratingName=" + sRatingName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("ratingsupdated-input").value = '';
            document.getElementById("ratingsupdated-input").style.backgroundColor = "rgb(255,255,255)";           // white
            fPopulateLOV('ratings');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'ratings') && sMode === "delete") {
        let sRatingID = document.getElementById("ratingsdelete-input").value;
        let sRequest = uri17 + '?' + "ratingID=" + sRatingID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            fPopulateLOV('ratings');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'series') && sMode === "add") {
        sAddSeriesEncoded = encodeURIComponent(document.getElementById("seriesadd-input").value);
        sAddSeriesEncoded1 = sAddSeriesEncoded.replace(/'/g, "''");
        let sRequest = uri07 + '?' + "series=" + sAddSeriesEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("seriesadd-input").value = '';
            fPopulateLOV('series');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'series') && sMode === "update") {
        let sSeriesId =  document.getElementById("seriesupdateid-input").value;
        let sSeriesName = document.getElementById("seriesupdated-input").value;
        let sRequest = uri30 + '?' + "seriesID=" + sSeriesId + "&seriesName=" + sSeriesName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("seriesupdated-input").value = '';
            document.getElementById("seriesupdated-input").style.backgroundColor = "rgb(255,255,255)";            // white
            fPopulateLOV('series');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'series') && sMode === "delete") {
        let sSeriesID = document.getElementById("seriesdelete-input").value;
        let sRequest = uri18 + '?' + "seriesID=" + sSeriesID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            fPopulateLOV('series');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'sources') && sMode === "add") {
        sAddSourcesEncoded = encodeURIComponent(document.getElementById("sourcesadd-input").value);
        sAddSourcesEncoded1 = sAddSourcesEncoded.replace(/'/g, "''");
        let sRequest = uri08 + '?' + "source=" + sAddSourcesEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("sourcesadd-input").value = '';
            fPopulateLOV('sources');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'sources') && sMode === "update") {
        let sSourceId =  document.getElementById("sourcesupdateid-input").value;
        let sSourceName = document.getElementById("sourcesupdated-input").value;
        let sRequest = uri28 + '?' + "sourceID=" + sSourceId + "&sourceName=" + sSourceName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("sourcesupdated-input").value = '';
            document.getElementById("sourcesupdated-input").style.backgroundColor = "rgb(255,255,255)";           // white
            fPopulateLOV('sources');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'sources') && sMode === "delete") {
        let sSourceID = document.getElementById("sourcesdelete-input").value;
        let sRequest = uri19 + '?' + "sourceID=" + sSourceID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            fPopulateLOV('sources');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'genres') && sMode === "add") {
        sAddGenresEncoded = encodeURIComponent(document.getElementById("genresadd-input").value);
        sAddGenresEncoded1 = sAddGenresEncoded.replace(/'/g, "''");
        let sRequest = uri09 + '?' + "genre=" + sAddGenresEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("genresadd-input").value = '';
            fPopulateLOV('genres');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'genres') && sMode === "update") {
        let sGenreId =  document.getElementById("genresupdateid-input").value;
        let sGenreName = document.getElementById("genresupdated-input").value;
        let sRequest = uri26 + '?' + "genreID=" + sGenreId + "&genreName=" + sGenreName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("genresupdated-input").value = '';
            document.getElementById("genresupdated-input").style.backgroundColor = "rgb(255,255,255)";            // white
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'genres') && sMode === "delete") {
        let sGenreID = document.getElementById("genresdelete-input").value;
        let sRequest = uri20 + '?' + "genreID=" + sGenreID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            fPopulateLOV('genres');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'statuses') && sMode === "add") {
        sAddStatusesEncoded = encodeURIComponent(document.getElementById("statusesadd-input").value);
        sAddStatusesEncoded1 = sAddStatusesEncoded.replace(/'/g, "''");
        let sRequest = uri10 + '?' + "status=" + sAddStatusesEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("statusesadd-input").value = '';
            fPopulateLOV('statuses');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'statuses') && sMode === "update") {
        let sStatusId =  document.getElementById("statusesupdateid-input").value;
        let sStatusName = document.getElementById("statusesupdated-input").value;
        let sRequest = uri24 + '?' + "statusID=" + sStatusId + "&statusName=" + sStatusName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("statusesupdated-input").value = '';
            document.getElementById("statusesupdated-input").style.backgroundColor = "rgb(255,255,255)";            // white
            fPopulateLOV('statuses');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'statuses') && sMode === "delete") {
        let sStatusID = document.getElementById("statusesdelete-input").value;
        let sRequest = uri21 + '?' + "statusID=" + sStatusID;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            fPopulateLOV('statuses');
        } else {
            alert("HttpError: " + response.status);
        }
    }
}

// function to fetch book results from the server when Validate button is clicked ......................................

async function fonclick_chars_vldt_bk_id() {

    let iTitleId = document.getElementById("charactersbookid-input").value;
    let sRequest = uri11 + '?' + 'TitleID=' + iTitleId;
    document.getElementById("charactersarea-textarea").value = '';
    let response = await fetch(sRequest);
    if (response.ok) {
        let text = await response.text();
        document.getElementById("charactersbooktitle-input").value = text;
        if (text === "No title found") {
            document.getElementById("submit-message").value = "No book exists for this Title ID";
        } else if (document.getElementById("mode-label").innerHTML == "fetch mode") {
            fSetElement("Hide", "charactersadd-div");
            fSetElement("Enable", "submit-button");
            fSetElement("Enable", "charactersfilter-input");
            fSetElement("Enable", "charactersadd-input");
            fSetElement("Unhide", "charactersfilter-div");
            document.getElementById("submit-message").value = "Enter a character filter (optional) and 'submit'";
            let w = document.getElementById("wrapButton");
            w.disabled=false;
            let CharFilter = document.getElementById("charactersfilter-input");
            CharFilter.focus()
            fonclick_submit_submit();
        } else if (document.getElementById("mode-label").innerHTML == "add mode") {
            fSetElement("Unhide", "charactersadd-div");
//            fSetElement("Enable", "submit-button");
            fSetElement("Enable", "charactersfilter-input");
            fSetElement("Enable", "charactersadd-input");
            fSetElement("Hide", "charactersfilter-div");
            fGetBookCharacters2();
            document.getElementById("submit-message").value = "Enter a character filter (optional) and 'submit'";
            let AddChar = document.getElementById("charactersadd-input");
            AddChar.focus()
        }
    } else {
        alert("HttpError: " + response.status);
    }
    return;
}

// function to fetch results from the server when Validate character is clicked ........................................

async function fonclick_chars_vldt_char_id() {

    // fetch to fetch the title matching a book ID .....................................................................

    let iCharId = document.getElementById("charactersupdateid-input").value;
    let sRequest = uri14 + '?' + 'CharID=' + iCharId;
    let response = await fetch(sRequest);
    document.getElementById("charactersupdatedname-input").value = '';
    if (response.ok) {
        let text = await response.text();
        document.getElementById("charactersupdatename-input").value = text;
        if (text === "No character found") {
            document.getElementById("submit-message").value = "No character exists for this Character ID";
        } else {
            document.getElementById("charactersupdatedname-input").value = text;
            fSetElement("Enable", "charactersupdatedname-input");
            let UpdtdChar = document.getElementById("charactersupdatedname-input");
            UpdtdChar.focus();
            fSetElement("Hide", "charactersfilter-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("charactersupdatedname-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
            document.getElementById("charactersupdatedname-input").style.borderWidth = "thin";
            document.getElementById("submit-message").value = "Enter the changed character name and 'submit'";
        }
    } else {
        alert("HttpError: " + response.status);
    }
    return;
}

// function to fetch a book detail record ..............................................................................

async function fGetBookDetails() {

    let arrayRows = [];
    let arrayFields = [];
    let newtext = '';
    fClearBookDivElements();
    let sRequest = uri02 + '?' + 'TitleID=' + document.getElementById("booksid-input").value;
    let response = await fetch(sRequest);
    if (response.ok) {
        let text = await response.text();
        arrayRows = text.split('\n');
        arrayFields = arrayRows[1].split('&');
        if (arrayFields[1] != undefined) {
            document.getElementById("booksname-input").value = arrayFields[1];
            document.getElementById("booksauthor-input").value = arrayFields[2];
            document.getElementById("booksauthor-select").value = arrayFields[3];
            document.getElementById("bookssource-input").value = arrayFields[4];
            document.getElementById("bookssource-select").value = arrayFields[5];
            document.getElementById("booksseries-input").value = arrayFields[6];
            document.getElementById("booksseries-select").value = arrayFields[7];
            document.getElementById("booksstatus-input").value = arrayFields[8];
            document.getElementById("booksstatus-select").value = arrayFields[9];
            document.getElementById("booksgenre-input").value = arrayFields[10];
            document.getElementById("booksgenre-select").value = arrayFields[11];
            document.getElementById("booksclassification-input").value = arrayFields[12];
            document.getElementById("booksclassification-select").value = arrayFields[13];
            document.getElementById("booksrating-input").value = arrayFields[14];
            document.getElementById("booksrating-select").value = arrayFields[15];
            if (arrayFields[16] != "(null)") {                                           // set the start date empty if (null)
                document.getElementById("booksstart-input").value = arrayFields[16];
            } else {
                document.getElementById("booksstart-input").value = "";
            }
            if (arrayFields[17] != "(null)") {                                           // set the finish date empty if (null)
                document.getElementById("booksfinish-input").value = arrayFields[17];
            } else {
                document.getElementById("booksfinish-input").value = "";
            }
            if (arrayFields[18] != "(null)") {                                           // set comments empty if (nullL
                document.getElementById("bookscomments-textarea").value = arrayFields[18];
            } else {
                document.getElementById("bookscomments-textarea").value = "";
            }
            if (arrayFields[19] != "(null)") {                                           // set comments empty if (nullL
                document.getElementById("booksabstract-textarea").value = arrayFields[19];
            } else {
                document.getElementById("booksabstract-textarea").value = "";
            }
        } else {
            document.getElementById("submit-message").value = "No matching book found for this title ID";
        }
    } else {
        alert("HttpError: " + response.status);
    }
    fGetBookCharacters();
}

// function to fetch book characters  ..................................................................................

async function fGetBookCharacters() {

    let arrayCharacters = [];
    let sNewText = '';
    let sRequest = uri03 + '?' + 'TitleID=' + document.getElementById("booksid-input").value + "&Filter=";
    let response = await fetch(sRequest);
    if (response.ok) {
        let text = await response.text();
        arrayCharacters = text.split('\n');
        for (let i = 1; i < arrayCharacters.length; i++) {
            if (i + 1 - arrayCharacters.length == 0) {
                sNewText = sNewText + arrayCharacters[i];
            } else {
                sNewText = sNewText + arrayCharacters[i] + '\n';
            }
        }

         document.getElementById("bookscharacters-textarea").value = sNewText;

    } else {
        alert("HttpError: " + response.status);
    }
}

// function to fetch book characters and display in characersArea ......................................................

async function fGetBookCharacters2() {

    let arrayCharacters = [];
    let sNewText = '';
    let sRequest = uri03 + '?' + 'TitleID=' + document.getElementById("charactersbookid-input").value + '&Filter=' + (document.getElementById("charactersfilter-input").value).replace(/'/g, "''");
    let response = await fetch(sRequest);
    if (response.ok) {
        let text = await response.text();
        arrayCharacters = text.split('\n');
        for (let i = 1; i < arrayCharacters.length; i++) {
            if (i + 1 - arrayCharacters.length == 0) {
                sNewText = sNewText + arrayCharacters[i];
            } else {
                sNewText = sNewText + arrayCharacters[i] + '\n';
            }
        }

         document.getElementById("charactersarea-textarea").value = sNewText;
         document.getElementById("submit-message").value = "Characters fetched for the entered book title";

    } else {
        alert("HttpError: " + response.status);
    }
}

// function to fetch book title and display in characersArea ...........................................................

async function fGetBookTitle(iTitleID) {

    let sRequest = uri11 + '?' + 'TitleID=' + iTitleID;
    let response = await fetch(sRequest);
    if (response.ok) {
        let text = await response.text();
        return text;
    } else {
        alert("HttpError: " + response.status);
    }
}

// function to set wrap on textarea ....................................................................................

function fWrapText() {
    let r1 = document.getElementById("titlesarea-textarea");
    let r2 = document.getElementById("recentsarea-textarea");
    let r3 = document.getElementById("charactersarea-textarea");
    let r4 = document.getElementById("authorslist-textarea");
    let r5 = document.getElementById("classificationslist-textarea");
    let r6 = document.getElementById("ratingslist-textarea");
    let r7 = document.getElementById("serieslist-textarea");
    let r8 = document.getElementById("sourceslist-textarea");
    let r9 = document.getElementById("genreslist-textarea");
    let ra = document.getElementById("statuseslist-textarea");
    let rb = document.getElementById("unreadsarea-textarea");
    let rc = document.getElementById("bookscharacters-textarea");
    let w = document.getElementById("wrapButton");
    if((r1.style.whiteSpace == "pre") && (sTopic == "titles")) {
        r1.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r1.style.whitespace != "pre") && (sTopic == "titles")) {
        r1.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r2.style.whiteSpace == "pre") && (sTopic == "recents")) {
        r2.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r2.style.whitespace != "pre") && (sTopic == "recents")) {
        r2.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r3.style.whiteSpace == "pre") && (sTopic == "characters")) {
        r3.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r3.style.whiteSpace != "pre") && (sTopic == "characters")) {
        r3.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r4.style.whiteSpace == "pre") && (sTopic == "authors")) {
        r4.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r4.style.whiteSpace != "pre") && (sTopic == "authors")) {
        r4.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r5.style.whiteSpace == "pre") && (sTopic == "classifications")) {
        r5.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r5.style.whiteSpace != "pre") && (sTopic == "classifications")) {
        r5.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r6.style.whiteSpace == "pre") && (sTopic == "ratings")) {
        r6.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r6.style.whiteSpace != "pre") && (sTopic == "ratings")) {
        r6.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r7.style.whiteSpace == "pre") && (sTopic == "series")) {
        r7.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r7.style.whiteSpace != "pre") && (sTopic == "series")) {
        r7.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r8.style.whiteSpace == "pre") && (sTopic == "sources")) {
        r8.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r8.style.whiteSpace != "pre") && (sTopic == "sources")) {
        r8.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((r9.style.whiteSpace == "pre") && (sTopic == "genres")) {
        r9.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((r9.style.whiteSpace != "pre") && (sTopic == "genres")) {
        r9.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((ra.style.whiteSpace == "pre") && (sTopic == "statuses")) {
        ra.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((ra.style.whiteSpace != "pre") && (sTopic == "statuses")) {
        ra.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((rb.style.whiteSpace == "pre") && (sTopic == "unreads")) {
        rb.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((rb.style.whiteSpace != "pre") && (sTopic == "unreads")) {
        rb.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
    if((rc.style.whiteSpace == "pre") && (sTopic == "books")) {
        rc.style.whiteSpace = "pre-wrap";
        w.innerHTML = "Unwrap";
    } else if ((rc.style.whiteSpace != "pre") && (sTopic == "books")) {
        rc.style.whiteSpace = "pre";
        w.innerHTML = "Wrap";
    }
}

//    function to set wrap off on all textareas

function fUnwrapAllText() {
    let r1 = document.getElementById("titlesarea-textarea");
    let r2 = document.getElementById("recentsarea-textarea");
    let r3 = document.getElementById("charactersarea-textarea");
    let r4 = document.getElementById("authorslist-textarea");
    let r5 = document.getElementById("classificationslist-textarea");
    let r6 = document.getElementById("ratingslist-textarea");
    let r7 = document.getElementById("serieslist-textarea");
    let r8 = document.getElementById("sourceslist-textarea");
    let r9 = document.getElementById("genreslist-textarea");
    let ra = document.getElementById("statuseslist-textarea");
    let rb = document.getElementById("unreadsarea-textarea");
    let rc = document.getElementById("bookscharacters-textarea");
    let w = document.getElementById("wrapButton");
    r1.style.whiteSpace = "pre";
    r2.style.whiteSpace = "pre";
    r3.style.whiteSpace = "pre";
    r4.style.whiteSpace = "pre";
    r5.style.whiteSpace = "pre";
    r6.style.whiteSpace = "pre";
    r7.style.whiteSpace = "pre";
    r8.style.whiteSpace = "pre";
    r9.style.whiteSpace = "pre";
    ra.style.whiteSpace = "pre";
    rb.style.whiteSpace = "pre";
    rc.style.whiteSpace = "pre";
    w.innerHTML = "Wrap";
}

// function to validate Title ID entered before fetching a book record .................................................

function fEnableSubmitButton01(titleId) {

    TID = document.getElementById(titleId);

    let iTitleId = TID.value;

    if (iTitleId === '') {
        fSetElement("Disable", "submit-button");
        let dt1 = document.getElementById("booksid-input");
        dt1.style.backgroundColor = "rgb(255,255,224)";                                            // light yellow color
        let dt2 = document.getElementById("charactersbookid-input");
        dt2.style.backgroundColor = "rgb(255,255,224)";                                            // light yellow color
    } else if(isNaN(iTitleId) === false){
        TID.style.backgroundColor = "rgb(189,245,189)";                                              // pale green color
        fSetElement("Enable", "submit-button");
        fSetElement("Enable", "charactersvalidatebook-button");
    } else {
        TID.style.backgroundColor = "rgb(255, 204, 203)";                                             // light red color
        fSetElement("Disable", "submit-button");
        fSetElement("Disable", "charactersvalidatebook-button");
    }
}

// function to enable the validate or submit buttons if an entered book ID is a valid format ...........................

function fonKeyUp_chars_book_id() {

    if (document.getElementById("mode-label").innerHTML === "add mode") {
        iBookId = document.getElementById("charactersbookid-input").value;
        document.getElementById("charactersbooktitle-input").value = '';
        if (iBookId === '') {
            fSetElement("Disable", "charactersvalidatebook-button");
            document.getElementById("charactersbookid-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if(isNaN(iBookId) === false){
            document.getElementById("charactersbookid-input").style.backgroundColor = "rgb(189,245,189)";     // pale green color
            fSetElement("Enable", "charactersvalidatebook-button");
        } else {
            document.getElementById("charactersbookid-input").style.backgroundColor = "rgb(255, 204, 203)";    // light red color
            fSetElement("Disable", "charactersvalidatebook-button");
        }
    } else if (document.getElementById("mode-label").innerHTML === "fetch mode") {
        iBookId = document.getElementById("charactersbookid-input").value;
        document.getElementById("charactersbooktitle-input").value = '';
        fSetElement("Disable", "charactersadd-input");
        if (iBookId === '') {
            fSetElement("Disable", "charactersvalidatebook-button");
            fSetElement("Disable", "submit-button");
            document.getElementById("charactersbookid-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if(isNaN(iBookId) === false){
            document.getElementById("charactersbookid-input").style.backgroundColor = "rgb(189,245,189)";     // pale green color
            fSetElement("Enable", "charactersvalidatebook-button");
            fSetElement("Disable", "submit-button");
        } else {
            document.getElementById("charactersbookid-input").style.backgroundColor = "rgb(255, 204, 203)";    // light red color
            fSetElement("Disable", "charactersvalidatebook-button");
            fSetElement("Disable", "submit-button");
        }
    }
}

// function to enable the validate or submit buttons if an entered book ID is a valid format ...........................

function fonKeyUp_chars_add_name() {

    if (document.getElementById("mode-label").innerHTML === "add mode") {
        iBookId = document.getElementById("charactersbookid-input").value;
        fSetElement("Disable", "charactersbooktitle-input");
        let sCharToAdd = document.getElementById("charactersadd-input");
        if (sCharToAdd === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("charactersadd-input").style.backgroundColor = "rgb(255,255,224)";         //light yellow
        } else {
            fSetElement("Enable", "submit-button");
        }
    }
}

// function to enable the submit button when a classification is changed ...............................................

function fonKeyUp_class_updtd_name() {

    let sUpdtdClass = document.getElementById("classificationsupdated-input").value;
    if (sUpdtdClass != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
    ;
}

// function to enable the submit button when a changed rating is entered ...............................................

function fonKeyUp_rating_updtd_name() {
    let sUpdtdRating = document.getElementById("ratingsupdated-input").value;
    if (sUpdtdRating != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed series is entered ...............................................

function fonKeyUp_series_updtd_name() {
    let sUpdtdSeries = document.getElementById("seriesupdated-input").value;
    if (sUpdtdSeries != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed source is entered ...............................................

function fonKeyUp_sources_updtd_name() {
    let sUpdtdSource = document.getElementById("sourcesupdated-input").value;
    if (sUpdtdSource != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed genre is entered ................................................

function fonKeyUp_genres_updtd_name() {
    let sUpdtChar = document.getElementById("genresupdated-input").value;
    if (sUpdtChar != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed status is entered ...............................................

function fonKeyUp_statuses_updtd_name() {
    let sUpdtChar = document.getElementById("statusesupdated-input").value;
    if (sUpdtChar != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button if an entered character ID is a valid format ...................................

function fonKeyUp_chars_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iCharId = document.getElementById("charactersdeleteid-input").value;
        if (iCharId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("charactersdeleteid-input").style.backgroundColor = "rgb(255,255,224)";          // light yellow
        } else if (isNaN(iCharId) === true) {
            document.getElementById("charactersdeleteid-input").style.backgroundColor = "rgb(255, 204, 203)";           // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("charactersdeleteid-input").style.backgroundColor = "rgb(189,245,189)";            // pale green
        }
    }
}

// function to enable the submit button when a changed author is entered ...............................................

function fonKeyUp_author_updtd_name() {

    let sUpdtdClass = document.getElementById("authorsupdated-input").value;
    if (sUpdtdClass != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button if an entered classification ID is a valid format ..............................

function fonKeyUp_author_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iClassId = document.getElementById("authorsdelete-input").value;
        if (iClassId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("authorsdelete-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
        } else if (isNaN(iClassId) === true) {
            document.getElementById("authorsdelete-input").style.backgroundColor = "rgb(255, 204, 203)";         // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("authorsdelete-input").style.backgroundColor = "rgb(189,245,189)";          // pale green
        }
    }
}

// function to enable the submit button if an entered classification ID is a valid format ..............................

function fonKeyUp_class_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iClassId = document.getElementById("classificationsdelete-input").value;
        if (iClassId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("classificationsdelete-input").style.backgroundColor = "rgb(255,255,224)";          // light yellow
        } else if (isNaN(iClassId) === true) {
            document.getElementById("classificationsdelete-input").style.backgroundColor = "rgb(255, 204, 203)";           // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("classificationsdelete-input").style.backgroundColor = "rgb(189,245,189)";            // pale green
        }
    }
}

// function to enable the submit button if an entered rating ID (for deletion) is a valid format .......................

function fonKeyUp_ratings_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iRatingId = document.getElementById("ratingsdelete-input").value;
        if (iRatingId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("ratingsdelete-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
        } else if (isNaN(iRatingId) === true) {
            document.getElementById("ratingsdelete-input").style.backgroundColor = "rgb(255, 204, 203)";         // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("ratingsdelete-input").style.backgroundColor = "rgb(189,245,189)";          // pale green
        }
    }
}

// function to enable the submit button if an entered series ID is a valid format ......................................

function fonKeyUp_series_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iSeriesId = document.getElementById("seriesdelete-input").value;
        if (iSeriesId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("seriesdelete-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if (isNaN(iSeriesId) === true) {
            document.getElementById("seriesdelete-input").style.backgroundColor = "rgb(255, 204, 203)";          // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("seriesdelete-input").style.backgroundColor = "rgb(189,245,189)";           // pale green
        }
    }
}

// function to enable the submit button if an entered source ID is a valid format ......................................

function fonKeyUp_sources_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iSourceId = document.getElementById("sourcesdelete-input").value;
        if (iSourceId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("sourcesdelete-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
        } else if (isNaN(iSourceId) === true) {
            document.getElementById("sourcesdelete-input").style.backgroundColor = "rgb(255, 204, 203)";         // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("sourcesdelete-input").style.backgroundColor = "rgb(189,245,189)";          // pale green
        }
    }
}

// function to enable the submit button if an entered genre ID is a valid format .......................................

function fonKeyUp_genres_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iGenreId = document.getElementById("genresdelete-input").value;
        if (iGenreId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("genresdelete-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if (isNaN(iGenreId) === true) {
            document.getElementById("genresdelete-input").style.backgroundColor = "rgb(255, 204, 203)";          // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("genresdelete-input").style.backgroundColor = "rgb(189,245,189)";           // pale green
        }
    }
}

// function to enable the submit button if an entered status ID is a valid format ......................................

function fonKeyUp_statuses_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iStatusId = document.getElementById("statusesdelete-input").value;
        if (iStatusId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("statusesdelete-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if (isNaN(iStatusId) === true) {
            document.getElementById("statusesdelete-input").style.backgroundColor = "rgb(255, 204, 203)";          // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("statusesdelete-input").style.backgroundColor = "rgb(189,245,189)";           // pale green
        }
    }
}

// function to enable all the mode buttons .............................................................................

function fEnableAllModeButtons() {

    let fb = document.getElementById("modesfetch-button");
    fb.style.backgroundColor = "rgb(239,239,239)";                                                  // white smoke color
    fb.style.borderWidth = "thin";
    fb.disabled = false;
    let qb = document.getElementById("modesquery-button");
    qb.style.backgroundColor = "rgb(239,239,239)";                                                  // white smoke color
    qb.style.borderWidth = "thin";
    qb.disabled = false;
    let nb = document.getElementById("modesadd-button");
    nb.style.backgroundColor = "rgb(239,239,239)";                                                  // white smoke color
    nb.style.borderWidth = "thin";
    nb.disabled = false;
    let ub = document.getElementById("modesupdate-button");
    ub.style.backgroundColor = "rgb(239,239,239)";                                                  // white smoke color
    ub.style.borderWidth = "thin";
    ub.disabled = false;
    let db = document.getElementById("modesdelete-button");
    db.style.backgroundColor = "rgb(239,239,239)";                                                  // white smoke color
    db.style.borderWidth = "thin";
    db.disabled = false;
}

// function to disable book fields ..........................................................................

function fDisableBookFields() {

    for(let i = 0; i < arguments.length; i++) {
        fSetElement("Disable", arguments[i]);
    }
}

// function to enable book fields ..........................................................................

function fEnableBookFields() {

    for(let i = 0; i < arguments.length; i++) {
        fSetElement("Enable", arguments[i]);
    }
}

// function to unhide, hide or disable a single HTML element ...........................................................

function fSetElement(sFunction, sElement) {

    if (sFunction === "Unhide") {
        document.getElementById(sElement).style.display = "block";
    } else if (sFunction === "UnhideInline") {
        document.getElementById(sElement).style.display = "inline";
    } else if (sFunction === "Hide") {
        document.getElementById(sElement).style.display = "none";
    } else if (sFunction === "Disable") {
        document.getElementById(sElement).disabled = true;
    } else if (sFunction === "Enable") {
        document.getElementById(sElement).disabled = false;
    } else if (sFunction === "Clear") {
        document.getElementById(sElement).value = '';
    }
}

// function to unhide multiple elements ................................................................................

function fUnhideMultiple() {

    for (let i = 0; i < arguments.length; i++) {
        fSetElement("Unhide", arguments[i]);
    }
}

// fClearPage() - function to clear and hide all elements on the page ..................................................

function fClearPage() {

    let arrayOfHides = ["books-div", "titles-div", "recents-div", "unreads-div", "characters-div"
                      , "authors-div", "authorsadd-div", "classificationsadd-div", "classifications-div"
                      , "ratingsadd-div", "ratings-div", "seriesadd-div", "series-div", "sourcesadd-div"
                      , "sources-div", "genresadd-div", "genres-div", "statusesadd-div", "statuses-div"
                      , "modes-div", "submit-div"];
    for(let i = 0; i < arrayOfHides.length; i++) {
        fSetElement("Hide", arrayOfHides[i]);
    }
    let arrayOfClears = ["titlesfilter-input", "titlesarea-textarea", "recentsfilter-input", "recentsarea-textarea", "charactersfilter-input"
                       , "charactersarea-textarea", "authorsfilter-input", "authorslist-textarea", "seriesadd-input", "classificationsadd-input"
                       , "classificationsfilter-input", "classificationslist-textarea", "ratingsadd-input", "ratingsfilter-div"
                       , "ratingslist-textarea", "seriesadd-input", "seriesfilter-input", "serieslist-textarea", "sourcesadd-input"
                       , "sourcesfilter-input", "sourceslist-textarea", "genresadd-input", "genresfilter-input", "genreslist-textarea"
                       , "statusesadd-input", "statusesfilter-input", "statuseslist-textarea"];
    for(let i = 0; i < arrayOfClears.length; i++) {
        fSetElement("Hide", arrayOfClears[i]);
    }
    let h = document.getElementById("HELPDIV");
    h.style.display="none"
    let w = document.getElementById("wrapButton");
    w.disabled=true;
}

// fClearBookFields() - function to clear the page and set the topic to 'Choose' to put the page in the initial state 

function fClearBookFields() {

    fClearPage();
    fcClearExtras();
    let tc = document.getElementById("topics-select");
    tc.value = "choose";
}

// fDisableModeButton() - function to disable one mode button and enable all the other mode buttons ....................

function fDisableModeButton(sButton) {

    fEnableAllModeButtons();
    fSetElement("Unhide", "modes-div");
    let b = document.getElementById(sButton);
    b.style.backgroundColor = "rgb(189,245,189)";                                                    // pale green color
    b.style.borderWidth = "thin";
    b.disabled = true;
}

// fEnableSubmitIfNotNull() - function to enable the submit button if a field is not empty .............................

function fEnableSubmitIfNotNull(sElementId) {

    let elementId = document.getElementById(sElementId);
    if (elementId.value != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// fClearBookElements() - function to clear all elements in books:div ..................................................

function fClearBookDivElements() {

    let arrayOfClears = ["booksname-input", "booksauthor-input", "booksauthor-select", "bookssource-input", "bookssource-select"
                       , "booksseries-input", "booksseries-select" , "booksgenre-input", "booksgenre-select", "booksstatus-input", "booksstatus-select"
                       , "booksclassification-input", "booksclassification-select", "booksrating-input", "booksrating-select", "booksstart-input" , "booksfinish-input"
                       , "booksabstract-textarea", "bookscomments-textarea", "bookscharacters-textarea"];
    for(let i = 0; i < arrayOfClears.length; i++) {
        fSetElement("Clear", arrayOfClears[i]);
    }
}

// function to enable the validate button if an entered ID is a valid format for validating an update .................

function fonKeyUp_any_updt_id() {

    if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "characters")) {
        iBookId = document.getElementById("charactersbookid-input").value;
        let iCharToAdd = document.getElementById("charactersupdateid-input").value;
        if (iCharToAdd === '') {
            document.getElementById("charactersupdateid-input").style.backgroundColor = "rgb(255,255,224)";          //light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "charactersupdatename-input");
            fSetElement("Disable", "charactersupdatedname-input");
            fSetElement("Disable", "charactersvalidateid-button");
        } else if (isNaN(iCharToAdd) === true) {
            document.getElementById("charactersupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";          // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "charactersupdatename-input");
            fSetElement("Disable", "charactersupdatedname-input");
            fSetElement("Disable", "charactersvalidateid-button");
        } else {
            document.getElementById("charactersupdateid-input").style.backgroundColor = "rgb(189,245,189)";           // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "charactersupdatename-input");
            fSetElement("Disable", "charactersupdatedname-input");
            fSetElement("Enable", "charactersvalidateid-button");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "authors")) {
        iAuthorId = document.getElementById("authorsupdateid-input").value;
        if (iAuthorId === '') {
            document.getElementById("authorsupdateid-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "authorsupdatename-input");
            fSetElement("Disable", "authorsupdated-input");
            fSetElement("Disable", "authorsupdate-button");
        } else if (isNaN(iAuthorId) === true) {
            document.getElementById("authorsupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";        // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "authorsupdatename-input");
            fSetElement("Disable", "authorsupdated-input");
            fSetElement("Disable", "authorsupdate-button");
        } else {
            document.getElementById("authorsupdateid-input").style.backgroundColor = "rgb(189,245,189)";         // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "authorsupdatename-input");
            fSetElement("Disable", "authorsupdated-input");
            fSetElement("Enable", "authorsupdate-button");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "classifications")) {
        iClassId = document.getElementById("classificationsupdateid-input").value;
        if (iClassId === '') {
            document.getElementById("classificationsupdateid-input").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "classificationsupdatename-input");
            fSetElement("Disable", "classificationsupdated-input");
            fSetElement("Disable", "classificationsupdate-button");
        } else if (isNaN(iClassId) === true) {
            document.getElementById("classificationsupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";          // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "classificationsupdatename-input");
            fSetElement("Disable", "classificationsupdated-input");
            fSetElement("Disable", "classificationsupdate-button");
        } else {
            document.getElementById("classificationsupdateid-input").style.backgroundColor = "rgb(189,245,189)";           // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "classificationsupdatename-input");
            fSetElement("Disable", "classificationsupdated-input");
            fSetElement("Enable", "classificationsupdate-button");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "ratings")) {
        iRatingId = document.getElementById("ratingsupdateid-input").value;
        if (iRatingId === '') {
            document.getElementById("ratingsupdateid-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "ratingsupdatename-input");
            fSetElement("Disable", "ratingsupdated-input");
            fSetElement("Disable", "ratingsupdate-button");
        } else if (isNaN(iRatingId) === true) {
            document.getElementById("ratingsupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";        // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "ratingsupdatename-input");
            fSetElement("Disable", "ratingsupdated-input");
            fSetElement("Disable", "ratingsupdate-button");
        } else {
            document.getElementById("ratingsupdateid-input").style.backgroundColor = "rgb(189,245,189)";         // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "ratingsupdatename-input");
            fSetElement("Disable", "ratingsupdated-input");
            fSetElement("Enable", "ratingsupdate-button");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "series")) {
        iSeriesId = document.getElementById("seriesupdateid-input").value;
        if (iSeriesId === '') {
            document.getElementById("seriesupdateid-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "seriesupdatename-input");
            fSetElement("Disable", "seriesupdated-input");
            fSetElement("Disable", "seriesupdate-button");
        } else if (isNaN(iSeriesId) === true) {
            document.getElementById("seriesupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";         // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "seriesupdatename-input");
            fSetElement("Disable", "seriesupdated-input");
            fSetElement("Disable", "seriesupdate-button");
        } else {
            document.getElementById("seriesupdateid-input").style.backgroundColor = "rgb(189,245,189)";          // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "seriesupdatename-input");
            fSetElement("Disable", "seriesupdated-input");
            fSetElement("Enable", "seriesupdate-button");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "sources")) {
        iSourceId = document.getElementById("sourcesupdateid-input").value;
        if (iSourceId === '') {
            document.getElementById("sourcesupdateid-input").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "sourcesupdatename-input");
            fSetElement("Disable", "sourcesupdated-input");
            fSetElement("Disable", "sourcesupdate-button");
        } else if (isNaN(iSourceId) === true) {
            document.getElementById("sourcesupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";        // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "sourcesupdatename-input");
            fSetElement("Disable", "sourcesupdated-input");
            fSetElement("Disable", "sourcesupdate-button");
        } else {
            document.getElementById("sourcesupdateid-input").style.backgroundColor = "rgb(189,245,189)";         // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "sourcesupdatename-input");
            fSetElement("Disable", "sourcesupdated-input");
            fSetElement("Enable", "sourcesupdate-button");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "genres")) {
        iGenreId = document.getElementById("genresupdateid-input").value;
        if (iGenreId === '') {
            document.getElementById("genresupdateid-input").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "genresupdatename-input");
            fSetElement("Disable", "genresupdated-input");
            fSetElement("Disable", "genresupdate-button");
        } else if (isNaN(iGenreId) === true) {
            document.getElementById("genresupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";         // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "genresupdatename-input");
            fSetElement("Disable", "genresupdated-input");
            fSetElement("Disable", "genresupdate-button");
        } else {
            document.getElementById("genresupdateid-input").style.backgroundColor = "rgb(189,245,189)";          // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "genresupdatename-input");
            fSetElement("Disable", "genresupdated-input");
            fSetElement("Enable", "genresupdate-button");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "statuses")) {
        iStatusId = document.getElementById("statusesupdateid-input").value;
        if (iStatusId === '') {
            document.getElementById("statusesupdateid-input").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "statusesupdatename-input");
            fSetElement("Disable", "statusesupdated-input");
            fSetElement("Disable", "statusesupdate-button");
        } else if (isNaN(iStatusId) === true) {
            document.getElementById("statusesupdateid-input").style.backgroundColor = "rgb(255, 204, 203)";       // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "statusesupdatename-input");
            fSetElement("Disable", "statusesupdated-input");
            fSetElement("Disable", "statusesupdate-button");
        } else {
            document.getElementById("statusesupdateid-input").style.backgroundColor = "rgb(189,245,189)";        // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "statusesupdatename-input");
            fSetElement("Disable", "statusesupdated-input");
            fSetElement("Enable", "statusesupdate-button");
        }
    }
}

//    fonKeyUp_any_vldt_id() = validate an id for a single topic value and return the value

async function fonclick_any_vldt_id() {

    if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "authors")) {
        let iAuthorId = document.getElementById("authorsupdateid-input").value;
        let sRequest = uri35 + '?' + 'authorID=' + iAuthorId;
        let response = await fetch(sRequest);
        document.getElementById("authorsupdated-input").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("authorsupdatename-input").value = text;
            if (text === "No author found") {
                document.getElementById("submit-message").value = "No author exists for this Author ID";
                fSetElement("Disable", "authorsupdated-input");
            } else {
                fSetElement("Enable", "authorsupdated-input");
                document.getElementById("authorsupdated-input").value = text;
                document.getElementById("authorsupdated-input").focus();
                document.getElementById("authorsupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("authorsupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed Author Name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "classifications")) {
        let iClassId = document.getElementById("classificationsupdateid-input").value;
        let sRequest = uri33 + '?' + 'classID=' + iClassId;
        let response = await fetch(sRequest);
        document.getElementById("classificationsupdated-input").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("classificationsupdatename-input").value = text;
            if (text === "No classification found") {
                document.getElementById("submit-message").value = "No classification exists for this Classification ID";
                fSetElement("Disable", "classificationsupdated-input");
            } else {
                fSetElement("Enable", "classificationsupdated-input");
                document.getElementById("classificationsupdated-input").value = text;
                document.getElementById("classificationsupdated-input").focus();
                document.getElementById("classificationsupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("classificationsupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed classification name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "ratings")) {
        let iRatingId = document.getElementById("ratingsupdateid-input").value;
        let sRequest = uri31 + '?' + 'ratingID=' + iRatingId;
        let response = await fetch(sRequest);
        document.getElementById("ratingsupdated-input").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("ratingsupdatename-input").value = text;
            if (text === "No rating found") {
                document.getElementById("submit-message").value = "No rating exists for this Rating ID";
                fSetElement("Disable", "ratingsupdated-input");
            } else {
                fSetElement("Enable", "ratingsupdated-input");
                document.getElementById("ratingsupdated-input").value = text;
                document.getElementById("ratingsupdated-input").focus();
                document.getElementById("ratingsupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("ratingsupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed rating name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "series")) {
        let iSeriesId = document.getElementById("seriesupdateid-input").value;
        let sRequest = uri29 + '?' + 'seriesID=' + iSeriesId;
        let response = await fetch(sRequest);
        document.getElementById("seriesupdated-input").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("seriesupdatename-input").value = text;
            if (text === "No series found") {
                document.getElementById("submit-message").value = "No series exists for this Series ID";
                fSetElement("Disable", "seriesupdated-input");
            } else {
                fSetElement("Enable", "seriesupdated-input");
                document.getElementById("seriesupdated-input").value = text;
                document.getElementById("seriesupdated-input").focus();
                document.getElementById("seriesupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("seriesupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed series name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "sources")) {
        let iSourceId = document.getElementById("sourcesupdateid-input").value;
        let sRequest = uri27 + '?' + 'sourceID=' + iSourceId;
        let response = await fetch(sRequest);
        document.getElementById("sourcesupdated-input").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("sourcesupdatename-input").value = text;
            fSetElement("Enable", "sourcesupdated-input");
            if (text === "No source found") {
                document.getElementById("submit-message").value = "No source exists for this Source ID";
                fSetElement("Disable", "sourcesupdated-input");
            } else {
                fSetElement("Enable", "sourcesupdated-input");
                document.getElementById("sourcesupdated-input").value = text;
                document.getElementById("sourcesupdated-input").focus();
                document.getElementById("sourcesupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("sourcesupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed source name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "genres")) {
        let iGenreId = document.getElementById("genresupdateid-input").value;
        let sRequest = uri25 + '?' + 'genreID=' + iGenreId;
        let response = await fetch(sRequest);
        document.getElementById("genresupdated-input").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("genresupdatename-input").value = text;
            fSetElement("Enable", "genresupdated-input");
            if (text === "No genre found") {
                document.getElementById("submit-message").value = "No genre exists for this Genre ID";
                fSetElement("Disable", "genresupdated-input");
            } else {
                fSetElement("Enable", "genresupdated-input");
                document.getElementById("genresupdated-input").value = text;
                document.getElementById("genresupdated-input").focus();
                document.getElementById("genresupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("genresupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed genre name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "statuses")) {
        let iStatusId = document.getElementById("statusesupdateid-input").value;
        let sRequest = uri23 + '?' + 'statusID=' + iStatusId;
        let response = await fetch(sRequest);
        document.getElementById("statusesupdated-input").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("statusesupdatename-input").value = text;
            fSetElement("Enable", "statusesupdated-input");
            if (text === "No status found") {
                document.getElementById("submit-message").value = "No character exists for this Status ID";
                fSetElement("Disable", "statusesupdated-input");
            } else {
                fSetElement("Enable", "statusesupdated-input");
                document.getElementById("statusesupdated-input").value = text;
                document.getElementById("statusesupdated-input").focus();
                document.getElementById("statusesupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("statusesupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed status name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    }
}

// function to fetch lists of values and populate the LOV's for Books

async function fPopulateLOV(strAttribute) {

    if (strAttribute == 'authors') {                                           // start procedure to populate authors

        if (arrAuthorNames.length > 0) {
            return;
        }
    
        let text = '';                                                         // fetch the list of authors as lines of text
        const uri01 = "http://gjarman2020.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'authors' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrAuthorNames = text.split("\n");                                     // split the author lines of text into a 2-dim array
        const y = document.getElementById("booksauthor-select");
        for (let i = 0; i < arrAuthorNames.length; i++) {
            arrAuthorNames[i] = arrAuthorNames[i].split(",");
        }
    
        arrAuthorNames.pop();
    
        for (let i = 0; i < arrAuthorNames.length; i++) {                      // add each author name to the list of author options in books
            var option = document.createElement("option");
            option.text = arrAuthorNames[i][1].trim();
            y.add(option);
        
        }

        return;

    } else if (strAttribute == 'sources') {                                    // start procedure to populate sources

        if (arrSourceNames.length > 0) {
            return;
        }
    
        let text = '';                                                         // fetch the list of sources as lines of text
        const uri01 = "http://gjarman2020.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'sources' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrSourceNames = text.split("\n");                                     // split the source lines of text into a 2-dim array
        const y = document.getElementById("bookssource-select");
        for (let i = 0; i < arrSourceNames.length; i++) {
            arrSourceNames[i] = arrSourceNames[i].split(",");
        }
    
        arrSourceNames.pop();
    
        for (let i = 0; i < arrSourceNames.length; i++) {                      // add each source to the list of source options in books
            var option = document.createElement("option");
            option.text = arrSourceNames[i][1].trim();
            y.add(option);
        
        }

    } else if (strAttribute == 'series') {                                     // start procedure to populate series

        if (arrSeriesNames.length > 0) {
            return;
        }
    
        let text = '';                                                         // fetch the list of series as lines of text
        const uri01 = "http://gjarman2020.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'series' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrSeriesNames = text.split("\n");                                     // split the series lines of text into a 2-dim array
        const y = document.getElementById("booksseries-select");
        for (let i = 0; i < arrSeriesNames.length; i++) {
            arrSeriesNames[i] = arrSeriesNames[i].split(",");
        }
    
        arrSeriesNames.pop();
    
        for (let i = 0; i < arrSeriesNames.length; i++) {                      // add each series to the list of series options in books
            var option = document.createElement("option");
            option.text = arrSeriesNames[i][1].trim();
            y.add(option);
        
        }

    } else if (strAttribute == 'genres') {                                     // start procedure to populate genres

        if (arrGenreNames.length > 0) {
            return;
        }
    
        let text = '';                                                         // fetch the list of genres as lines of text
        const uri01 = "http://gjarman2020.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'genres' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrGenreNames = text.split("\n");                                      // split the genre lines into a 2-dim array
        const y = document.getElementById("booksgenre-select");
        for (let i = 0; i < arrGenreNames.length; i++) {
            arrGenreNames[i] = arrGenreNames[i].split(",");
        }
    
        arrGenreNames.pop();
    
        for (let i = 0; i < arrGenreNames.length; i++) {                       // add each genre to the list of genre options in books
            var option = document.createElement("option");
            option.text = arrGenreNames[i][1].trim();
            y.add(option);
        
        }

    } else if (strAttribute == 'statuses') {                                   // start the procedure to populate statuses

        if (arrStatusNames.length > 0) {
            return;
        }
    
        let text = '';                                                         // fetch the list of statuses as lines of text
        const uri01 = "http://gjarman2020.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'statuses' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrStatusNames = text.split("\n");                                     // split the status lines into a 2-dim array
        const y = document.getElementById("booksstatus-select");
        for (let i = 0; i < arrStatusNames.length; i++) {
            arrStatusNames[i] = arrStatusNames[i].split(",");
        }
    
        arrStatusNames.pop();
    
        for (let i = 0; i < arrStatusNames.length; i++) {                      // add each status to the list of status options in books
            var option = document.createElement("option");
            option.text = arrStatusNames[i][1].trim();
            y.add(option);
        
        }

    } else if (strAttribute == 'classifications') {                            // start the procedure to populate classifications

        if (arrClassificationNames.length > 0) {
            return;
        }
    
        let text = '';                                                         // fetch the list of classifications as lines of text
        const uri01 = "http://gjarman2020.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'classifications' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrClassificationNames = text.split("\n");                             // split the classification list into a 2-dim array
        const y = document.getElementById("booksclassification-select");
        for (let i = 0; i < arrClassificationNames.length; i++) {
            arrClassificationNames[i] = arrClassificationNames[i].split(",");
        }
    
        arrClassificationNames.pop();
    
        for (let i = 0; i < arrClassificationNames.length; i++) {              // add each classification to the list of classification options in books
            var option = document.createElement("option");
            option.text = arrClassificationNames[i][1].trim();
            y.add(option);
        
        }

    } else if (strAttribute == 'ratings') {                                    // start the procedure to populate ratings

        if (arrRatingNames.length > 0) {
            return;
        }
    
        let text = '';                                                         // fetch the list of ratings as lines of text
        const uri01 = "http://gjarman2020.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'ratings' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrRatingNames = text.split("\n");                                     // split the ratings list into a 2-dim array
        const y = document.getElementById("booksrating-select");
        for (let i = 0; i < arrRatingNames.length; i++) {
            arrRatingNames[i] = arrRatingNames[i].split(",");
        }
    
        arrRatingNames.pop();
    
        for (let i = 0; i < arrRatingNames.length; i++) {                      // add each rating to the list of rating options in books
            var option = document.createElement("option");
            option.text = arrRatingNames[i][1].trim();
            y.add(option);
        
        }
    }

    return;
}

function fPopulateLOVId(strAttribute) {

    if (strAttribute == 'authors') {

        for (let i = 0; i < arrAuthorNames.length; i++) {

            if (arrAuthorNames[i][1] == document.getElementById("booksauthor-select").value) 
            {
                document.getElementById("booksauthor-input").value = arrAuthorNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'sources') {

        for (let i = 0; i < arrSourceNames.length; i++) {

            if (arrSourceNames[i][1] == document.getElementById("bookssource-select").value) 
            {
                document.getElementById("bookssource-input").value = arrSourceNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'series') {

        for (let i = 0; i < arrSeriesNames.length; i++) {

            if (arrSeriesNames[i][1] == document.getElementById("booksseries-select").value) 
            {
                document.getElementById("booksseries-input").value = arrSeriesNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'genres') {

        for (let i = 0; i < arrGenreNames.length; i++) {

            if (arrGenreNames[i][1] == document.getElementById("booksgenre-select").value) 
            {
                document.getElementById("booksgenre-input").value = arrGenreNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'statuses') {

        for (let i = 0; i < arrStatusNames.length; i++) {

            if (arrStatusNames[i][1] == document.getElementById("booksstatus-select").value) 
            {
                document.getElementById("booksstatus-input").value = arrStatusNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'classifications') {

        for (let i = 0; i < arrClassificationNames.length; i++) {

            if (arrClassificationNames[i][1] == document.getElementById("booksclassification-select").value) 
            {
                document.getElementById("booksclassification-input").value = arrClassificationNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'ratings') {

        for (let i = 0; i < arrRatingNames.length; i++) {

            if (arrRatingNames[i][1] == document.getElementById("booksrating-select").value) 
            {
                document.getElementById("booksrating-input").value = arrRatingNames[i][0].trim();
                break;
            }
        }

    }

    return;
}

async function fFetchTopicList(sInputFilter, sTextAreaResults) {
    let tempText = [];
    let row = []
    let sFilterEncoded = encodeURIComponent(document.getElementById(sInputFilter).value);
    let sFilterEncoded1 = sFilterEncoded.replace(/'/g, "''");
    let sRequest = uri01 + '?' + "topic=" + document.getElementById("topics-select").value + '&filter=' + sFilterEncoded1;
    let response = await fetch(sRequest);
    if (response.ok) {
        let text = await response.text();
        if (sTopic == "authors") {
            tempText = text.split('\n');
            text = '';
            for (let i = 0; i < tempText.length - 1; i++) {
                row = tempText[i].split(',');
                text = text + row[0] + ', ' + row[1] + ", " + row[2] + '\n';
            }
        }
        document.getElementById(sTextAreaResults).style.display="block";
        document.getElementById(sTextAreaResults).value=text;

        let w = document.getElementById("wrapButton");
        w.disabled=false;

        if (sTopic === "titles") {
            document.getElementById("submit-message").value = "Titles fetched (including author, start/end dates and comments)";
        } else if (sTopic === "recents") {
            document.getElementById("submit-message").value = "Recents fetched (in descending start-date order)";
        } else if (sTopic === "unreads") {
            document.getElementById("submit-message").value = "Unreads, authors and author ratings (max 10) fetched (in asc Title ID order)";
        } else if (sTopic === "authors") {
            document.getElementById("submit-message").value = "Authors fetched (including author rating out of 10)";
        } else if (sTopic === "classifications") {
            document.getElementById("submit-message").value = "Classifications fetched";
        } else if (sTopic === "ratings") {
            document.getElementById("submit-message").value = "Ratings fetched";
        } else if (sTopic === "series") {
            document.getElementById("submit-message").value = "Series fetched (inc author, author rating and count of titles read)";
        } else if (sTopic === "sources") {
            document.getElementById("submit-message").value = "Sources fetched";
        } else if (sTopic === "genres") {
            document.getElementById("submit-message").value = "Genres fetched";
        } else if (sTopic === "statuses") {
            document.getElementById("submit-message").value = "Statuses fetched";
        } else {
            document.getElementById("submit-message").value=sTopic + " fetched";
        }
    } else {
        alert("HttpError: " + response.status);
    }
}

function fSetFocusOnSubmit() {
    document.getElementById("submit-button").focus();
}

function fPrintBookText() {
    let restorepage = document.body.innerHTML;
    let titleid = document.getElementById("booksid-input").value;
    let nametext = document.getElementById("booksname-input").value;
    let bookauthor = document.getElementById("booksauthor-select").value;
    let seriestext = document.getElementById("booksseries-select").value;
    let starttext = document.getElementById("booksstart-input").value;
    let abstracttext = document.getElementById("booksabstract-textarea").value;
    let commentstext = document.getElementById("bookscomments-textarea").value
    let characterstext = document.getElementById("bookscharacters-textarea").value.replaceAll("\n", "<br />\r\n");
    let printcontent = 'Title ID: ' + titleid + '<br><br>'
                     + 'Title: ' + nametext + '<br><br>'
                     + 'Author: ' + bookauthor + '<br><br>'
                     + 'Series: ' + seriestext + '<br><br>'
                     + 'Started: ' + starttext + '<br><br>'
                     + 'Abstract:<br>' + abstracttext + '<br><br>'
                     + 'Comments:<br>' + commentstext + '<br><br>'
                     + 'Characters:<br>' + characterstext;
    document.body.innerHTML = printcontent;
    window.print();
    document.body.innerHTML = restorepage;
}

// function to enable the Submit button when text is entered in an input field

function fEnableSubmitOnInput() {
    bookname = document.getElementById("booksname-input").value;
    if (bookname.length > 0) {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to click the submit button when Enter is clicked in an input field

function fClickSubmitOnEnter() {
      fSetElement("Enable", "submit-button");
      if (event.key === 'Enter') {
        document.getElementById("submit-button").click();
      }
}

// function to click the validate book button when Enter is clicked on input

function fClickValidateBookOnEnter() {
      fSetElement("Enable", "charactersvalidatebook-button");
      if (event.key === 'Enter') {
        fonclick_chars_vldt_bk_id();
      }
}

// function to click the validate author button when Enter is clicked on input

function fClickValidateAuthorOnEnter() {
      fSetElement("Enable", "authorsupdate-button");
      if (event.key === 'Enter') {
        fonclick_any_vldt_id();
      }
}

// function to click the validate classification button when Enter is clicked on input

function fClickValidateClassificationOnEnter() {
      fSetElement("Enable", "classificationsupdate-button");
      if (event.key === 'Enter') {
        fonclick_any_vldt_id();
      }
}

// function to click the validate rating button when Enter is clicked on input

function fClickValidateRatingOnEnter() {
      fSetElement("Enable", "ratingsupdate-button");
      if (event.key === 'Enter') {
        fonclick_any_vldt_id();
      }
}

// function to click the validate series button when Enter is clicked on input

function fClickValidateSeriesOnEnter() {
      fSetElement("Enable", "seriesupdate-button");
      if (event.key === 'Enter') {
        fonclick_any_vldt_id();
      }
}

// function to click the validate source button when Enter is clicked on input

function fClickValidateSourceOnEnter() {
      fSetElement("Enable", "sourcesupdate-button");
      if (event.key === 'Enter') {
        fonclick_any_vldt_id();
      }
}

// function to click the validate genre button when Enter is clicked on input

function fClickValidateGenreOnEnter() {
      fSetElement("Enable", "genresupdate-button");
      if (event.key === 'Enter') {
        fonclick_any_vldt_id();
      }
}

// function to click the validate status button when Enter is clicked on input

function fClickValidateStatusOnEnter() {
      fSetElement("Enable", "statusesupdate-button");
      if (event.key === 'Enter') {
        fonclick_any_vldt_id();
      }
}

// function to click the validate character button when Enter is clicked on input

function fClickValidateCharacterOnEnter() {
      fSetElement("Enable", "charactersvalidateid-button");
      if (event.key === 'Enter') {
        fonclick_chars_vldt_char_id();
      }
}
