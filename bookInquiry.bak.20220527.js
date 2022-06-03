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
//    12-May-2022 change "none" to "" for x.style.display in fShowHelp()
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
//    fShowHelp() - show or hide (toggle) help text
//    fClearPageToChoose() - clear the page and set the topic to 'Choose' to put the page in the initial state 
//    fDisableModeButton() - disable one mode button and enable all the other mode buttons
//    fEnableSubmitIfNotNull() - enable the submit button if a field is not empty
//    fClearBookElements() - clear all elements in books:div
//    fonKeyUp_any_updt_id() - enable the validate button if an entered ID is a valid format for validating an update
//    fonKeyUp_any_vldt_id() = validate an id for a single topic value and return the value
//    fFetchTopicList() -- fetch a list from the database for a single topic

// define globals for URI's .............................................................................................

const uri01 = "http://www.risingfast.com/cgi-bin/bookInquiry.cgi";
const uri02 = "http://www.risingfast.com/cgi-bin/bookDetails2.cgi";
const uri03 = "http://www.risingfast.com/cgi-bin/bookCharacters.cgi";
const uri04 = "http://www.risingfast.com/cgi-bin/bookAddAuthor.cgi";
const uri05 = "http://www.risingfast.com/cgi-bin/bookAddClassification.cgi";
const uri06 = "http://www.risingfast.com/cgi-bin/bookAddRating.cgi";
const uri07 = "http://www.risingfast.com/cgi-bin/bookAddSeries.cgi";
const uri08 = "http://www.risingfast.com/cgi-bin/bookAddSource.cgi";
const uri09 = "http://www.risingfast.com/cgi-bin/bookAddGenre.cgi";
const uri10 = "http://www.risingfast.com/cgi-bin/bookAddStatus.cgi";
const uri11 = "http://www.risingfast.com/cgi-bin/bookTitleID.cgi";
const uri12 = "http://www.risingfast.com/cgi-bin/bookAddCharacter.cgi";
const uri13 = "http://www.risingfast.com/cgi-bin/bookDelCharacter.cgi";
const uri14 = "http://www.risingfast.com/cgi-bin/bookVldtCharId.cgi";
const uri15 = "http://www.risingfast.com/cgi-bin/bookChgCharNme.cgi";
const uri16 = "http://www.risingfast.com/cgi-bin/bookDelClassification.cgi";
const uri17 = "http://www.risingfast.com/cgi-bin/bookDelRating.cgi";
const uri18 = "http://www.risingfast.com/cgi-bin/bookDelSeries.cgi";
const uri19 = "http://www.risingfast.com/cgi-bin/bookDelSource.cgi";
const uri20 = "http://www.risingfast.com/cgi-bin/bookDelGenre.cgi";
const uri21 = "http://www.risingfast.com/cgi-bin/bookDelStatus.cgi";
const uri22 = "http://www.risingfast.com/cgi-bin/bookDelAuthor.cgi";
const uri23 = "http://www.risingfast.com/cgi-bin/bookVldtStatusId.cgi";
const uri24 = "http://www.risingfast.com/cgi-bin/bookChgStatusNme.cgi";
const uri25 = "http://www.risingfast.com/cgi-bin/bookVldtGenreId.cgi";
const uri26 = "http://www.risingfast.com/cgi-bin/bookChgGenreNme.cgi";
const uri27 = "http://www.risingfast.com/cgi-bin/bookVldtSourceId.cgi";
const uri28 = "http://www.risingfast.com/cgi-bin/bookChgSourceNme.cgi";
const uri29 = "http://www.risingfast.com/cgi-bin/bookVldtSeriesId.cgi";
const uri30 = "http://www.risingfast.com/cgi-bin/bookChgSeriesNme.cgi";
const uri31 = "http://www.risingfast.com/cgi-bin/bookVldtRatingId.cgi";
const uri32 = "http://www.risingfast.com/cgi-bin/bookChgRatingNme.cgi";
const uri33 = "http://www.risingfast.com/cgi-bin/bookVldtClassId.cgi";
const uri34 = "http://www.risingfast.com/cgi-bin/bookChgClassNme.cgi";
const uri35 = "http://www.risingfast.com/cgi-bin/bookVldtAuthorId.cgi";
const uri36 = "http://www.risingfast.com/cgi-bin/bookChgAuthorNme.cgi";
const uri37 = "http://www.risingfast.com/cgi-bin/bookAddBook.cgi";
const uri38 = "http://www.risingfast.com/cgi-bin/bookDelBook.cgi";
const uri39 = "http://www.risingfast.com/cgi-bin/bookUpdtBook.cgi";

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
        fUnhideMultiple("ratings:div", "modes-div", "submit-div");
        sTopic = "ratings";
    } else if (tc.value === "series") {
        fUnhideMultiple("series:div", "modes-div", "submit-div");
        sTopic = "series";
    } else if (tc.value === "sources") {
        fUnhideMultiple("sources:div", "modes-div", "submit-div");
        sTopic = "sources";
    } else if (tc.value === "genres") {
        fUnhideMultiple("genres:div", "modes-div", "submit-div");
        sTopic = "genres";
    } else if (tc.value === "statuses") {
        fUnhideMultiple("statuses:div", "modes-div", "submit-div");
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

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("books:titleId").style.width = "35px";

            //  disable all book fields except Book ID .................................................................

            fDisableBookFields("books:titleId", "books:name", "books:authorId", "books:author", "books:sourceId", "books:source"
                             , "books:seriesId", "books:series", "books:genreId", "books:genre", "books:statusId", "books:status"
                             , "books:clsfnId", "books:clsfn", "books:ratingId", "books:rating", "books:start", "books:finish"
                             , "books:cmnts", "books:chrs");
            fSetElement("Unhide", "books-div"); 

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter the Book ID and 'Submit'";
            document.getElementById("books:stage").value = 'Nothing Fetched';

            //  enable the ID field and color light yellow as a required field .........................................

            fSetElement("Clear", "books:titleId");

            let dt = document.getElementById("books:titleId");
            dt.disabled = false;
            dt.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color

        } else if (sMode === 'query') {

            //  disable the 'query' mode button and color it green .....................................................

            fDisableModeButton("modesquery-button");
            document.getElementById("mode-label").innerHTML = "query mode";

            //  enable all book fields for 'query' mode except for the books:TitleId and books:chrs fields ................

            fEnableBookFields("books:titleId", "books:name", "books:author", "books:source", "books:series", "books:genre", "books:status", "books:clsfn", "books:rating", "books:start", "books:finish", "books:cmnts", "books:chrs");
            fSetElement("Disable", "books:titleId");
            fSetElement("Disable", "books:chrs");
            fSetElement("Unhide", "books-div"); 
            let dt = document.getElementById("books:titleId");
            dt.style.backgroundColor = "rgba(230,239,239, 0.3)";                                          // white color

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter query values in any editable fields and 'Submit'";

        } else if (sMode === 'add') {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "add mode";

            //  enable all book fields for 'add' mode

            fEnableBookFields("books:titleId", "books:name", "books:author", "books:source", "books:series", "books:genre", "books:status", "books:clsfn", "books:rating", "books:start", "books:finish", "books:cmnts", "books:chrs");
            fSetElement("Unhide", "books-div"); 
            
            //  enable all book fields for 'query' mode except for the books:titleId and books:chrs fields ................

            fEnableBookFields("books:titleId", "books:name", "books:author", "books:source", "books:series", "books:genre", "books:status", "books:clsfn", "books:rating", "books:start", "books:finish", "books:cmnts", "books:chrs");
            fSetElement("Disable", "books:titleId");
            fSetElement("Disable", "books:chrs");
            fSetElement("Unhide", "books-div"); 

            fSetElement("Clear", "books:titleId");
            
            fSetElement("Enable", "submit-button");
            document.getElementById("books:cmnts").readOnly = false;
            document.getElementById("books:cmnts").disable = false;
            let dt = document.getElementById("books:titleId");
            dt.style.backgroundColor = "rgba(239,239,239, 0.3)";                                     // light grey color

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter values for a new book and 'Submit'";

            // set the initial value of book attributes to 'Unassigned' and get the matching ID

            document.getElementById("books:author").value = 'Unassigned';
            fPopulateLOVId('authors');
            
            document.getElementById("books:source").value = 'Unassigned';
            fPopulateLOVId('sources');
            
            document.getElementById("books:series").value = 'Unassigned';
            fPopulateLOVId('series');

            document.getElementById("books:genre").value = 'Unassigned';
            fPopulateLOVId('genres');

            document.getElementById("books:status").value = 'Unassigned';
            fPopulateLOVId('statuses');

            document.getElementById("books:clsfn").value = 'Unassigned';
            fPopulateLOVId('classifications');

            document.getElementById("books:rating").value = 'Unassigned';
            fPopulateLOVId('ratings');

        } else if (sMode === 'update') {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "update mode";

            //  disable all book fields except Book ID for initial fetch ...............................................

            fDisableBookFields("books:titleId", "books:name", "books:authorId", "books:author", "books:sourceId", "books:source"
                             , "books:seriesId", "books:series", "books:genreId", "books:genre", "books:statusId", "books:status"
                             , "books:clsfnId", "books:clsfn", "books:ratingId", "books:rating", "books:start", "books:finish"
                             , "books:cmnts", "books:chrs");
            fSetElement("Unhide", "books-div"); 

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("books:stage").value = 'Nothing Fetched';
            document.getElementById("submit-message").value = "Enter the Book ID and 'Submit' to fetch book to update";

            //  enable the ID field and color light yellow as a required field .........................................

            fSetElement("Clear", "books:titleId");

            let dt = document.getElementById("books:titleId");
            dt.disabled = false;
            dt.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color

        } else if (sMode === 'delete') {

            //  disable all book fields except Book ID .................................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            document.getElementById("mode-label").innerHTML = "delete mode";

            //  disable and unhide all book fields .....................................................................

            fDisableBookFields("books:name", "books:author", "books:source", "books:series", "books:genre", "books:status", "books:clsfn", "books:rating", "books:start", "books:finish", "books:cmnts", "books:chrs");
            fSetElement("Enable", "books:titleId");
            fSetElement("Unhide", "books-div"); 
            let dt = document.getElementById("books:titleId");
            dt.style.backgroundColor = "rgb(255,255,224)";                                         // light yellow color
            
            // clear the books:titleId field ...........................................................................

            fSetElement("Clear", "books:titleId");

            // show instructions in the message area on how to proceed .................................................

            document.getElementById("submit-message").value = "Enter the Title ID and 'submit' to delete the book";
        }

    } else if (sTopic === "titles") {

        fUnhideMultiple("modes-div", "submit-div", "titles-div", "titlesarea-textarea");
        fSetElement("Clear", "titlesfilter-input");
        fSetElement("UnhideInline", "titlesfilter-input");
        fSetElement("Clear", "titlesarea-textarea");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "titlesfilter-input");

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
        fSetElement("Clear", "unreadsarea-textarea");
        fSetElement("Disable", "submit-button");

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
        }

    } else if (sTopic === "authors") {

        fUnhideMultiple("modes-div", "submit-div", "authors-div", "authorslist-textarea");
        fSetElement("Clear", "authorsfilter-input");
        fSetElement("UnhideInline", "authorsfilter-input");
        fSetElement("Clear", "authorslist-textarea");
        fSetElement("Unhide", "authorsadd-div");
        fSetElement("Clear", "series:add-name");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "authorsfilter-input");

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
        }

    } else if (sTopic === "classifications") {

        fUnhideMultiple("modes-div", "submit-div", "classifications-div", "classificationslist-textarea");
        fSetElement("Clear", "classificationsfilter-input");
        fSetElement("UnhideInline", "classificationsfilter-input");
        fSetElement("Clear", "classificationslist-textarea");
        fSetElement("Unhide", "classificationsadd-div");
        fSetElement("UnhideInline", "classificationsadd-input");
        fSetElement("Clear", "classificationsadd-input");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "classificationsfilter-input");

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
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("submit-message").value = "Enter the classification ID to delete and 'Submit'";
        }

    } else if (sTopic === "ratings") {

        fUnhideMultiple("modes-div", "submit-div", "ratings:div", "ratings:list");
        fSetElement("Clear", "ratings:filter");
        fSetElement("UnhideInline", "ratings:filter");
        fSetElement("Clear", "ratings:list");
        fSetElement("Unhide", "ratings:add-div");
        fSetElement("Clear", "ratings:add-name");
        fSetElement("UnhideInline", "ratings:add-name");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "ratings:filter");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "ratings:add-div");
            fSetElement("Hide", "ratings:updt-div");
            fSetElement("Hide", "ratings:del-div");
            fSetElement("Unhide", "ratings:filter-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch ratings (or set a filter, then click 'Submit')";
            sTextAreaResults = "ratings:list";
            sInputFilter = "ratings:filter";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Hide", "ratings:filter-div");
            fSetElement("Hide", "ratings:updt-div");
            fSetElement("Hide", "ratings:del-div");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "ratings:filter");
            document.getElementById("ratings:add-name").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("ratings:add-name").style.borderWidth = "thin";
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter the new rating and click 'Submit'";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "ratings:filter");
            fSetElement("Hide", "ratings:filter-div");
            fSetElement("Hide", "ratings:add-div");
            fSetElement("Unhide", "ratings:updt-div");
            fSetElement("Disable", "ratings:updtd-name");
            fSetElement("Hide", "ratings:del-div");
            document.getElementById("ratings:updt-id").value = '';
            document.getElementById("ratings:updt-name").value = '';
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("ratings:updt-id").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("ratings:updt-id").style.borderWidth = "thin";
            document.getElementById("ratings:updt-name").style.borderWidth = "thin";
            document.getElementById("submit-message").value = "Enter the existing rating and click 'Submit'";

        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "ratings:add-div");
            fSetElement("Unhide", "ratings:del-div");
            fSetElement("Hide", "ratings:filter-div");
            fSetElement("Hide", "ratings:add-div");
            fSetElement("Hide", "ratings:updt-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("ratings:del-id").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            document.getElementById("ratings:del-id").style.borderWidth = "thin";
            document.getElementById("ratings:del-id").value = '';
            document.getElementById("submit-message").value = "Enter the rating ID to delete and 'Submit'";
        }

    } else if (sTopic === "series") {

        fUnhideMultiple("modes-div", "submit-div", "series:div", "series:list");
        fSetElement("Clear", "series:filter");
        fSetElement("UnhideInline", "series:filter");
        fSetElement("Clear", "series:list");
        fSetElement("Clear", "series:add-name");
        fSetElement("UnhideInline", "series:add-name");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "series:filter");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Unhide", "series:filter-div");
            fSetElement("Hide", "series:add-div");
            fSetElement("Hide", "series:updt-div");
            fSetElement("Hide", "series:del-div");
            fSetElement("Unhide", "series:list-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch series (or set a filter, then 'Submit')";
            sTextAreaResults = "series:list";
            sInputFilter = "series:filter";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "series:filter-div");
            fSetElement("Unhide", "series:add-div");
            fSetElement("Hide", "series:updt-div");
            fSetElement("Hide", "series:del-div");
            fSetElement("unHide", "series:list-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter the new series and click 'Submit'";
            document.getElementById("series:add-name").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("series:add-name").style.borderWidth = "thin";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "series:filter-div");
            fSetElement("Hide", "series:add-div");
            fSetElement("Hide", "series:del-div");
            fSetElement("Unhide", "series:updt-div");
            fSetElement("Unhide", "series:list-div");
            fSetElement("UnhideInline", "series:updtd-name");
            fSetElement("Disable", "series:updtd-name");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("series:updt-id").value = '';
            document.getElementById("series:updt-name").value = '';
            document.getElementById("series:updt-id").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("series:updt-id").style.borderWidth = "thin";
            document.getElementById("series:updt-name").style.borderWidth = "thin";
            document.getElementById("submit-message").value = "Enter the existing series ID and click 'Submit'";

        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "series:filter-div");
            fSetElement("Hide", "series:add-div");
            fSetElement("Hide", "series:updt-div");
            fSetElement("Unhide", "series:del-div");
            fSetElement("Unhide", "series:list-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("series:del-id").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("series:del-id").style.borderWidth = "thin";
            document.getElementById("series:del-id").value = '';
            document.getElementById("submit-message").value = "Enter the Series ID to delete and 'Submit'";
        }

    } else if (sTopic === "sources") {

        fUnhideMultiple("modes-div", "submit-div", "sources:div", "sources:list");
        fSetElement("Clear", "sources:filter");
        fSetElement("UnhideInline", "sources:filter");
        fSetElement("Clear", "sources:list");
        fSetElement("Unhide", "sources:add-div");
        fSetElement("Clear", "sources:add-name");
        fSetElement("UnhideInline", "sources:add-name");
        fSetElement("Disable", "submit-button");
        fSetElement("Enable", "sources:filter");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Unhide", "sources:filter-div");
            fSetElement("Hide", "sources:add-div");
            fSetElement("Hide", "sources:del-div");
            fSetElement("Hide", "sources:updt-div");
            fSetElement("Enable", "submit-button");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then click 'Submit')";
            sTextAreaResults = "sources:list";
            sInputFilter = "sources:filter";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "sources:filter");
            fSetElement("Hide", "sources:filter-div");
            fSetElement("Unhide", "sources:add-div");
            fSetElement("Hide", "sources:updt-div");
            fSetElement("Hide", "sources:del-div");
            fSetElement("Unhide", "sources:list-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("sources:add-name").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("sources:add-name").style.borderWidth = "thin";
            document.getElementById("sources:add-name").value = '';
            document.getElementById("submit-message").value = "Enter the new source and click 'Submit'";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "sources:filter-div");
            fSetElement("Hide", "sources:add-div");
            fSetElement("Hide", "sources:del-div");
            fSetElement("Unhide", "sources:list-div");
            fSetElement("Unhide", "sources:updt-div");
            fSetElement("UnhideInline", "sources:updtd-name");
            fSetElement("Disable", "sources:updtd-name");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("sources:updt-id").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            document.getElementById("sources:updt-id").style.borderWidth = "thin";
            document.getElementById("sources:updt-name").value = '';
            document.getElementById("sources:updt-id").value = '';
            document.getElementById("submit-message").value = "Enter the existing source and 'Submit'";
            
        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "sources:filter-div");
            fSetElement("Hide", "sources:add-div");
            fSetElement("Hide", "sources:updt-div");
            fSetElement("Unhide", "sources:del-div");
            fSetElement("Unhide", "sources:list-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("sources:del-id").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            document.getElementById("sources:del-id").style.borderWidth = "thin";
            document.getElementById("sources:del-id").value = '';
            document.getElementById("submit-message").value = "Enter the Source ID to delete and 'Submit'";
        }

    } else if (sTopic === "genres") {

        fUnhideMultiple("modes-div", "submit-div", "genres:div", "genres:list");
        fSetElement("Clear", "genres:filter");
        fSetElement("UnhideInline", "genres:filter");
        fSetElement("Clear", "genres:list");
        fSetElement("Unhide", "genres:add-div");
        fSetElement("Clear", "genres:add-name");
        fSetElement("UnhideInline", "genres:add-name");
        fSetElement("Enable", "genres:filter");
        fSetElement("Disable", "submit-button");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "genres:add-div");
            fSetElement("Hide", "genres:updt-div");
            fSetElement("Hide", "genres:del-div");
            fSetElement("Enable", "submit-button");
            fSetElement("Unhide", "genres:filter-div");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then 'Submit')";
            sTextAreaResults = "genres:list";
            sInputFilter = "genres:filter";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'new' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "genres:filter");
            fSetElement("Hide", "genres:filter-div");
            fSetElement("Unhide", "genres:add-div");
            fSetElement("Hide", "genres:updt-div");
            fSetElement("Hide", "genres:del-div");
            fSetElement("Unhide", "genres:list-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("genres:add-name").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("genres:add-name").style.borderWidth = "thin";
            document.getElementById("genres:add-name").value = '';
            document.getElementById("submit-message").value = "Enter a new genre and 'Submit'";

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "genres:filter-div");
            fSetElement("Hide", "genres:add-div");
            fSetElement("Disable", "genres:updtd-name");
            fSetElement("Unhide", "genres:updt-div");
            fSetElement("Hide", "genres:del-div");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("genres:updt-id").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("genres:updt-id").style.borderWidth = "thin";
            document.getElementById("genres:updt-id").value = '';
            document.getElementById("genres:updt-name").value = '';
            document.getElementById("genres:updtd-name").value = '';
            document.getElementById("submit-message").value = "Enter the existing genre and 'Submit'";

        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Disable", "genres:filter");
            fSetElement("Hide", "genres:add-div");
            fSetElement("Hide", "genres:updt-div");
            fSetElement("Unhide", "genres:del-div");
            fSetElement("Hide", "genres:filter-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("genres:del-id").value = '';
            document.getElementById("genres:del-id").style.borderWidth = "thin";
            document.getElementById("genres:del-id").style.backgroundColor = "rgb(255,255,224)";         // light yellow
            document.getElementById("submit-message").value = "Enter the existing Genre ID and 'Submit'";

        }
    } else if (sTopic === "statuses") {

        fUnhideMultiple("modes-div", "submit-div", "statuses:div", "statuses:list");
        fSetElement("Clear", "statuses:filter");
        fSetElement("UnhideInline", "statuses:filter");
        fSetElement("Clear", "statuses:list");
        fSetElement("Unhide", "statuses:add-div");
        fSetElement("Clear", "statuses:add-name");
        fSetElement("UnhideInline", "statuses:add-name");
        fSetElement("Disable", "submit-button");

        if (sMode === 'fetch') {

            //  disable the 'fetch' mode button and color it green .....................................................

            fDisableModeButton("modesfetch-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Enable", "statuses:filter");
            fSetElement("Enable", "submit-button");
            fSetElement("Unhide", "statuses:filter-div");
            fSetElement("Hide", "statuses:add-div");
            fSetElement("Hide", "statuses:updt-div");
            fSetElement("Hide", "statuses:del-div");
            document.getElementById("mode-label").innerHTML = "fetch mode";
            document.getElementById("submit-message").value = "Click 'Submit' to fetch " + sTopic + " (or set a filter, then 'Submit')";
            sTextAreaResults = "statuses:list";
            sInputFilter = "statuses:filter";
            fFetchTopicList(sInputFilter, sTextAreaResults);
            fUnwrapAllText();

        } else if (sMode === "add") {

            //  disable the 'add' mode button and color it green .......................................................

            fDisableModeButton("modesadd-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "statuses:filter-div");
            fSetElement("Unhide", "statuses:add-div");
            fSetElement("Hide", "statuses:updt-div");
            fSetElement("Hide", "statuses:del-div");
            document.getElementById("mode-label").innerHTML = "add mode";
            document.getElementById("submit-message").value = "Enter a new Status Name and 'Submit'";
            document.getElementById("statuses:add-name").style.backgroundColor = "rgb(255,255,224)";     // light yellow
            document.getElementById("statuses:add-name").style.borderWidth = "thin";
            document.getElementById("statuses:add-name").value = '';

        } else if (sMode === "update") {

            //  disable the 'update' mode button and color it green ....................................................

            fDisableModeButton("modesupdate-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "statuses:filter-div");
            fSetElement("Hide", "statuses:add-div");
            fSetElement("Unhide", "statuses:updt-div");
            fSetElement("Disable", "statuses:updtd-name");
            fSetElement("Hide", "statuses:del-div");
            document.getElementById("mode-label").innerHTML = "update mode";
            document.getElementById("statuses:updt-id").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            document.getElementById("statuses:updt-id").style.borderWidth = "thin";
            document.getElementById("statuses:updt-id").value = '';
            document.getElementById("statuses:updt-name").value = '';
            document.getElementById("statuses:updtd-name").value = '';
            document.getElementById("submit-message").value = "Enter the existing Status ID and 'Submit'";


        } else if (sMode === "delete") {

            //  disable the 'delete' mode button and color it green ....................................................

            fDisableModeButton("modesdelete-button");
            fSetElement("Disable", "modesquery-button");
            fSetElement("Hide", "statuses:filter-div");
            fSetElement("Hide", "statuses:add-div");
            fSetElement("Hide", "statuses:updt-div");
            fSetElement("Unhide", "statuses:del-div");
            document.getElementById("mode-label").innerHTML = "delete mode";
            document.getElementById("statuses:del-id").value = '';
            document.getElementById("statuses:del-id").value = '';
            document.getElementById("statuses:del-id").style.borderWidth = "thin";
            document.getElementById("statuses:del-id").style.backgroundColor = "rgb(255,255,224)";       // light yellow
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
            sTextAreaResults = "ratings:list";
            sInputFilter = "ratings:filter";
        } else if (sTopic === "series") {
            sTextAreaResults = "series:list";
            sInputFilter = "series:filter";
        } else if (sTopic === "sources") {
            sTextAreaResults = "sources:list";
            sInputFilter = "sources:filter";
        } else if (sTopic === "genres") {
            sTextAreaResults = "genres:list";
            sInputFilter = "genres:filter";
        } else if (sTopic === "statuses") {
            sTextAreaResults = "statuses:list";
            sInputFilter = "statuses:filter";
        }

        fFetchTopicList(sInputFilter, sTextAreaResults);

    } else if ((sTopic === 'books') && sMode === "fetch") {

        fPopulateLOV('authors');
        fPopulateLOV('sources');
        fPopulateLOV('series');
        fPopulateLOV('genres');
        fGetBookDetails();
        document.getElementById("submit-message").value=sTopic.substring(0, 4) + " details fetched";
        document.getElementById("books:stage").value = 'Book Fetch Submitted';
        let w = document.getElementById("wrapButton");
        w.disabled=false;
    } else if ((sTopic === 'books') && sMode === "add") {
        let sBookName = document.getElementById("books:name").value;
        let sBookAuthorId = document.getElementById("books:authorId").value;
        let sBookSourceId = document.getElementById("books:sourceId").value;
        let sBookSeriesId = document.getElementById("books:seriesId").value;
        let sBookGenreId = document.getElementById("books:genreId").value;
        let sBookStatusId = document.getElementById("books:statusId").value;
        let sBookClsfnId = document.getElementById("books:clsfnId").value;
        let sBookRatingId = document.getElementById("books:ratingId").value;
        let sBookStart = document.getElementById("books:start").value;
        let sBookFinish = document.getElementById("books:finish").value;
        let sBookCmnts = document.getElementById("books:cmnts").value;
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
                               '&' + 'cmnts=' + sBookCmnts.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("books:titleId").value = text;
            document.getElementById("submit-message").value = 'Book added';
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "books:titleId");
            fSetElement("Disable", "books:name");
            fSetElement("Disable", "books:author");
            fSetElement("Disable", "books:source");
            fSetElement("Disable", "books:series");
            fSetElement("Disable", "books:genre");
            fSetElement("Disable", "books:status");
            fSetElement("Disable", "books:clsfn");
            fSetElement("Disable", "books:rating");
            fSetElement("Disable", "books:start");
            fSetElement("Disable", "books:finish");
            fSetElement("Disable", "books:cmnts");
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'books') && sMode === "update" && document.getElementById("books:stage").value == 'Nothing Fetched') {

        fPopulateLOV('authors');
        fPopulateLOV('sources');
        fPopulateLOV('series');
        fPopulateLOV('genres');
        fSetElement("Disable", "books:titleId");
        fGetBookDetails();
        document.getElementById("books:stage").value = 'Book Fetch Submitted';
        document.getElementById("submit-message").value=sTopic.substring(0, 4) + " details fetched so make updates and submit";
        fSetElement("Disable", "books:stage");
        fSetElement("Disable", "books:titleId");
        fSetElement("Enable", "books:name");
        fSetElement("Enable", "books:author");
        fSetElement("Enable", "books:source");
        fSetElement("Enable", "books:series");
        fSetElement("Enable", "books:genre");
        fSetElement("Enable", "books:status");
        fSetElement("Enable", "books:clsfn");
        fSetElement("Enable", "books:rating");
        fSetElement("Enable", "books:start");
        fSetElement("Enable", "books:finish");
        fSetElement("Enable", "books:cmnts");

    } else if ((sTopic === 'books') && sMode === "update" && document.getElementById("books:stage").value == 'Book Fetch Submitted') {

        let sBookId = document.getElementById("books:titleId").value;
        let sBookName = document.getElementById("books:name").value;
        let sBookAuthorId = document.getElementById("books:authorId").value;
        let sBookSourceId = document.getElementById("books:sourceId").value;
        let sBookSeriesId = document.getElementById("books:seriesId").value;
        let sBookGenreId = document.getElementById("books:genreId").value;
        let sBookStatusId = document.getElementById("books:statusId").value;
        let sBookClsfnId = document.getElementById("books:clsfnId").value;
        let sBookRatingId = document.getElementById("books:ratingId").value;
        let sBookStart = document.getElementById("books:start").value;
        let sBookFinish = document.getElementById("books:finish").value;
        let sBookCmnts = document.getElementById("books:cmnts").value;
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
                               '&' + 'cmnts=' + sBookCmnts.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("books:titleId").value = text;
            document.getElementById("submit-message").value = 'Book added';
            fSetElement("Disable", "books:stage");
            fSetElement("Disable", "books:titleId");
            fSetElement("Disable", "books:name");
            fSetElement("Disable", "books:author");
            fSetElement("Disable", "books:source");
            fSetElement("Disable", "books:series");
            fSetElement("Disable", "books:genre");
            fSetElement("Disable", "books:status");
            fSetElement("Disable", "books:clsfn");
            fSetElement("Disable", "books:rating");
            fSetElement("Disable", "books:start");
            fSetElement("Disable", "books:finish");
            fSetElement("Disable", "books:cmnts");
            document.getElementById("submit-message").value = " Book Updated";
        }
    } else if ((sTopic === 'books') && sMode === "delete") {

            let sTitleID = document.getElementById("books:titleId").value;
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
        sAddRatingsEncoded = encodeURIComponent(document.getElementById("ratings:add-name").value);
        sAddRatingsEncoded1 = sAddRatingsEncoded.replace(/'/g, "''");
        let sRequest = uri06 + '?' + "rating=" + sAddRatingsEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("ratings:add-name").value = '';
            fPopulateLOV('ratings');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'ratings') && sMode === "update") {
        let sRatingId =  document.getElementById("ratings:updt-id").value;
        let sRatingName = document.getElementById("ratings:updtd-name").value;
        let sRequest = uri32 + '?' + "ratingID=" + sRatingId + "&ratingName=" + sRatingName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("ratings:updtd-name").value = '';
            document.getElementById("ratings:updtd-name").style.backgroundColor = "rgb(255,255,255)";           // white
            fPopulateLOV('ratings');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'ratings') && sMode === "delete") {
        let sRatingID = document.getElementById("ratings:del-id").value;
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
        sAddSeriesEncoded = encodeURIComponent(document.getElementById("series:add-name").value);
        sAddSeriesEncoded1 = sAddSeriesEncoded.replace(/'/g, "''");
        let sRequest = uri07 + '?' + "series=" + sAddSeriesEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("series:add-name").value = '';
            fPopulateLOV('series');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'series') && sMode === "update") {
        let sSeriesId =  document.getElementById("series:updt-id").value;
        let sSeriesName = document.getElementById("series:updtd-name").value;
        let sRequest = uri30 + '?' + "seriesID=" + sSeriesId + "&seriesName=" + sSeriesName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("series:updtd-name").value = '';
            document.getElementById("series:updtd-name").style.backgroundColor = "rgb(255,255,255)";            // white
            fPopulateLOV('series');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'series') && sMode === "delete") {
        let sSeriesID = document.getElementById("series:del-id").value;
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
        sAddSourcesEncoded = encodeURIComponent(document.getElementById("sources:add-name").value);
        sAddSourcesEncoded1 = sAddSourcesEncoded.replace(/'/g, "''");
        let sRequest = uri08 + '?' + "source=" + sAddSourcesEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("sources:add-name").value = '';
            fPopulateLOV('sources');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'sources') && sMode === "update") {
        let sSourceId =  document.getElementById("sources:updt-id").value;
        let sSourceName = document.getElementById("sources:updtd-name").value;
        let sRequest = uri28 + '?' + "sourceID=" + sSourceId + "&sourceName=" + sSourceName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("sources:updtd-name").value = '';
            document.getElementById("sources:updtd-name").style.backgroundColor = "rgb(255,255,255)";           // white
            fPopulateLOV('sources');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'sources') && sMode === "delete") {
        let sSourceID = document.getElementById("sources:del-id").value;
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
        sAddGenresEncoded = encodeURIComponent(document.getElementById("genres:add-name").value);
        sAddGenresEncoded1 = sAddGenresEncoded.replace(/'/g, "''");
        let sRequest = uri09 + '?' + "genre=" + sAddGenresEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("genres:add-name").value = '';
            fPopulateLOV('genres');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'genres') && sMode === "update") {
        let sGenreId =  document.getElementById("genres:updt-id").value;
        let sGenreName = document.getElementById("genres:updtd-name").value;
        let sRequest = uri26 + '?' + "genreID=" + sGenreId + "&genreName=" + sGenreName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("genres:updtd-name").value = '';
            pdocument.getElementById("genres:updtd-name").style.backgroundColor = "rgb(255,255,255)";            // white
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'genres') && sMode === "delete") {
        let sGenreID = document.getElementById("genres:del-id").value;
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
        sAddStatusesEncoded = encodeURIComponent(document.getElementById("statuses:add-name").value);
        sAddStatusesEncoded1 = sAddStatusesEncoded.replace(/'/g, "''");
        let sRequest = uri10 + '?' + "status=" + sAddStatusesEncoded1;
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("statuses:add-name").value = '';
            fPopulateLOV('statuses');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'statuses') && sMode === "update") {
        let sStatusId =  document.getElementById("statuses:updt-id").value;
        let sStatusName = document.getElementById("statuses:updtd-name").value;
        let sRequest = uri24 + '?' + "statusID=" + sStatusId + "&statusName=" + sStatusName.replace(/'/g, "''");
        let response = await fetch(sRequest);
        if (response.ok) {
            let text = await response.text();
            document.getElementById("submit-message").value = text;
            document.getElementById("statuses:updtd-name").value = '';
            document.getElementById("statuses:updtd-name").style.backgroundColor = "rgb(255,255,255)";            // white
            fPopulateLOV('statuses');
        } else {
            alert("HttpError: " + response.status);
        }
    } else if ((sTopic === 'statuses') && sMode === "delete") {
        let sStatusID = document.getElementById("statuses:del-id").value;
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

    // validate the title matching a book ID and set a Fetch filter or Add Character field .............................

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
            fonclick_submit_submit();
        } else if (document.getElementById("mode-label").innerHTML == "add mode") {
            fSetElement("Unhide", "charactersadd-div");
            fSetElement("Enable", "submit-button");
            fSetElement("Enable", "charactersfilter-input");
            fSetElement("Enable", "charactersadd-input");
            fSetElement("Hide", "charactersfilter-div");
            document.getElementById("submit-message").value = "Enter a character filter (optional) and 'submit'";
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
            fSetElement("Enable", "charactersupdatedname-input");
            fSetElement("Disable", "charactersupdatename-input");
            fSetElement("Hide", "charactersfilter-div");
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
    let sRequest = uri02 + '?' + 'TitleID=' + document.getElementById("books:titleId").value;
    let response = await fetch(sRequest);
    if (response.ok) {
        let text = await response.text();
        arrayRows = text.split('\n');
        arrayFields = arrayRows[1].split('&');
        if (arrayFields[1] != undefined) {
            document.getElementById("books:name").value = arrayFields[1];
            document.getElementById("books:authorId").value = arrayFields[2];
            document.getElementById("books:author").value = arrayFields[3];
            document.getElementById("books:sourceId").value = arrayFields[4];
            document.getElementById("books:source").value = arrayFields[5];
            document.getElementById("books:seriesId").value = arrayFields[6];
            document.getElementById("books:series").value = arrayFields[7];
            document.getElementById("books:statusId").value = arrayFields[8];
            document.getElementById("books:status").value = arrayFields[9];
            document.getElementById("books:genreId").value = arrayFields[10];
            document.getElementById("books:genre").value = arrayFields[11];
            document.getElementById("books:clsfnId").value = arrayFields[12];
            document.getElementById("books:clsfn").value = arrayFields[13];
            document.getElementById("books:ratingId").value = arrayFields[14];
            document.getElementById("books:rating").value = arrayFields[15];
            if (arrayFields[15] != "(null)") {                                           // set the start date empty if (null)
                document.getElementById("books:start").value = arrayFields[16];
            } else {
                document.getElementById("books:start").value = "";
            }
            if (arrayFields[17] != "(null)") {                                           // set the finish date empty if (null)
                document.getElementById("books:finish").value = arrayFields[17];
            } else {
                document.getElementById("books:finish").value = "";
            }
            if (arrayFields[18] != "(null)") {                                           // set comments empty if (nullL
                document.getElementById("books:cmnts").value = arrayFields[18];
            } else {
                document.getElementById("books:cmnts").value = "";
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
    let sRequest = uri03 + '?' + 'TitleID=' + document.getElementById("books:titleId").value + "&Filter=";
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

         document.getElementById("books:chrs").value = sNewText;

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
    let r6 = document.getElementById("ratings:list");
    let r7 = document.getElementById("series:list");
    let r8 = document.getElementById("sources:list");
    let r9 = document.getElementById("genres:list");
    let ra = document.getElementById("statuses:list");
    let rb = document.getElementById("unreadsarea-textarea");
    let rc = document.getElementById("books:chrs");
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
    let r6 = document.getElementById("ratings:list");
    let r7 = document.getElementById("series:list");
    let r8 = document.getElementById("sources:list");
    let r9 = document.getElementById("genres:list");
    let ra = document.getElementById("statuses:list");
    let rb = document.getElementById("unreadsarea-textarea");
    let rc = document.getElementById("books:chrs");
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
        let dt1 = document.getElementById("books:titleId");
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

// function to enable the submit button when a changed character is entered ............................................

function fonKeyUp_chars_updtd_name() {

    let sUpdtChar = document.getElementById("charactersupdatedname-input").value;
    if (sUpdtChar != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
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
    let sUpdtdRating = document.getElementById("ratings:updtd-name").value;
    if (sUpdtdRating != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed series is entered ...............................................

function fonKeyUp_series_updtd_name() {
    let sUpdtdSeries = document.getElementById("series:updtd-name").value;
    if (sUpdtdSeries != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed source is entered ...............................................

function fonKeyUp_sources_updtd_name() {
    let sUpdtdSource = document.getElementById("sources:updtd-name").value;
    if (sUpdtdSource != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed genre is entered ................................................

function fonKeyUp_genres_updtd_name() {
    let sUpdtChar = document.getElementById("genres:updtd-name").value;
    if (sUpdtChar != '') {
        fSetElement("Enable", "submit-button");
    } else {
        fSetElement("Disable", "submit-button");
    }
}

// function to enable the submit button when a changed status is entered ...............................................

function fonKeyUp_statuses_updtd_name() {
    let sUpdtChar = document.getElementById("statuses:updtd-name").value;
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
        iRatingId = document.getElementById("ratings:del-id").value;
        if (iRatingId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("ratings:del-id").style.backgroundColor = "rgb(255,255,224)";        // light yellow
        } else if (isNaN(iRatingId) === true) {
            document.getElementById("ratings:del-id").style.backgroundColor = "rgb(255, 204, 203)";         // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("ratings:del-id").style.backgroundColor = "rgb(189,245,189)";          // pale green
        }
    }
}

// function to enable the submit button if an entered series ID is a valid format ......................................

function fonKeyUp_series_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iSeriesId = document.getElementById("series:del-id").value;
        if (iSeriesId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("series:del-id").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if (isNaN(iSeriesId) === true) {
            document.getElementById("series:del-id").style.backgroundColor = "rgb(255, 204, 203)";          // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("series:del-id").style.backgroundColor = "rgb(189,245,189)";           // pale green
        }
    }
}

// function to enable the submit button if an entered source ID is a valid format ......................................

function fonKeyUp_sources_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iSourceId = document.getElementById("sources:del-id").value;
        if (iSourceId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("sources:del-id").style.backgroundColor = "rgb(255,255,224)";        // light yellow
        } else if (isNaN(iSourceId) === true) {
            document.getElementById("sources:del-id").style.backgroundColor = "rgb(255, 204, 203)";         // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("sources:del-id").style.backgroundColor = "rgb(189,245,189)";          // pale green
        }
    }
}

// function to enable the submit button if an entered genre ID is a valid format .......................................

function fonKeyUp_genres_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iGenreId = document.getElementById("genres:del-id").value;
        if (iGenreId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("genres:del-id").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if (isNaN(iGenreId) === true) {
            document.getElementById("genres:del-id").style.backgroundColor = "rgb(255, 204, 203)";          // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("genres:del-id").style.backgroundColor = "rgb(189,245,189)";           // pale green
        }
    }
}

// function to enable the submit button if an entered status ID is a valid format ......................................

function fonKeyUp_statuses_del_id() {

    if (document.getElementById("mode-label").innerHTML === "delete mode") {
        iStatusId = document.getElementById("statuses:del-id").value;
        if (iStatusId === '') {
            fSetElement("Disable", "submit-button");
            document.getElementById("statuses:del-id").style.backgroundColor = "rgb(255,255,224)";         // light yellow
        } else if (isNaN(iStatusId) === true) {
            document.getElementById("statuses:del-id").style.backgroundColor = "rgb(255, 204, 203)";          // light red
        } else {
            fSetElement("Enable", "submit-button");
            document.getElementById("statuses:del-id").style.backgroundColor = "rgb(189,245,189)";           // pale green
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

// function to show or hide (toggle) help text .........................................................................

function fShowHelp() {

    var x = document.getElementById("HELPDIV");
    if (x.style.display === "") {
        x.style.display = "block";
    } else {
        x.style.display = "";
    }
}

// fClearPage() - function to clear and hide all elements on the page ..................................................

function fClearPage() {

    let arrayOfHides = ["books-div", "titles-div", "recents-div", "unreads-div", "characters-div"
                      , "authors-div", "authorsadd-div", "classificationsadd-div", "classifications-div"
                      , "ratings:add-div", "ratings:div", "series:add-div", "series:div", "sources:add-div"
                      , "sources:div", "genres:add-div", "genres:div", "statuses:add-div", "statuses:div"
                      , "modes-div", "submit-div"];
    for(let i = 0; i < arrayOfHides.length; i++) {
        fSetElement("Hide", arrayOfHides[i]);
    }
    let arrayOfClears = ["titlesfilter-input", "titlesarea-textarea", "recentsfilter-input", "recentsarea-textarea", "charactersfilter-input"
                       , "charactersarea-textarea", "authorsfilter-input", "authorslist-textarea", "series:add-name", "classificationsadd-input"
                       , "classificationsfilter-input", "classificationslist-textarea", "ratings:add-name", "ratings:filter"
                       , "ratings:list", "series:add-name", "series:filter", "series:list", "sources:add-name"
                       , "sources:filter", "sources:list", "genres:add-name", "genres:filter", "genres:list"
                       , "statuses:add-name", "statuses:filter", "statuses:list"];
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
    fClearExtras();
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

    let arrayOfClears = ["books:name", "books:authorId", "books:author", "books:sourceId", "books:source"
                       , "books:seriesId", "books:series" , "books:genreId", "books:genre", "books:statusId", "books:status"
                       , "books:clsfnId", "books:clsfn", "books:ratingId", "books:rating", "books:start" , "books:finish"
                       , "books:cmnts", "books:chrs"];
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
        iRatingId = document.getElementById("ratings:updt-id").value;
        if (iRatingId === '') {
            document.getElementById("ratings:updt-id").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "ratings:updt-name");
            fSetElement("Disable", "ratings:updtd-name");
            fSetElement("Disable", "ratings:vldt-id");
        } else if (isNaN(iRatingId) === true) {
            document.getElementById("ratings:updt-id").style.backgroundColor = "rgb(255, 204, 203)";        // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "ratings:updt-name");
            fSetElement("Disable", "ratings:updtd-name");
            fSetElement("Disable", "ratings:vldt-id");
        } else {
            document.getElementById("ratings:updt-id").style.backgroundColor = "rgb(189,245,189)";         // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "ratings:updt-name");
            fSetElement("Disable", "ratings:updtd-name");
            fSetElement("Enable", "ratings:vldt-id");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "series")) {
        iSeriesId = document.getElementById("series:updt-id").value;
        if (iSeriesId === '') {
            document.getElementById("series:updt-id").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "series:updt-name");
            fSetElement("Disable", "series:updtd-name");
            fSetElement("Disable", "series:vldt-id");
        } else if (isNaN(iSeriesId) === true) {
            document.getElementById("series:updt-id").style.backgroundColor = "rgb(255, 204, 203)";         // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "series:updt-name");
            fSetElement("Disable", "series:updtd-name");
            fSetElement("Disable", "series:vldt-id");
        } else {
            document.getElementById("series:updt-id").style.backgroundColor = "rgb(189,245,189)";          // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "series:updt-name");
            fSetElement("Disable", "series:updtd-name");
            fSetElement("Enable", "series:vldt-id");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "sources")) {
        iSourceId = document.getElementById("sources:updt-id").value;
        if (iSourceId === '') {
            document.getElementById("sources:updt-id").style.backgroundColor = "rgb(255,255,224)";       // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "sources:updt-name");
            fSetElement("Disable", "sources:updtd-name");
            fSetElement("Disable", "sources:vldt-id");
        } else if (isNaN(iSourceId) === true) {
            document.getElementById("sources:updt-id").style.backgroundColor = "rgb(255, 204, 203)";        // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "sources:updt-name");
            fSetElement("Disable", "sources:updtd-name");
            fSetElement("Disable", "sources:vldt-id");
        } else {
            document.getElementById("sources:updt-id").style.backgroundColor = "rgb(189,245,189)";         // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "sources:updt-name");
            fSetElement("Disable", "sources:updtd-name");
            fSetElement("Enable", "sources:vldt-id");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "genres")) {
        iGenreId = document.getElementById("genres:updt-id").value;
        if (iGenreId === '') {
            document.getElementById("genres:updt-id").style.backgroundColor = "rgb(255,255,224)";        // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "genres:updt-name");
            fSetElement("Disable", "genres:updtd-name");
            fSetElement("Disable", "genres:vldt-id");
        } else if (isNaN(iGenreId) === true) {
            document.getElementById("genres:updt-id").style.backgroundColor = "rgb(255, 204, 203)";         // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "genres:updt-name");
            fSetElement("Disable", "genres:updtd-name");
            fSetElement("Disable", "genres:vldt-id");
        } else {
            document.getElementById("genres:updt-id").style.backgroundColor = "rgb(189,245,189)";          // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "genres:updt-name");
            fSetElement("Disable", "genres:updtd-name");
            fSetElement("Enable", "genres:vldt-id");
        }
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "statuses")) {
        iStatusId = document.getElementById("statuses:updt-id").value;
        if (iStatusId === '') {
            document.getElementById("statuses:updt-id").style.backgroundColor = "rgb(255,255,224)";      // light yellow
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "statuses:updt-name");
            fSetElement("Disable", "statuses:updtd-name");
            fSetElement("Disable", "statuses:vldt-id");
        } else if (isNaN(iStatusId) === true) {
            document.getElementById("statuses:updt-id").style.backgroundColor = "rgb(255, 204, 203)";       // light red
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "statuses:updt-name");
            fSetElement("Disable", "statuses:updtd-name");
            fSetElement("Disable", "statuses:vldt-id");
        } else {
            document.getElementById("statuses:updt-id").style.backgroundColor = "rgb(189,245,189)";        // pale green
            fSetElement("Disable", "submit-button");
            fSetElement("Disable", "statuses:updt-name");
            fSetElement("Disable", "statuses:updtd-name");
            fSetElement("Enable", "statuses:vldt-id");
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
            if (text === "No authors found") {
                document.getElementById("submit-message").value = "No author exists for this Author ID";
                fSetElement("Disable", "authorsupdated-input");
            } else {
                fSetElement("Enable", "authorsupdated-input");
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
                document.getElementById("submit-message").value = "No classification exists for this classification ID";
                fSetElement("Disable", "classificationsupdated-input");
            } else {
                fSetElement("Enable", "classificationsupdated-input");
                document.getElementById("classificationsupdated-input").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("classificationsupdated-input").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed classification name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "ratings")) {
        let iRatingId = document.getElementById("ratings:updt-id").value;
        let sRequest = uri31 + '?' + 'ratingID=' + iRatingId;
        let response = await fetch(sRequest);
        document.getElementById("ratings:updtd-name").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("ratings:updt-name").value = text;
            if (text === "No rating found") {
                document.getElementById("submit-message").value = "No rating exists for this rating ID";
                fSetElement("Disable", "ratings:updtd-name");
            } else {
                fSetElement("Enable", "ratings:updtd-name");
                document.getElementById("ratings:updtd-name").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("ratings:updtd-name").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed rating name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "series")) {
        let iSeriesId = document.getElementById("series:updt-id").value;
        let sRequest = uri29 + '?' + 'seriesID=' + iSeriesId;
        let response = await fetch(sRequest);
        document.getElementById("series:updtd-name").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("series:updt-name").value = text;
            if (text === "No series found") {
                document.getElementById("submit-message").value = "No series exists for this series ID";
                fSetElement("Disable", "series:updtd-name");
            } else {
                fSetElement("Enable", "series:updtd-name");
                document.getElementById("series:updtd-name").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("series:updtd-name").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed series name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "sources")) {
        let iSourcesId = document.getElementById("sources:updt-id").value;
        let sRequest = uri27 + '?' + 'sourceID=' + iSourceId;
        let response = await fetch(sRequest);
        document.getElementById("sources:updtd-name").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("sources:updt-name").value = text;
            fSetElement("Enable", "sources:updtd-name");
            if (text === "No source found") {
                document.getElementById("submit-message").value = "No source exists for this source ID";
                fSetElement("Disable", "sources:updtd-name");
            } else {
                fSetElement("Enable", "sources:updtd-name");
                document.getElementById("sources:updtd-name").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("sources:updtd-name").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed source name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "genres")) {
        let iGenresId = document.getElementById("genres:updt-id").value;
        let sRequest = uri25 + '?' + 'genreID=' + iGenreId;
        let response = await fetch(sRequest);
        document.getElementById("genres:updtd-name").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("genres:updt-name").value = text;
            fSetElement("Enable", "genres:updtd-name");
            if (text === "No genre found") {
                document.getElementById("submit-message").value = "No genre exists for this genre ID";
                fSetElement("Disable", "genres:updtd-name");
            } else {
                fSetElement("Enable", "genres:updtd-name");
                document.getElementById("genres:updtd-name").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("genres:updtd-name").style.borderWidth = "thin";
                document.getElementById("submit-message").value = "Enter the changed genre name and 'submit'";
            }
        } else {
            alert("HttpError: " + response.status);
        }
        return;
    } else if ((document.getElementById("mode-label").innerHTML === "update mode") && (document.getElementById("topics-select").value === "statuses")) {
        let iStatusId = document.getElementById("statuses:updt-id").value;
        let sRequest = uri23 + '?' + 'statusID=' + iStatusId;
        let response = await fetch(sRequest);
        document.getElementById("statuses:updtd-name").value = '';
        if (response.ok) {
            let text = await response.text();
            document.getElementById("statuses:updt-name").value = text;
            fSetElement("Enable", "statuses:updtd-name");
            if (text === "No status found") {
                document.getElementById("submit-message").value = "No character exists for this Status ID";
                fSetElement("Disable", "statuses:updtd-name");
            } else {
                fSetElement("Enable", "statuses:updtd-name");
                document.getElementById("statuses:updtd-name").style.backgroundColor = "rgb(255,255,224)";    // light yellow
                document.getElementById("statuses:updtd-name").style.borderWidth = "thin";
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
        const uri01 = "http://www.risingfast.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'authors' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrAuthorNames = text.split("\n");                                     // split the author lines of text into a 2-dim array
        const y = document.getElementById("books:author");
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
        const uri01 = "http://www.risingfast.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'sources' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrSourceNames = text.split("\n");                                     // split the source lines of text into a 2-dim array
        const y = document.getElementById("books:source");
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
        const uri01 = "http://www.risingfast.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'series' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrSeriesNames = text.split("\n");                                     // split the series lines of text into a 2-dim array
        const y = document.getElementById("books:series");
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
        const uri01 = "http://www.risingfast.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'genres' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrGenreNames = text.split("\n");                                      // split the genre lines into a 2-dim array
        const y = document.getElementById("books:genre");
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
        const uri01 = "http://www.risingfast.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'statuses' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrStatusNames = text.split("\n");                                     // split the status lines into a 2-dim array
        const y = document.getElementById("books:status");
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
        const uri01 = "http://www.risingfast.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'classifications' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrClassificationNames = text.split("\n");                             // split the classification list into a 2-dim array
        const y = document.getElementById("books:clsfn");
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
        const uri01 = "http://www.risingfast.com/cgi-bin/bookFetchLOVs.cgi"
        const sRequest = uri01 + '?' + 'topic=' + 'ratings' + '&filter= ';
        let response = await fetch(sRequest);
        if (response.ok) {
            text = await response.text();
        } else {
              alert("HttpError: " + response.status);
        }
    
        arrRatingNames = text.split("\n");                                     // split the ratings list into a 2-dim array
        const y = document.getElementById("books:rating");
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

            if (arrAuthorNames[i][1] == document.getElementById("books:author").value) 
            {
                document.getElementById("books:authorId").value = arrAuthorNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'sources') {

        for (let i = 0; i < arrSourceNames.length; i++) {

            if (arrSourceNames[i][1] == document.getElementById("books:source").value) 
            {
                document.getElementById("books:sourceId").value = arrSourceNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'series') {

        for (let i = 0; i < arrSeriesNames.length; i++) {

            if (arrSeriesNames[i][1] == document.getElementById("books:series").value) 
            {
                document.getElementById("books:seriesId").value = arrSeriesNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'genres') {

        for (let i = 0; i < arrGenreNames.length; i++) {

            if (arrGenreNames[i][1] == document.getElementById("books:genre").value) 
            {
                document.getElementById("books:genreId").value = arrGenreNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'statuses') {

        for (let i = 0; i < arrStatusNames.length; i++) {

            if (arrStatusNames[i][1] == document.getElementById("books:status").value) 
            {
                document.getElementById("books:statusId").value = arrStatusNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'classifications') {

        for (let i = 0; i < arrClassificationNames.length; i++) {

            if (arrClassificationNames[i][1] == document.getElementById("books:clsfn").value) 
            {
                document.getElementById("books:clsfnId").value = arrClassificationNames[i][0].trim();
                break;
            }
        }

    } else if (strAttribute == 'ratings') {

        for (let i = 0; i < arrRatingNames.length; i++) {

            if (arrRatingNames[i][1] == document.getElementById("books:rating").value) 
            {
                document.getElementById("books:ratingId").value = arrRatingNames[i][0].trim();
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
            document.getElementById("submit-message").value = "Titles fetched (including start/end dates and comments)";
        } else if (sTopic === "recents") {
            document.getElementById("submit-message").value = "Recents fetched (in descending start-date order)";
        } else if (sTopic === "unreads") {
            document.getElementById("submit-message").value = "Unreads fetched (in descending Title ID order)";
        } else if (sTopic === "authors") {
            document.getElementById("submit-message").value = "Authors fetched (including author rating out of 10)";
        } else if (sTopic === "classifications") {
            document.getElementById("submit-message").value = "Classifications fetched";
        } else if (sTopic === "ratings") {
            document.getElementById("submit-message").value = "Ratings fetched";
        } else if (sTopic === "series") {
            document.getElementById("submit-message").value = "Series fetched (including Author, rating and count of titles)";
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
