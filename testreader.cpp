/* testreader.cpp
 * Author: Anthony Roenker
 * Date: 2/25/26
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main (int argc, char **argv)
{
    // Create Bible object to process the raw text file
    Bible webBible("/home/class/csc3004/Bibles/web-complete");

    if (argc < 4) {
        cout << "Error: verse number is missing" << endl;
        return 1;
    }

    Verse verse;
    int bookNum, chapterNum, verseNum;
    LookupResult result;

    int numOfVerses;

    // Set all variables to command line arguments
    bookNum = atoi(argv[1]);
    chapterNum = atoi(argv[2]);
    verseNum = atoi(argv[3]);

    // If 4th argument exists, use it as count
    if (argc == 5) {
        numOfVerses = atoi(argv[4]);
    }
    else {
        numOfVerses = 1;
    }

    // Create a reference from the numbers
    Ref ref(bookNum, chapterNum, verseNum);

    // Loop to display the requested number of verses
    for (int i = 0; i < numOfVerses; i++) {

        // Lookup the verse
        verse = webBible.lookup(ref, result);

        if (result != SUCCESS) {
            cout << webBible.error(result) << endl;
            return 1;
        }

        // Display the verse
        verse.display();
        // cout << endl;
		// cout << webBible.lookup() << endl;
		cout << "Last Offset added: " << webBible.getLastOffset() << endl;
		
        // Get the next reference for the next loop iteration
        ref = webBible.next(ref, result);

        if (result != SUCCESS) {
            cout << webBible.error(result) << endl;
            return 1;
        }
    }
    return 0;
}
