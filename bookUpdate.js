// bookUpdate.js script for bookUpdate.html
// Author: Geoffrey Jarman
// Started: 09/21/2020
// Reference:
// Log:
//     09/21/2020 started
//     09/22/2020 expand disable logic to include buttons
//     09/23/2020 add a system message area and update with messages
//     09/23/2020 add end of file checks for running previous and next record actions
//     09/23/2020 add handler for single record fetch
//     09/24/2020 clear all non-inquiry fields when fetch or fetchall is chosen by calling init first
//     09/24/2020 change single record fetch for no match condition
//     09/25/2020 Add handler to save changes back to a file
//     09/25/2020 trouble-shoot new line and multi-record post for changes
//     09/26/2020 change submit button to red when active
//     09/26/2020 create functions to clean up repetitive code
//     09/16/2022 change www.risingfast.com to gjarman2020.com
// Enhancements:

"use strict";

const url1 = "http://gjarman2020.com/cgi-bin/bookUpdate.cgi";

// intialize internals division and run initial settings ------------------------------------------

var y = document.getElementById("internalsDiv");
const sty = getComputedStyle(y);
y.style.display = sty.display;

var text = "";
var arrBuffer = new Array(2);
var i = 0;

initSettings();                                          // set the initial form state to no action

// function to process an action ------------------------------------------------------------------

