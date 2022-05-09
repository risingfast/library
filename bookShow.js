// bookShow.js
// Author: Geoff Jarman
// References: 
//     https://www.youtube.com/watch?v=_5yhmkDQqIQ&list=PLyuRouwmQCjmQTKvgqIgah03HF1wrYkA9&index=14
//      https://medium.com/@dtkatz/3-ways-to-fix-the-cors-error-and-how-access-control-allow-origin-works-d97d55946d9
// Log:
//    09/11/2020 started
//    09/12/2020 changed cgi target to bookShow.cgi
//    09/15/2020 changed choices to titles and Characters
//    09/15/2020 changed Titles listing to show start and end dates
//    09/16/2020 add authors option
// Enhancements:
//

'use strict';

const uri = "http://www.risingfast.com/cgi-bin/bookShow.cgi";


// function to ajax fetch text1 from the server ---------------------------------------------------

async function bookShow() {

    var arrBuffer = new Array(2);
    var strTemp = "";
    var text1 = "";
    var i = 0;
    var j = 0;
    var k = 0;
    var BookTitle = "";
    var intPos = 0;
    var boolPosFound = "";

    let response = await fetch(uri);
    if (response.ok) {

// read the file into a two-dimensional array of strings

        let text = await response.text();
        arrBuffer = text.split("\n");
        for (i = 0; (i < arrBuffer.length - 1); i++) {
            arrBuffer[i] = arrBuffer[i].split("|");
        }
        document.getElementById("messages").value="bookShow() fired";

// list characters if the characters option is chosen

        if (document.getElementById("characters").checked === true) {
            
            BookTitle = document.getElementById("BookTitle").value;

            for (var i = 0; i < arrBuffer.length - 1; i++) {
                if (arrBuffer[i][0].replace("Title=", "") === BookTitle) {
                    intPos = i;
                    boolPosFound = "Found";
                    break;
                }
            }

            if (Boolean(boolPosFound) === true) {

                strTemp = strTemp + arrBuffer[intPos][0].replace("Title=", "") + ": ";
                for (var j = 6; j < arrBuffer[intPos].length; j++) {
                    if (arrBuffer[intPos][j].length === 7) {
                        continue;
                    }
                    else {
                        strTemp = strTemp + arrBuffer[intPos][j].replace(/Char..=/, "") + "\n";
                    }
                }
                if (strTemp.charAt(strTemp.length - 2) === ":") {
                    strTemp = strTemp + "No characters recorded";
                }
    
                    document.getElementById("results").value=strTemp;
                    strTemp = strTemp + "\n";
            }
            else {
                strTemp = "No matching title found. Change the book title and retry"
                document.getElementById("results").value=strTemp;
            }

        }

// list titles if the titles option is chosen

        else if (document.getElementById("titles").checked === true) {
            for (var i = 0; i < arrBuffer.length - 1; i++) {
                strTemp = strTemp + arrBuffer[i][0].replace("Title=", "") + ": ";
                for (var j = 1; j < 6; j++) {
                    strTemp = strTemp + arrBuffer[i][j] + "; ";
                }
                strTemp = strTemp + "\n";
            }
            document.getElementById("results").value=strTemp;
        }

// list authors if the authors option is chosen

        else if (document.getElementById("authors").checked === true) {
            for (var i = 0; i < arrBuffer.length - 1; i++) {
                strTemp = strTemp + arrBuffer[i][1].replace("Author=", "") + ": ";
                strTemp = strTemp + arrBuffer[i][0].replace("Title=", "");
                strTemp = strTemp + "\n";
            }
            document.getElementById("results").value=strTemp;
        }

// show error if no valid option selected

        else {
            document.getElementById("results").value = "This wasn't supposed to happen!";
        }
    } else {
        alert("HttpError: " + response.status);
    }
}

// function to set form status based on action chosen ---------------------------------------------

function getAction() {

    if (document.getElementById("none").checked === true) {
        document.getElementById("titles").disabled = false;
        document.getElementById("authors").disabled = false;
        document.getElementById("characters").disabled = false;
        document.getElementById("go").disabled = true;
        document.getElementById("reset").disabled = true;
        document.getElementById("messages").value = "No action selected";
        document.getElementById("titles").required = false;
        document.getElementById("titles").required = false;
        document.getElementById("characters").required = false;
        document.getElementById("titles").required = false;
        document.getElementById("authors").required = false;
        document.getElementById("results").value = "";
    }
    else if (document.getElementById("titles").checked === true) {
        document.getElementById("characters").disabled = false;
        document.getElementById("authors").disabled = false;
        document.getElementById("none").disabled = false;
        document.getElementById("go").disabled = false;
        document.getElementById("reset").disabled = false;
        document.getElementById("messages").value = "Titles selected - click Go to proceed";
        document.getElementById("none").required = false;
        document.getElementById("characters").required = false;
        document.getElementById("authors").required = false;
        document.getElementById("results").value = "";
    } 
    else if (document.getElementById("characters").checked === true) {
        document.getElementById("titles").disabled = false;
        document.getElementById("authors").disabled = false;
        document.getElementById("none").disabled = false;
        document.getElementById("go").disabled = false;
        document.getElementById("reset").disabled = false;
        document.getElementById("messages").value = "Characters selected - a title is required";
        document.getElementById("none").required = false;
        document.getElementById("titles").required = false;
        document.getElementById("authors").required = false;
        document.getElementById("BookTitle").required = true;
        document.getElementById("results").value = "";
    }
    else if (document.getElementById("authors").checked === true) {
        document.getElementById("characters").disabled = false;
        document.getElementById("titles").disabled = false;
        document.getElementById("none").disabled = false;
        document.getElementById("go").disabled = false;
        document.getElementById("reset").disabled = false;
        document.getElementById("messages").value = "Authors selected - click Go to proceed";
        document.getElementById("none").required = false;
        document.getElementById("characters").required = false;
        document.getElementById("titles").required = false;
        document.getElementById("results").value = "";
    }
    else {
        document.getElementById("characters").disabled = true;
    }
}

// function to reset fields -----------------------------------------------------------------------

function reset() {
    document.getElementById("characters").disabled = true;
    document.getElementById("go").disabled = true;
    document.getElementById("reset").disabled = true;
    document.getElementById("messages").value = "Reset action selected - fields are reset";
    document.getElementById("characters").required = false;
    document.getElementById("results").value = "";
    document.getElementById("none").checked = true;
    document.getElementById("titles").checked = false;
    document.getElementById("characters").checked = false;
    document.getElementById("authors").checked = false;
}

// function to show internal information ----------------------------------------------------------

function showInternals() {
    var y = document.getElementById("internalsDiv");
    if (y.style.display === "none") {
        y.style.display = "block";
    } else {
         y.style.display = "none";
    }
    document.getElementById("Messages").value="See internal information below the Internals button. Click Internals again to hide it"
}

// function to show help information --------i-----------------------------------------------------

function showHelp() {
    var x = document.getElementById("helpDiv");
    if (x.style.display === "none") {
        x.style.display = "block";
    } else {
        x.style.display = "none";
    }
    document.getElementById("Messages").value="See Help information below the Help button. Click Help again to hide it"
}

// fetch the initial display attribute for the internals division ---------------------------------

var x = document.getElementById("helpDiv");
const stx = getComputedStyle(x);
x.style.display = stx.display;
var y = document.getElementById("internalsDiv");
const sty = getComputedStyle(y);
y.style.display = sty.display;

getAction();


