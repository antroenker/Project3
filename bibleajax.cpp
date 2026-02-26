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

#include "Bible.h"
#include "Ref.h"
#include "Verse.h"

using namespace std;
using namespace cgicc;

int main()
{
   /* A CGI program must send a response header with content type
    * back to the web client before any other output.
    * For an AJAX request, our response is not a complete HTML document,
    * so the response type is just plain text to insert into the web page.
    */
   cout << "Content-Type: text/plain\n\n";

   Cgicc cgi;  // create object used to access CGI request data

   // GET THE INPUT DATA
   // browser sends us a string of field name/value pairs from HTML form
   // retrieve the value for each appropriate field name
   form_iterator st = cgi.getElement("search_type");
   form_iterator book = cgi.getElement("book");
   
   // Added Bible Versions
   form_iterator bibleVersion = cgi.getElement("Bible");
   if (bibleVersion == cgi.getElements().end() || bibleVersion->isEmpty())
   {
	   cout << "<p>Error: Bible version not provided. <p>";
	   return 0;
   }
   
   form_iterator chapter = cgi.getElement("chapter");
   form_iterator verse = cgi.getElement("verse");
   form_iterator nv = cgi.getElement("num_verse");

   // Convert and check input data
   bool validInput = false;
   
   // Extracts form-iterator values to integer values
   int bibleVersionNumber = bibleVersion->getIntegerValue();
   int bookNum = book->getIntegerValue();
   string bookName = Ref::bookNames[bookNum];
   int chapterNum = chapter->getIntegerValue();
   int verseNum = verse->getIntegerValue();
   int numOfVerses = nv->getIntegerValue();
   
   if (chapter != cgi.getElements().end())
   {
      int chapterNum = chapter->getIntegerValue();
      if (chapterNum > 150)
      {
         cout << "<p>The chapter number (" << chapterNum << ") is too high.</p>" << endl;
      }
      else if (chapterNum <= 0)
      {
         cout << "<p>The chapter must be a positive number.</p>" << endl;
      }
	  
	  // added error handling
      else if (verseNum > 176)
	  {
		 cout << "<p>The verse number (" << verseNum << ") is too high.</p>" << endl;
	  }
	  else if (verseNum <= 0)
	  {
		 cout << "<p>The verse must be a positive number.</p>" << endl;
	  }	
	  else if (numOfVerses == 0 || numOfVerses < 0)
	  {
		  cout << "<p>Number of verses must be a positive number.<p>" << endl;
	  }
	  else
      {
         validInput = true;
      }
   }
	
   // TODO: OTHER INPUT VALUE CHECKS ARE NEEDED ... but that's up to you!

   /* TODO: PUT CODE HERE TO CALL YOUR BIBLE CLASS FUNCTIONS
    *        TO LOOK UP THE REQUESTED VERSES
    */
   
   // Ref object created from integer values
   Ref ref(bookNum, chapterNum, verseNum);
   Verse Verse;
   LookupResult status;
   // For Bible versions
   string fileName = Bible::bibleVersion(bibleVersionNumber);
   Bible bible(fileName);
  
   /* SEND BACK THE RESULTS
    * Finally we send the result back to the client on the standard output stream
    * in HTML text format.
    * This string will be inserted as is inside a container on the web page,
    * so we must include HTML formatting commands to make things look presentable!
    */
   if (validInput)
   {
	    /*
        cout << "Search Type: <b>" << **st << "</b>" << endl;
        cout << "<p>Your result: <br>"; 
	    // changed **book to bookName
        << bookName << " " << **chapter << ":" << **verse 
        << "<em> The " << **nv
        << " " << Verse.getVerse() << endl;
	    */
		if (numOfVerses <= 0)
			numOfVerses = 1;
			
		for(int i = 0; i < numOfVerses; i++){
   
		// uses verse object to look up the reference
		Verse = bible.lookup(ref, status);
		
		if(status != SUCCESS){
			if (status == NO_BOOK){
				cout << bible.error(status); 
				cout << bookNum << endl;
			}
			else if (status == NO_CHAPTER){
				cout << bible.error(status); 
				cout << chapterNum
				<< " in " << Ref::bookNames[bookNum] << endl;
			}
			else if (status == NO_VERSE){
				cout << bible.error(status);
				cout << (verseNum)
					<< " in " << Ref::bookNames[bookNum]
					<< " " << chapterNum << endl;	
			}
			return 1;
		}
		Verse.display();
		cout << endl;
		ref = bible.next(ref, status);
		}
    }		
    else if (status == NO_BOOK){
	    cout << bible.error(status); 
		cout << bookNum << endl;
	}
	else if (status == NO_CHAPTER){
		cout << bible.error(status); 
		cout << chapterNum
		<< " in " << Ref::bookNames[bookNum] << endl;
		}
	else if (status == NO_VERSE){
		cout << bible.error(status);
		cout << (verseNum)
		<< " in " << Ref::bookNames[bookNum]
		<< " " << chapterNum << endl;	
	}	
    else 
    {
      cout << "<p>Re-visit one of your inputs!</em></p>" << endl;
    }
    return 0;
}