async function takeAction() {

    var bfMatchFound = 0;
    var strData = "";

    if ((document.getElementById("fetchall").checked === true) || (document.getElementById("fetch").checked === true)) {
        let response = await fetch(url1);
        if (response.ok) {
            let text = await response.text();        // read the file into a two dimiensional array
            arrBuffer = text.split("\n");
            for (i = 0; (i < arrBuffer.length - 1); i++) {
                arrBuffer[i] = arrBuffer[i].split("|");
            }
        }
    }

    if (document.getElementById("fetchall").checked === true) {
        i = 0;
        document.getElementById("submit").style="color:black";
        document.getElementById("messages").value="All records fetched";
        document.getElementById("title").value = arrBuffer[i][0].replace("Title=", "");
        document.getElementById("author").value = arrBuffer[i][1].replace("Author=", "");
        document.getElementById("source").value = arrBuffer[i][2].replace("Source=", "");
        document.getElementById("series").value = arrBuffer[i][3].replace("Series=", "");
        document.getElementById("start").value = arrBuffer[i][4].replace("Start=", "");
        document.getElementById("finish").value = arrBuffer[i][5].replace("Finish=", "");
        document.getElementById("char01").value = arrBuffer[i][6].replace("Char01=", "");
        document.getElementById("char02").value = arrBuffer[i][7].replace("Char02=", "");
        document.getElementById("char03").value = arrBuffer[i][8].replace("Char03=", "");
        document.getElementById("char04").value = arrBuffer[i][9].replace("Char04=", "");
        document.getElementById("char05").value = arrBuffer[i][10].replace("Char05=", "");
        document.getElementById("char06").value = arrBuffer[i][11].replace("Char06=", "");
        document.getElementById("char07").value = arrBuffer[i][12].replace("Char07=", "");
        document.getElementById("char08").value = arrBuffer[i][13].replace("Char08=", "");
        document.getElementById("char09").value = arrBuffer[i][14].replace("Char09=", "");
        document.getElementById("char10").value = arrBuffer[i][15].replace("Char10=", "");
        document.getElementById("char11").value = arrBuffer[i][16].replace("Char11=", "");
        document.getElementById("char12").value = arrBuffer[i][17].replace("Char12=", "");
        document.getElementById("char13").value = arrBuffer[i][18].replace("Char13=", "");
        document.getElementById("char14").value = arrBuffer[i][19].replace("Char14=", "");
        document.getElementById("char15").value = arrBuffer[i][20].replace("Char15=", "");
        document.getElementById("char16").value = arrBuffer[i][21].replace("Char16=", "");
        document.getElementById("char17").value = arrBuffer[i][22].replace("Char17=", "");
        document.getElementById("char18").value = arrBuffer[i][23].replace("Char18=", "");
        document.getElementById("char19").value = arrBuffer[i][24].replace("Char19=", "");
        document.getElementById("char20").value = arrBuffer[i][25].replace("Char20=", "");
        document.getElementById("char21").value = arrBuffer[i][26].replace("Char21=", "");
        document.getElementById("char22").value = arrBuffer[i][27].replace("Char22=", "");
        document.getElementById("char23").value = arrBuffer[i][28].replace("Char23=", "");
        document.getElementById("char24").value = arrBuffer[i][29].replace("Char24=", "");
        document.getElementById("next").disabled=false;
        document.getElementById("previous").disabled=false;
    }
    else if (document.getElementById("fetch").checked === true) {
        i = 0;
        bfMatchFound = 1;
        while (arrBuffer[i][0].replace("Title=", "").localeCompare(document.getElementById("title").value) != 0) {
            if (i === (arrBuffer.length - 2)) {
                bfMatchFound = 0;
                break;
            }
            i++;
        }
        if (bfMatchFound) {
            document.getElementById("messages").value="Matching record found";
            document.getElementById("title").value = arrBuffer[i][0].replace("Title=", "");
            document.getElementById("author").value = arrBuffer[i][1].replace("Author=", "");
            document.getElementById("source").value = arrBuffer[i][2].replace("Source=", "");
            document.getElementById("series").value = arrBuffer[i][3].replace("Series=", "");
            document.getElementById("start").value = arrBuffer[i][4].replace("Start=", "");
            document.getElementById("finish").value = arrBuffer[i][5].replace("Finish=", "");
            document.getElementById("char01").value = arrBuffer[i][6].replace("Char01=", "");
            document.getElementById("char02").value = arrBuffer[i][7].replace("Char02=", "");
            document.getElementById("char03").value = arrBuffer[i][8].replace("Char03=", "");
            document.getElementById("char04").value = arrBuffer[i][9].replace("Char04=", "");
            document.getElementById("char05").value = arrBuffer[i][10].replace("Char05=", "");
            document.getElementById("char06").value = arrBuffer[i][11].replace("Char06=", "");
            document.getElementById("char07").value = arrBuffer[i][12].replace("Char07=", "");
            document.getElementById("char08").value = arrBuffer[i][13].replace("Char08=", "");
            document.getElementById("char09").value = arrBuffer[i][14].replace("Char09=", "");
            document.getElementById("char10").value = arrBuffer[i][15].replace("Char10=", "");
            document.getElementById("char11").value = arrBuffer[i][16].replace("Char11=", "");
            document.getElementById("char12").value = arrBuffer[i][17].replace("Char12=", "");
            document.getElementById("char13").value = arrBuffer[i][18].replace("Char13=", "");
            document.getElementById("char14").value = arrBuffer[i][19].replace("Char14=", "");
            document.getElementById("char15").value = arrBuffer[i][20].replace("Char15=", "");
            document.getElementById("char16").value = arrBuffer[i][21].replace("Char16=", "");
            document.getElementById("char17").value = arrBuffer[i][22].replace("Char17=", "");
            document.getElementById("char18").value = arrBuffer[i][23].replace("Char18=", "");
            document.getElementById("char19").value = arrBuffer[i][24].replace("Char19=", "");
            document.getElementById("char20").value = arrBuffer[i][25].replace("Char20=", "");
            document.getElementById("char21").value = arrBuffer[i][26].replace("Char21=", "");
            document.getElementById("char22").value = arrBuffer[i][27].replace("Char22=", "");
            document.getElementById("char23").value = arrBuffer[i][28].replace("Char23=", "");
            document.getElementById("char24").value = arrBuffer[i][29].replace("Char24=", "");
            document.getElementById("next").disabled=true;
            document.getElementById("previous").disabled=true;
        }
        else {
            document.getElementById("messages").value="No matching record found. Change the title to retry";
        }
    }
    else if (document.getElementById("update").checked === true) {
        document.getElementById("messages").value="Update posted";
        document.getElementById("next").disabled=true;
        document.getElementById("previous").disabled=true;
        arrBuffer[i][0] =  "Title=" + document.getElementById("title").value;
        arrBuffer[i][1] =  "Author=" + document.getElementById("author").value;
        arrBuffer[i][2] =  "Source=" + document.getElementById("source").value;
        arrBuffer[i][3] =  "Series=" + document.getElementById("series").value;
        arrBuffer[i][4] =  "Start=" + document.getElementById("start").value;
        arrBuffer[i][5] =  "Finish=" + document.getElementById("finish").value;
        arrBuffer[i][6] =  "Char01=" + document.getElementById("char01").value;
        arrBuffer[i][7] =  "Char02=" + document.getElementById("char02").value;
        arrBuffer[i][8] =  "Char03=" + document.getElementById("char03").value;
        arrBuffer[i][9] =  "Char04=" + document.getElementById("char04").value;
        arrBuffer[i][10] = "Char05=" + document.getElementById("char05").value;
        arrBuffer[i][11] = "Char06=" + document.getElementById("char06").value;
        arrBuffer[i][12] = "Char07=" + document.getElementById("char07").value;
        arrBuffer[i][13] = "Char08=" + document.getElementById("char08").value;
        arrBuffer[i][14] = "Char09=" + document.getElementById("char09").value;
        arrBuffer[i][15] = "Char10=" + document.getElementById("char10").value;
        arrBuffer[i][16] = "Char11=" + document.getElementById("char11").value;
        arrBuffer[i][17] = "Char12=" + document.getElementById("char12").value;
        arrBuffer[i][18] = "Char13=" + document.getElementById("char13").value;
        arrBuffer[i][19] = "Char14=" + document.getElementById("char14").value;
        arrBuffer[i][20] = "Char15=" + document.getElementById("char15").value;
        arrBuffer[i][21] = "Char16=" + document.getElementById("char16").value;
        arrBuffer[i][22] = "Char17=" + document.getElementById("char17").value;
        arrBuffer[i][23] = "Char18=" + document.getElementById("char18").value;
        arrBuffer[i][24] = "Char19=" + document.getElementById("char19").value;
        arrBuffer[i][25] = "Char20=" + document.getElementById("char20").value;
        arrBuffer[i][26] = "Char21=" + document.getElementById("char21").value;
        arrBuffer[i][28] = "Char23=" + document.getElementById("char23").value;
        arrBuffer[i][29] = "Char24=" + document.getElementById("char24").value;

        for (i = 0; i < (arrBuffer.length - 1); i++) {
            strData = strData + arrBuffer[i][0] + "|" + arrBuffer[i][1] + "|" + arrBuffer[i][2] + "|" + arrBuffer[i][3] + "|"
                  + arrBuffer[i][4] + "|" + arrBuffer[i][5] + "|" + arrBuffer[i][6] + "|" + arrBuffer[i][7] + "|"
                  + arrBuffer[i][8] + "|" + arrBuffer[i][9] + "|" + arrBuffer[i][10] + "|" + arrBuffer[i][11] + "|"
                  + arrBuffer[i][12] + "|" + arrBuffer[i][13] + "|" + arrBuffer[i][14] + "|" + arrBuffer[i][15] + "|"
                  + arrBuffer[i][16] + "|" + arrBuffer[i][17] + "|" + arrBuffer[i][18] + "|" + arrBuffer[i][19] + "|"
                  + arrBuffer[i][20] + "|" + arrBuffer[i][21] + "|" + arrBuffer[i][22] + "|" + arrBuffer[i][23] + "|"
                  + arrBuffer[i][24] + "|" + arrBuffer[i][25] + "|" + arrBuffer[i][26] + "|" + arrBuffer[i][27] + "|"
                  + arrBuffer[i][28] + "|" + arrBuffer[i][29];
             if (i < arrBuffer.length - 2) {
                 strData = strData + "%0A";
             }
        }

        let options = {
            method: 'POST',
            body: strData
        }
        let req = new Request(url1, options);
        fetch(req)
          .then ((response) => {
              if (response.ok) {
                  return response.text();
              }
              else {
                  throw new Error('Bad HTTP');
              }
          })
          .then( (j) => {
              console.log(j);
          })
          .catch( (err) => {
              console.log('ERROR:', err.message);
          })
        arrBuffer.length = 0;
        document.getElementById("messages").value="Update posted. Refetch records to proceed";
        document.getElementById("submit").disabled=true;
        document.getElementById("submit").style="color:black";

    }
}

    function fetchNext() {
        if (i === arrBuffer.length - 2) {
            document.getElementById("messages").value="At last record";
            document.getElementById("messages").style="color:red";
        }
        else {
            i++;
            document.getElementById("messages").value="Click 'Next' or 'Previous' to step through records";
            document.getElementById("messages").style="color:black";
            document.getElementById("title").value = arrBuffer[i][0].replace("Title=", "");
            document.getElementById("author").value = arrBuffer[i][1].replace("Author=", "");
            document.getElementById("source").value = arrBuffer[i][2].replace("Source=", "");
            document.getElementById("series").value = arrBuffer[i][3].replace("Series=", "");
            document.getElementById("start").value = arrBuffer[i][4].replace("Start=", "");
            document.getElementById("finish").value = arrBuffer[i][5].replace("Finish=", "");
            document.getElementById("char01").value = arrBuffer[i][6].replace("Char01=", "");
            document.getElementById("char02").value = arrBuffer[i][7].replace("Char02=", "");
            document.getElementById("char03").value = arrBuffer[i][8].replace("Char03=", "");
            document.getElementById("char04").value = arrBuffer[i][9].replace("Char04=", "");
            document.getElementById("char05").value = arrBuffer[i][10].replace("Char05=", "");
            document.getElementById("char06").value = arrBuffer[i][11].replace("Char06=", "");
            document.getElementById("char07").value = arrBuffer[i][12].replace("Char07=", "");
            document.getElementById("char08").value = arrBuffer[i][13].replace("Char08=", "");
            document.getElementById("char09").value = arrBuffer[i][14].replace("Char09=", "");
            document.getElementById("char10").value = arrBuffer[i][15].replace("Char10=", "");
            document.getElementById("char11").value = arrBuffer[i][16].replace("Char11=", "");
            document.getElementById("char12").value = arrBuffer[i][17].replace("Char12=", "");
            document.getElementById("char13").value = arrBuffer[i][18].replace("Char13=", "");
            document.getElementById("char14").value = arrBuffer[i][19].replace("Char14=", "");
            document.getElementById("char15").value = arrBuffer[i][20].replace("Char15=", "");
            document.getElementById("char16").value = arrBuffer[i][21].replace("Char16=", "");
            document.getElementById("char17").value = arrBuffer[i][22].replace("Char17=", "");
            document.getElementById("char18").value = arrBuffer[i][23].replace("Char18=", "");
            document.getElementById("char19").value = arrBuffer[i][24].replace("Char19=", "");
            document.getElementById("char20").value = arrBuffer[i][25].replace("Char20=", "");
            document.getElementById("char21").value = arrBuffer[i][26].replace("Char21=", "");
            document.getElementById("char22").value = arrBuffer[i][27].replace("Char22=", "");
            document.getElementById("char23").value = arrBuffer[i][28].replace("Char23=", "");
            document.getElementById("char24").value = arrBuffer[i][29].replace("Char24=", "");
        }
    }

    function fetchPrevious() {
        if (i === 0) {
            document.getElementById("messages").value="At first record";
            document.getElementById("messages").style="color:red";
        }
        else {
            i--;
            document.getElementById("messages").value="Click 'Next' or 'Previous' to step through records";
            document.getElementById("messages").style="color:black";
            document.getElementById("title").value = arrBuffer[i][0].replace("Title=", "");
            document.getElementById("author").value = arrBuffer[i][1].replace("Author=", "");
            document.getElementById("source").value = arrBuffer[i][2].replace("Source=", "");
            document.getElementById("series").value = arrBuffer[i][3].replace("Series=", "");
            document.getElementById("start").value = arrBuffer[i][4].replace("Start=", "");
            document.getElementById("finish").value = arrBuffer[i][5].replace("Finish=", "");
            document.getElementById("char01").value = arrBuffer[i][6].replace("Char01=", "");
            document.getElementById("char02").value = arrBuffer[i][7].replace("Char02=", "");
            document.getElementById("char03").value = arrBuffer[i][8].replace("Char03=", "");
            document.getElementById("char04").value = arrBuffer[i][9].replace("Char04=", "");
            document.getElementById("char05").value = arrBuffer[i][10].replace("Char05=", "");
            document.getElementById("char06").value = arrBuffer[i][11].replace("Char06=", "");
            document.getElementById("char07").value = arrBuffer[i][12].replace("Char07=", "");
            document.getElementById("char08").value = arrBuffer[i][13].replace("Char08=", "");
            document.getElementById("char09").value = arrBuffer[i][14].replace("Char09=", "");
            document.getElementById("char10").value = arrBuffer[i][15].replace("Char10=", "");
            document.getElementById("char11").value = arrBuffer[i][16].replace("Char11=", "");
            document.getElementById("char12").value = arrBuffer[i][17].replace("Char12=", "");
            document.getElementById("char13").value = arrBuffer[i][18].replace("Char13=", "");
            document.getElementById("char14").value = arrBuffer[i][19].replace("Char14=", "");
            document.getElementById("char15").value = arrBuffer[i][20].replace("Char15=", "");
            document.getElementById("char16").value = arrBuffer[i][21].replace("Char16=", "");
            document.getElementById("char17").value = arrBuffer[i][22].replace("Char17=", "");
            document.getElementById("char18").value = arrBuffer[i][23].replace("Char18=", "");
            document.getElementById("char19").value = arrBuffer[i][24].replace("Char19=", "");
            document.getElementById("char20").value = arrBuffer[i][25].replace("Char20=", "");
            document.getElementById("char21").value = arrBuffer[i][26].replace("Char21=", "");
            document.getElementById("char22").value = arrBuffer[i][27].replace("Char22=", "");
            document.getElementById("char23").value = arrBuffer[i][28].replace("Char23=", "");
            document.getElementById("char24").value = arrBuffer[i][29].replace("Char24=", "");
        }
    }

