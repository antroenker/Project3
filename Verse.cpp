/* Verse class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME:Anthony Roenker 
 */

#include "Verse.h"
#include <iostream>
#include <sstream>

using namespace std;

// Default constructor
Verse::Verse()
{  	
   verseText = "Uninitialized Verse!";
   verseRef = Ref();
} 

// REQUIRED: Parse constructor - pass verse string from file
Verse::Verse(const string s)
{
   // TODO: use Ref constructor to create verseRef
   // store the remainder of the string in verseText
   
   verseRef = Ref(s);
   
   // Removes leading space if needed
   if(!verseText.empty() && verseText[0] == ' ')
	   verseText.erase(0, 1);
}  	
// The "clean" constructor, purpose is to store verseRef, and verseText
Verse::Verse(Ref r, string text)
{
	verseRef = r;
	verseText = text;
		
	// Removes leading space if needed
	if (!verseText.empty() && verseText[0] == ' ')
	{
		verseText.erase(0, 1);
	}
}
// REQUIRED: Accessors
string Verse::getVerse()
{
   return verseText;	   
}

Ref Verse::getRef()
{
   return verseRef;
}

// display reference and verse
void Verse::display()
{
   verseRef.display();
   cout << " " << verseText << endl;
   // use <br> so html can read \n as a newline
   // cout << "<br>\n";
}