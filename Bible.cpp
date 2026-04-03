/* Bible class function definitions
 * Computer Science, MVNU
 * CSC-3004 Introduction to Software Development
 *
 * NOTE: You may add code to this file, but do not
 * delete any code or delete any comments.
 *
 * STUDENT NAME:Anthony Roenker 
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <map>

using namespace std;

// Default constructor
Bible::Bible()
{ 
   infile = "/home/class/csc3004/Bibles/web-complete";
}

// Constructor – pass bible filename
Bible::Bible(const string s) 
{ 
	infile = s; 
	buildTextIndex();
	
}

void Bible::buildTextIndex()
{
	refs.clear();  // Clear any previous index entries

    ifstream fin(infile.c_str()); // Opens bible for reading
	
	// If Bible cannot open, error returned
    if (!fin.is_open()) {
        cerr << "Error - cannot open Bible file for indexing: " << infile << endl;
        return;
    }
	
	//Offset
	streampos pos;
	
    string line;
    while (true) {
        // Record the byte offset before reading the line
        pos = fin.tellg();

        if (!getline(fin, line))
            break;  // End of file

        // Build the reference object
        Ref r(line);

        // Store the offset in the map
        refs[r] = pos;
    }

	lastOffset = pos;
	
    fin.close();
	
	cerr << "Index build. Total verses indexed: " << refs.size() << endl;
}

streampos Bible::getLastOffset() const 
{
	return lastOffset;
}

// Initializes a text file to string and passed to Bibles defualt constructor
string Bible::bibleVersion(int i) // make sure to declare this into the header file
{
	if (i == 1)
        return "/home/class/csc3004/Bibles/web-complete";
    else if (i == 2)
        return "/home/class/csc3004/Bibles/kjv-complete";
    else if (i == 3)
        return "/home/class/csc3004/Bibles/dby-complete";
    else if (i == 4)
        return "/home/class/csc3004/Bibles/ylt-complete";
    else if (i == 5)
        return "/home/class/csc3004/Bibles/webster-complete";
	else return "Should never happen, but who knows lol";
}

// REQUIRED: lookup finds a given verse in this Bible
Verse Bible::lookup(Ref ref, LookupResult& status)
{ 
   // TODO: scan the file to retrieve the line that holds ref ...
	
	instream.open(infile.c_str()); // opens the file
	
	if (!instream.is_open()) {   // checks if file failed to open
		status = OTHER;
		cerr << "Error - can't open input file: " << infile << endl;
		return Verse();
	}

	bool chapterFound = false;
	bool bookFound = false;

    string line1;  // defined string for book, chapter, verse number
	status = NO_VERSE;
	
		while (getline(instream, line1)){
		
			int book, chapter, verse;
			char s1, s2;
			string line2;
			
			// stringstream parses the reference from book(int) to
			// colon";" to chapter(int) to colon":" to verse(int)
			stringstream sep(line1);
			sep >> book >> s1 >> chapter >> s2 >> verse;
			getline(sep, line2);
			
			//string current = line1; // original verse reference
			
			//Ref r(current); // Calls ref's parse constructor
			
			Ref current(book, chapter, verse); 
		
			if(book == ref.getBook()){
			
				bookFound = true;
			
				if(chapter == ref.getChapter()){
					
					chapterFound = true;
					
					if (current == ref) // sees if current is the reference
					{
						status = SUCCESS;
						instream.close();
						return Verse(current, line2);
						// Version 2: return Verse(r, current);
				
					}
				}
			}
		}
	
	if (!bookFound)
        status = NO_BOOK;
    else if (!chapterFound)
        status = NO_CHAPTER;
    else
        status = NO_VERSE;
    
   // update the status variable
   // status = OTHER; // placeholder until retrieval is attempted
	
   // create and return the verse object
   Verse aVerse;   // default verse, to be replaced by a Verse object
	                // that is constructed from a line in the file.
   return(aVerse);
}

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
Verse Bible::nextVerse(LookupResult& status)
{
    if (!instream.is_open()) {
        instream.open(infile.c_str());
        if (!instream.is_open()) {
            status = OTHER;
            return Verse();
        }
		string line;
		if (!getline(instream, line)) {
			status = OTHER;
			// instream.close();
			return Verse();
		}
		
		status = SUCCESS;
		return Verse(line);
    }
	string line;
	if(!getline(instream, line)) {
		status = OTHER;
		instream.close();
		return Verse();
	}
	
	status = SUCCESS;
	return Verse(line);
	/*
    int book, chapter, verse;
    char c1, c2;
    string text;

    stringstream sep(line);
    sep >> book >> c1 >> chapter >> c2 >> verse;
    getline(sep, text);

    Ref ref(book, chapter, verse);

    status = SUCCESS;
    return Verse(ref, text);
	*/
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status)
{	
		if (status == NO_BOOK){
			return "Error: no such book ";
		}
		if (status == NO_CHAPTER){
			return "Error: no such chapter ";
		}
		if (status == NO_VERSE){
			return "Error: no such verse ";			
		}	
		return "";
}

void Bible::display()
{
   cout << "Bible file: " << infile << endl;
}
	
// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref
Ref Bible::next(const Ref ref, LookupResult& status)
{
    // Open the file
    instream.open(infile.c_str());
    if (!instream.is_open()) {
        status = OTHER;
        return ref;
    }

    string line;
    bool found = false;

    // Scan until we find the current reference
    while (getline(instream, line)) {

        Ref current(line);

        if (current == ref) {
            found = true;
            break;
        }
    }
	// If we never found the reference, something is wrong
    if (!found) {
        status = OTHER;
        instream.close();
        return ref;
    }

    // Now read the NEXT line
    if (!getline(instream, line)) {
        // End of file
        status = OTHER;
        instream.close();
        return ref;
    }

    int book, chapter, verse;
    char c1, c2;
    string text;

    stringstream sep(line);
    sep >> book >> c1 >> chapter >> c2 >> verse;
    getline(sep, text);

    instream.close();

    status = SUCCESS;
    return Ref(book, chapter, verse);

}

// OPTIONAL: Return the reference before the given ref
Ref Bible::prev(const Ref ref, LookupResult& status)
{
   return ref;
}