// functions for action buttons to display and hide help & internals ------------------------------

function showInternals() {
    var y = document.getElementById("internalsDiv");
    if (y.style.display === "none") {
        y.style.display = "block";
    } else {
        y.style.display = "none";
    }
}

function initSettings() {
    document.getElementById("none").checked=true;
    document.getElementById("reset").disabled=false;
    document.getElementById("messages").disabled=true;
    document.getElementById("messages").value="All fields disabled. Choose an action to proceed";
    document.getElementById("messages").style="color:black";
    document.getElementById("submit").disabled=true;
    document.getElementById("submit").style="color:black";
    document.getElementById("next").disabled=true;
    document.getElementById("previous").disabled=true;
    disableAllTextFields();
}

function fetchSettings() {
    resetSettings();
    document.getElementById("fetch").checked=true;
    document.getElementById("reset").disabled=false;
    document.getElementById("messages").disabled=true;
    document.getElementById("messages").style="color:black";
    document.getElementById("messages").value="Enter a title and click 'Submit'";
    document.getElementById("submit").disabled=false;
    document.getElementById("submit").style="color:red";
    document.getElementById("next").disabled=true;
    document.getElementById("previous").disabled=true;
    disableAllTextFields();
    document.getElementById("title").disabled=false;
}

function fetchAllSettings() {
    resetSettings();
    document.getElementById("fetchall").checked=true;
    document.getElementById("reset").disabled=false;
    document.getElementById("messages").style="color:black";
    document.getElementById("messages").disabled=true;
    document.getElementById("messages").value="Click 'Submit' to fetch all book records";
    document.getElementById("submit").disabled=false;
    document.getElementById("submit").style="color:red";
    document.getElementById("next").disabled=true;
    document.getElementById("previous").disabled=true;
    disableAllTextFields();
}

