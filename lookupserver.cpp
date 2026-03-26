/* testreader.cpp
 * Author: Anthony Roenker
 * Date: 2/25/26
 */

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include "fifo.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

using namespace std;

int main (int argc, char **argv)
{
    Bible webBible("/home/class/csc3004/Bibles/web-complete");

    while (true)
    {
        // Request Pipe
        Fifo request("request");
        request.openread();
		
        string message = request.recv();
        cout << "Received: " << message << endl;

        string replyMessage;

        // Empty request
        if (message.empty()) {
            replyMessage = "ERR|Empty request";
        }
        else {
            // Count colons
            int colonCount = 0;
            for (char c : message) {
                if (c == ':') colonCount++;
            }

            if (colonCount != 2) {
                replyMessage = "ERR|Invalid format. Use book:chapter:verse";
            } else {
                
                size_t first  = message.find(':');
                size_t second = message.find(':', first + 1);

                string bookStr  = message.substr(0, first);
                string chapStr  = message.substr(first + 1, second - first - 1);
                string verseStr = message.substr(second + 1);

                // Error handling for missing fields
                if (bookStr.empty())       replyMessage = "ERR|Missing book number";
                else if (chapStr.empty())  replyMessage = "ERR|Missing chapter number";
                else if (verseStr.empty()) replyMessage = "ERR|Missing verse number";
                else {
                    int book, chapter, verseNum;

                    // Numeric checks
					// "(...)" means to catch any exception
                    try 
					{ book = stoi(bookStr); }
                    catch (...) { replyMessage = "ERR|Book must be a number"; }

                    try 
					{ chapter = stoi(chapStr); }
                    catch (...) { replyMessage = "ERR|Chapter must be a number"; }

                    try 
					{ verseNum = stoi(verseStr); }
                    catch (...) { replyMessage = "ERR|Verse must be a number"; }

                    // Range checks
                    if (replyMessage.empty()) {
                        if (book <= 0) {
							replyMessage = "ERR|Book number must be positive";
						}
                        else if (book > 66) {
							replyMessage = "ERR|Book number out of range";
						}
                        else if (chapter <= 0) {
							replyMessage = "ERR|Chapter number must be positive";
						}
                        else if (verseNum <= 0) {
							replyMessage = "ERR|Verse number must be positive";
						}
                        else {
                            //Ref object
                            Ref ref(message);

                            LookupResult result;
                            Verse verse = webBible.lookup(ref, result);

                            if (result == SUCCESS)
                                replyMessage = "OK|" + verse.getVerse();
                            else if (result == NO_BOOK)
                                replyMessage = "ERR|Invalid book number";
                            else if (result == NO_CHAPTER)
                                replyMessage = "ERR|Invalid chapter";
                            else if (result == NO_VERSE)
                                replyMessage = "ERR|Invalid verse";
                            else
                                replyMessage = "ERR|Unknown error";
                        }
                    }
                }
            }
        }

        // Reply Pipe
        Fifo reply("reply");
        reply.openwrite();
        reply.send(replyMessage);
        cout << "Sent: " << replyMessage << endl;
    }

    return 0;
}