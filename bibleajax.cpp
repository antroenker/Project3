/* Demo server program for Bible lookup using AJAX/CGI interface
 * By James Skon, Febrary 10, 2011
 * updated by Bob Kasper, January 2020
 * updated by Deborah Wilson, February 2026
 * Mount Vernon Nazarene University
 *
 * This sample program works using the cgicc AJAX library to
 * allow live communication between a web page and a program running on the
 * same server that hosts the web server.
 *
 * This program is run by a request from the associated html web document.
 * A Javascript client function invokes an AJAX request,
 * passing the input form data as the standard input string.
 *
 * The cgi.getElement function parses the input string, searching for the matching
 * field name, and returing a "form_iterator" oject, which contains the actual
 * string the user entered into the corresponding field. The actual values can be
 * accessed by dereferencing the form iterator twice, e.g. **verse
 * refers to the actual string entered in the form's "verse" field.
 *
 * STUDENT NAME: Anthony Roenker
 */

#include <iostream>
#include <stdio.h>
#include <string.h>

/* Required libraries for AJAX to function */
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"

#include "fifo.h"

using namespace std;
using namespace cgicc;

int main()
{
    // CGI header
    cout << "Content-Type: text/plain\n\n";

    Cgicc cgi;

    // Read form fields
    form_iterator book     = cgi.getElement("book");
    form_iterator chapter  = cgi.getElement("chapter");
    form_iterator verse    = cgi.getElement("verse");
    form_iterator nv       = cgi.getElement("num_verse");

    // Convert to integers
    int bookNum    = book->getIntegerValue();
    int chapterNum = chapter->getIntegerValue();
    int verseNum   = verse->getIntegerValue();
    int numVerses  = nv->getIntegerValue();

    // Basic validation
    bool validInput = true;

    if (bookNum <= 0 || bookNum > 66)
        validInput = false;
    if (chapterNum <= 0)
        validInput = false;
    if (verseNum <= 0)
        validInput = false;
    if (numVerses <= 0)
        numVerses = 1;   // default to 1 verse

    if (!validInput)
    {
        cout << "ERR|Invalid input values\n";
        return 0;
    }

    // FIFO pipes
    Fifo request("request");
    Fifo reply("reply");

    // Loop for multiple verses
    for (int i = 0; i < numVerses; i++)
    {
        // Build request string
        string message = to_string(bookNum) + ":" +
                         to_string(chapterNum) + ":" +
                         to_string(verseNum);

        // Send request
        request.openwrite();
        request.send(message.c_str());
        request.fifoclose();

        // Receive reply
        reply.openread();
        string response = reply.recv();
        reply.fifoclose();

        // Output to browser
        cout << response << "\n";

        // Move to next verse
        verseNum++;
    }
    return 0;
}