function updateSettings() {
    if ((arrBuffer.length - 2) === 0) {
        document.getElementById("messages").value="Nothing to update. Perform a Fetch or FetchAll before updating";
        document.getElementById("messages").style="color:red";
        document.getElementById("messages").disabled=true;
        document.getElementById("reset").disabled=false;
        document.getElementById("submit").disabled=true;
        document.getElementById("submit").style="color:black";
        document.getElementById("next").disabled=true;
        document.getElementById("previous").disabled=true;
        disableAllTextFields();
    }
    else {
        document.getElementById("reset").disabled=false;
        document.getElementById("messages").style="color:black";
        document.getElementById("messages").disabled=true;
        document.getElementById("messages").value="Make changes to any fetched record and click 'Submit'";
        document.getElementById("submit").disabled=false;
        document.getElementById("submit").style="color:red";
        document.getElementById("next").disabled=true;
        document.getElementById("previous").disabled=true;
        enableAllTextFields();
        document.getElementById("title").required=true;
    }
}

function deleteSettings() {
    document.getElementById("reset").disabled=false;
    document.getElementById("messages").style="color:black";
    document.getElementById("messages").disabled=true;
    document.getElementById("messages").value="Enter a book title and click 'Submit' to delete it";
    document.getElementById("submit").disabled=false;
    document.getElementById("submit").style="color:red";
    document.getElementById("next").disabled=true;
    document.getElementById("previous").disabled=true;
    disableAllTextFields();
}

function resetSettings() {
    document.getElementById("none").checked = true;
    document.getElementById("reset").disabled=false;
    document.getElementById("messages").style="color:black";
    document.getElementById("messages").disabled=true;
    document.getElementById("messages").value="All fields are reset";
    document.getElementById("submit").disabled=true;
    document.getElementById("submit").style="color:black";
    document.getElementById("next").disabled=true;
    document.getElementById("previous").disabled=true;
    disableAllTextFields();
    clearAllTextFields();
}

function disableAllTextFields() {
    document.getElementById("title").disabled=true;
    document.getElementById("title").required=false;
    document.getElementById("author").disabled=true;
    document.getElementById("source").disabled=true;
    document.getElementById("series").disabled=true;
    document.getElementById("start").disabled=true;
    document.getElementById("finish").disabled=true;
    document.getElementById("char01").disabled=true;
    document.getElementById("char02").disabled=true;
    document.getElementById("char03").disabled=true;
    document.getElementById("char04").disabled=true;
    document.getElementById("char05").disabled=true;
    document.getElementById("char06").disabled=true;
    document.getElementById("char07").disabled=true;
    document.getElementById("char08").disabled=true;
    document.getElementById("char09").disabled=true;
    document.getElementById("char10").disabled=true;
    document.getElementById("char11").disabled=true;
    document.getElementById("char12").disabled=true;
    document.getElementById("char13").disabled=true;
    document.getElementById("char14").disabled=true;
    document.getElementById("char15").disabled=true;
    document.getElementById("char16").disabled=true;
    document.getElementById("char17").disabled=true;
    document.getElementById("char18").disabled=true;
    document.getElementById("char19").disabled=true;
    document.getElementById("char20").disabled=true;
    document.getElementById("char21").disabled=true;
    document.getElementById("char22").disabled=true;
    document.getElementById("char23").disabled=true;
    document.getElementById("char24").disabled=true;
}

function enableAllTextFields() {
    document.getElementById("title").disabled=false;
    document.getElementById("author").disabled=false;
    document.getElementById("source").disabled=false;
    document.getElementById("series").disabled=false;
    document.getElementById("start").disabled=false;
    document.getElementById("finish").disabled=false;
    document.getElementById("char01").disabled=false;
    document.getElementById("char02").disabled=false;
    document.getElementById("char03").disabled=false;
    document.getElementById("char04").disabled=false;
    document.getElementById("char05").disabled=false;
    document.getElementById("char06").disabled=false;
    document.getElementById("char07").disabled=false;
    document.getElementById("char08").disabled=false;
    document.getElementById("char09").disabled=false;
    document.getElementById("char10").disabled=false;
    document.getElementById("char11").disabled=false;
    document.getElementById("char12").disabled=false;
    document.getElementById("char13").disabled=false;
    document.getElementById("char14").disabled=false;
    document.getElementById("char15").disabled=false;
    document.getElementById("char16").disabled=false;
    document.getElementById("char17").disabled=false;
    document.getElementById("char18").disabled=false;
    document.getElementById("char19").disabled=false;
    document.getElementById("char20").disabled=false;
    document.getElementById("char21").disabled=false;
    document.getElementById("char22").disabled=false;
    document.getElementById("char23").disabled=false;
    document.getElementById("char24").disabled=false;
}

function clearAllTextFields() {
    document.getElementById("title").value = "";
    document.getElementById("author").value = "";
    document.getElementById("source").value = "";
    document.getElementById("series").value = "";
    document.getElementById("start").value = "";
    document.getElementById("finish").value = "";
    document.getElementById("char01").value = "";
    document.getElementById("char02").value = "";
    document.getElementById("char03").value = "";
    document.getElementById("char04").value = "";
    document.getElementById("char05").value = "";
    document.getElementById("char06").value = "";
    document.getElementById("char07").value = "";
    document.getElementById("char08").value = "";
    document.getElementById("char09").value = "";
    document.getElementById("char10").value = "";
    document.getElementById("char11").value = "";
    document.getElementById("char12").value = "";
    document.getElementById("char13").value = "";
    document.getElementById("char14").value = "";
    document.getElementById("char15").value = "";
    document.getElementById("char16").value = "";
    document.getElementById("char17").value = "";
    document.getElementById("char18").value = "";
    document.getElementById("char19").value = "";
    document.getElementById("char20").value = "";
    document.getElementById("char21").value = "";
    document.getElementById("char22").value = "";
    document.getElementById("char23").value = "";
    document.getElementById("char24").value = "";
}
