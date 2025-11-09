#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stddef.h>

typedef struct WordNode_struct {
    char* myWord;
    struct WordNode_struct* next; 
} WordNode;

typedef struct LadderNode_struct {
    WordNode* topWord;
    struct LadderNode_struct* next; 
} LadderNode;


//------------------- \/\/\/ TOP OF TASK 1 \/\/\/ --------------------

int countWordsOfLength(char* filename, int wordSize) { 
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        return -1;
    }
    int count = 0;
    char currWord[30];
    while(fscanf(f, "%s", currWord) > 0) {
        count += (strlen(currWord) == wordSize);
    }
    fclose(f);
    return count;

    //---------------------------------------------------------
    // TODO - write countWordsOfLength()    
    //---------------------------------------------------------
}

bool buildWordArray(char* filename, char** words, int numWords, int wordSize) { 
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        return false;
    }
    char currWord[30];

    unsigned int i = 0;
    while(fscanf(f, "%s\n", currWord) > 0) {
        if(strlen(currWord) == wordSize) {
            strcpy(words[i], currWord);
            i++;
        }
    }
    fclose(f);
    return (i == numWords);

}

int strCmpInd(char* str1, char* str2) {
    char *shortStr = (strlen(str1) < strlen(str2)) ? str1 : str2;
    char *longStr = (strlen(str1) >= strlen(str2)) ? str1 : str2;
    int i;
    for(i = 0; shortStr[i] != '\0'; i++) {
        if(shortStr[i] != longStr[i]) {
            return i;
        }
    }
    return (longStr[i] == '\0') ? -1 : i;
}

int findWord(char** words, char* aWord, int loInd, int hiInd) {
    if(loInd > hiInd) {
        return -1;
    }
    int midInd = (loInd + hiInd) / 2;


    if(strcmp(words[midInd], aWord) == 0) {
        return midInd;
    }

    int diffInd = strCmpInd(words[midInd], aWord);

    if((words[midInd])[diffInd] > aWord[diffInd]) {
        // too high, search bottom of current list
        return findWord(words, aWord, loInd, midInd-1);
    }
    else {
        return findWord(words, aWord, midInd+1, hiInd);
    }
}

void freeWords(char** words, int numWords) {
    if(words == NULL) {
        return;
    }
    for(int i = 0; i < numWords; i++) {
        free(words[i]);
    }
    free(words);
}

//---------------------- ^^^ END OF TASK 1 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 2 \/\/\/ -------------------

int strCmpCnt(char* str1, char* str2) {
    int count = 0;
    char* shortStr = (strlen(str1) < strlen(str2)) ? str1 : str2;
    char* longStr = (strlen(str1) >= strlen(str2)) ? str1 : str2;

    int i;
    for(i = 0; i < strlen(shortStr) && shortStr[i] != '\0'; i++) {
        if(shortStr[i] != longStr[i]) {
            count++;
        }
    }
    count += strlen(longStr) - strlen(shortStr);
    return count;
}



//---------------------- ^^^ END OF TASK 2 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 3 \/\/\/ -------------------

void insertWordAtFront(WordNode** ladder, char* newWord) {
    /* 
    Allocate space for a new WordNode, set its myWord subitem 
    using newWord and insert it to the front of ladder, which 
    is a pointer-passed-by-pointer since the head node of the 
    ladder changes inside this function;  newWord is a pointer 
    to a C-string from the full words array, which is already heap-allocated
    */
    if(ladder ==  NULL) {
        return;
    }

    WordNode *prevHead = *ladder;
    *ladder = (WordNode*)malloc(sizeof(WordNode));
    (*ladder)->myWord = newWord;
    (*ladder)->next = prevHead;
    return;

}

int getLadderHeight(WordNode* ladder) {
    int height = 0;
    if(ladder == NULL) {
        return 0;
    }
    while(ladder != NULL) {
        height++;
        ladder = ladder->next;

    }
    return height;
}

bool checkForValidWord(char** words, int numWords, int wordSize, WordNode* ladder, char* aWord) {
    /*  
    Copy-pasted directions from Programming Tasks:
    Write the checkForValidWord() function, which checks if a queried word is a valid 
    option for the next word in an incomplete word ladder. This is a Boolean function 
    that should return false if the word is invalid or return true if the word is valid.
    There are three ways in which a word can be invalid:
        (1) the queried word is invalid if it does not have the correct character count, 
        (2) the queried word is invalid if it is not in the dictionary, and
        (3) the queried word is invalid if it is not a single-character difference 
            (more than one character changed OR no character change) from the previous word in the ladder.
    This function should call findWord(), strCmpCnt(), & string.h function(s). 
    Note that many words may break more than one of these invalidity measures; 
    so, the order of the above list is the order in which the invalid checks must be done. 
    For example, if the queried word is both: 
        too long (1) and not a single-character difference from the top of the current ladder (3),
    then it would break on invalidity measure (1), since the length would be checked first. 
    There is one more special case that this function must handle: 
        the user can enter DONE at any point to stop the process of building the word ladder. 
    The primary application is written such that this function should return TRUE if the queried word is DONE.
    This must take precedence over all other validity/invalidity checks.
    */

    // Case 1: user entered "DONE"
    if(strcmp(aWord, "DONE") == 0) {
        printf("Stopping with an incomplete word ladder...\n");
        return true;
    }

    // Case 2: word is incorrect length
    if(strlen(aWord) != wordSize) {
        printf("Entered word does NOT have the correct length. Try again...\n");
        return false;
    }

    // Case 3: word is not in the dictionary
    if(findWord(words, aWord, 0, numWords-1) == -1) {
        printf("Entered word NOT in dictionary. Try again...\n");
        return false;
    }

    // Case 4: word is not a single character difference from previous word
    // from parameters it can be inferred that someLadder->topWord will be passed in as ladder
    if(strCmpCnt(ladder->myWord, aWord) != 1) {
        printf("Entered word is NOT a one-character change from the previous word. Try again...\n");
        return false;
    }

    printf("Entered word is valid and will be added to the word ladder.\n");
    return true;
}

bool isLadderComplete(WordNode* ladder, char* finalWord) {
    if(ladder == NULL || ladder->myWord == NULL || finalWord == NULL) {
        return false;
    }
    return (strcmp(ladder->myWord, finalWord) == 0);
}

WordNode* copyLadder(WordNode* ladder) {
    if(ladder == NULL) {
        return NULL;
    }
    WordNode* newHead = (WordNode*)malloc(sizeof(WordNode));
    WordNode* newLadder = newHead;
    while(ladder->next != NULL) {
        newLadder->myWord = ladder->myWord;
        newLadder->next = (WordNode*)malloc(sizeof(WordNode));
        newLadder = newLadder->next;
        ladder = ladder->next;
    }
    newLadder->myWord = ladder->myWord;
    newLadder->next = NULL;
    return newHead;
}

void freeLadder(WordNode* ladder) {
    if(ladder == NULL) {
        return;
    }
    WordNode* toFree;
    while(ladder != NULL) {
        toFree = ladder;
        ladder = ladder->next;
        free(toFree);
        toFree = NULL;
    }
}

//---------------------- ^^^ END OF TASK 3 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 4 \/\/\/ -------------------

void displayIncompleteLadder(WordNode* ladder) {
    //-------------------------------------------------------------------
// displayIncompleteLadder() should display the C-strings in the  
//      [ladderWords] array with the first word at the bottom, and 
//      each successive C-string one rung higher on the ladder. 
//      The ladder [height] is the number of words it contains.  
//      To signify the ladder as incomplete, display three lines of  
//      "..." at the top of the ladder. The ladder must be displayed 
//      with an indentation of two whitespaces on every line;
    if(ladder == NULL) {
        return;
    }
    for(int i = 0; i < 3; i++) {
        printf("  ...\n");
    }

    while(ladder->next != NULL && ladder->myWord != NULL) {
        printf("  %s\n", ladder->myWord);
        ladder = ladder->next;
    }
    if(ladder->myWord != NULL) {
        printf("  %s\n", ladder->myWord);
    }
    return;
}

void displayCompleteLadder(WordNode* ladder) {
//      Ex: if the start word is "data" and final word is "code" 
//          then the output display for a complete ladder should be  
//          as follows (where the quotation marks are NOT a part of  
//          the actual display):
//              "  code"
//              "    ^ "
//              "  cove"
//              "   ^  "
//              "  cave"
//              "  ^   "
//              "  gave"
//              "    ^ "
//              "  gate"
//              "  ^   "
//              "  date"
//              "     ^"
//              "  data" 
    if(ladder == NULL) {
        return;
    }

    int wordSize = strlen(ladder->myWord);
    while(ladder->next != NULL) {
        printf("  %s\n", ladder->myWord);
        int caretInd = strCmpInd(ladder->myWord, ladder->next->myWord);
        printf("  ");
        for(int i = 0; i < wordSize; i++) {
            if(i == caretInd) {
                printf("^");
                continue;
            }
            printf(" ");
        }
        printf("\n");
        ladder = ladder->next;
    }
    printf("  %s\n", ladder->myWord);
    
}


//---------------------- ^^^ END OF TASK 4 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 5 \/\/\/ -------------------

void insertLadderAtBack(LadderNode** list, WordNode* newLadder) {
    LadderNode* thisLadder = (LadderNode*)malloc(sizeof(LadderNode)); // allocates memory for new ladder node
    thisLadder->topWord = newLadder;
    thisLadder->next = NULL;
    // allocates memory for a new LadderNode and sets its topWord field to newLadder parameter
    if(*list == NULL) {
        // if list is empty, sets the head node to thisLadder
        *list = thisLadder;
        return;
    }
    // else, finds tail of list and sets tail->next to thisLadder
    LadderNode* ladder = *list;
    while(ladder->next != NULL) {
        ladder = ladder->next;
    }
    ladder->next = thisLadder;
}

WordNode* popLadderFromFront(LadderNode** list) {
    /*
    Pop the first ladder from the front of the list 
    by returning the pointer to the head node of the 
    ladder that is the subitem of the head node of 
    list AND updating the head node of list to the next 
    LadderNode. Note that list is a pointer-passed-by-pointer, 
    since this function updates the head node to be one down the list. 
    The LadderNode popped off the front must have its memory 
    freed in this function, since it will go out of scope, 
    but the ladder itself, i.e. the head WordNode, should NOT be freed. 
    */
    if(*list == NULL) {
        // if empty list, pop nothing
        return NULL;
    }
    LadderNode *popLadder = *list;
    WordNode *popNode = popLadder->topWord;
    (*list) = (*list)->next; // move head node back one
    free(popLadder); // free the ladderNode
    return popNode; // return the wordNode
}

void freeLadderList(LadderNode* myList) {
    /*
    Free up all heap-allocated space 
    for myList. For each ladder in myList,
    first free the space allocated for 
    each WordNode in the ladder using 
    freeLadder(), then, free the space 
    allocated for the LadderNode itself.
    */
    if(myList == NULL) {
        return;
    }
    LadderNode *temp = NULL;
    while(myList != NULL) {
        freeLadder(myList->topWord);
        myList->topWord = NULL;
        temp = myList;
        myList = myList->next;
        free(temp);
        temp = NULL;
    }

}

//---------------------- ^^^ END OF TASK 5 ^^^ ----------------------


//------------------- \/\/\/ TOP OF TASK 6 \/\/\/ -------------------


// WordNode* shortestRecursive(char **dict, int dictSize, , bool *used, int wordSize, LadderNode* thisLadder, char* finalWord) {

//     // base case: word is final
//     // first case: word in dictionary
//         // continue this three
//     // second case: word not in dictionary
//         // try new letter
//     /*
//     potential problem:
//         how will other search tree know if word has already been attempted in previous tree?
//         - each recursive call is a new ladder, so used array should reflect accurate cache
//     */
//     if(strcmp(thisLadder->topWord->myWord, finalWord) == 0) {
//         return thisLadder;
//     }

//     int dictInd = findWord(dict, thisLadder->topWord->myWord, 0, dictSize-1);
//     if(dictInd != -1 && !used[dictInd]) {
//         insertWordAtFront(thisLadder)
//     }
// }


WordNode* findShortestWordLadder(   char** words, 
                                    bool* usedWord, 
                                    int numWords, 
                                    int wordSize, 
                                    char* startWord, 
                                    char* finalWord ) {
/*
Implement the algorithm to find the shortest word ladder 
from startWord to finalWord in the words word array, with 
numWords total words where each word is wordSize long. 
Also, usedWord has size numWords, such that usedWord[i] is 
only true if words[i] has previously be entered into a ladder, 
and should therefore not be added to any other ladders. 
The algorithm creates partial word ladders, which are WordNode 
linked lists, and stores them in a LadderNode linked list. 
Return NULL if no ladder is possible; otherwise, return a pointer 
to the shortest ladder.Make sure to free all heap-allocated memory 
that is created in this function that is not used for the returned 
ladder before returning from this function.
*/

/*
To find the shortest word ladder between words w1 and w2:
   Create myList, an empty list of LadderNode structs
   Create myLadder, an empty list of WordNode structs
   Prepend w1 to the front of myLadder
   Append myLadder to the back of myList 
   While myList is not empty: 
      Pop the head LadderNode off the front of myList, call it myLadder
      For each word in the words array that is a neighbor of the head myWord of myLadder:
         If the neighbor word has not already been used in a ladder to this point:
            If the neighbor word is w2:
               Prepend w2 to the front of myLadder
               Hooray! We found the shortest word ladder, so return myLadder
            Otherwise:
               Copy myLadder to anotherLadder
               Prepend neighbor word to the front of anotherLadder
               Append anotherLadder to the back of myList 
   If no ladder was returned, then no ladder is possible
*/
    LadderNode *thisList = NULL;
    WordNode *thisLadder = NULL;
    insertWordAtFront(&thisLadder, startWord);
    insertLadderAtBack(&thisList, thisLadder);
    char tryWord[wordSize+1];
    
    while(thisList != NULL) {
        thisLadder = popLadderFromFront(&thisList); 
        int wordLen = strlen(thisLadder->myWord);

        for(int i = 0; i < wordLen; i++) {
            strcpy(tryWord, thisLadder->myWord); // copy current word on ladder into tryWord for modification
            for(char c = 'a'; c <= 'z'; c++) { // loop through the alphabet

                if(c == tryWord[i]) {
                    // if character is current character, then words are identical, no need to check
                    continue;
                }

                tryWord[i] = c; // modify single character

                int tryInd = findWord(words, tryWord, 0, numWords-1); // position of word in dictionary if found, -1 if not found

                if (tryInd >= 0 && !usedWord[tryInd]) { 
                    // is neighbor and not tried before
                    if(strcmp(tryWord, finalWord) == 0) {
                        // shortest ladder found
                        insertWordAtFront(&thisLadder, words[tryInd]);
                        freeLadderList(thisList);
                        return thisLadder;
                    }

                    WordNode *ladderCopy = copyLadder(thisLadder);
                    insertWordAtFront(&ladderCopy, words[tryInd]);
                    insertLadderAtBack(&thisList, ladderCopy); // store in list for next while loop iteration
                    usedWord[tryInd] = true; // no need to check this word again
                }
            }
        }
        freeLadder(thisLadder);
        thisLadder = NULL;
            
    }
    return NULL;
}

//---------------------- ^^^ END OF TASK 5 ^^^ ----------------------


//------------------- \/\/\/ TOP OF OTHERS \/\/\/ -------------------


// randomly set a word from the dictionary word array
void setWordRand(char** words, int numWords, int wordSize, char* aWord) {
    printf("  Picking a random word for you...\n");
    strcpy(aWord,words[rand()%numWords]);
    printf("  Your word is: %s\n",aWord);
}

// interactive user-input to set a word;
//  ensures the word is in the dictionary word array
void setWord(char** words, int numWords, int wordSize, char* aWord) {
    bool valid = false;
    if (strcmp(aWord,"RAND") != 0) printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
    int count = 0;
    while (!valid) {
        if (strcmp(aWord,"RAND") != 0) scanf("%s",aWord);
        count++;
        valid = (strlen(aWord) == wordSize);
        if (valid) {
            int wordInd = findWord(words, aWord, 0, numWords-1);
            if (wordInd < 0) {
                valid = false;
                printf("    Entered word %s is not in the dictionary.\n",aWord);
                printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
            }
        } else if (strcmp(aWord,"RAND") != 0) {
            printf("    Entered word %s is not a valid %d-letter word.\n",aWord,wordSize);
            printf("  Enter a %d-letter word (enter RAND for a random word): ", wordSize);
        }
        if (!valid && (count >= 5 || strcmp(aWord,"RAND") == 0)) { //too many tries, picking random word
            setWordRand(words, numWords, wordSize, aWord);
            valid = true;
        }
    }
}

// helpful debugging function to print a single Ladder
void printLadder(WordNode* ladder) {
    WordNode* currNode = ladder;
    while (currNode != NULL) {
        printf("\t\t\t%s\n",currNode->myWord);
        currNode = currNode->next;
    }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode* list) {
    printf("\n");
    printf("Printing the full list of ladders:\n");
    LadderNode* currList = list;
    while (currList != NULL) {
        printf("  Printing a ladder:\n");
        printLadder(currList->topWord);
        currList = currList->next;
    }
    printf("\n");
}

//---------------------- ^^^ END OF OTHERS ^^^ ----------------------



//-----------------------------------------------------
// The primary application is mostly fully-develop as
//  provided in main(); changes in main() should be
//  limited to updates made for the game play task(s)
//  and testing-related purposes (such as command-line
//  arguments for "TESTING MODE" to call a test case 
//  master function, or something similar)
//-----------------------------------------------------
int main(int argc, char* argv[]) {

    printf("\n");
    printf("--------------------------------------------\n");
    printf("Welcome to the CS 211 Word Ladder Generator!\n");
    printf("--------------------------------------------\n\n");
    

    //-------------- \/\/\/ TOP OF PROGRAM SETTINGS \/\/\/ --------------
    //--- COMMAND-LINE ARGUMENTS AND/OR INTERACTIVE USER-INPUT \/\/\/ ---

    
    // default values for program parameters that may be set with
    //  command-line arguments
    int wordSize = -2114430;
    char dict[100] = "notAfile";
    char startWord[30] = "notAword";
    char finalWord[30] = "notValid";
    bool playMode = false;
    
    printf("\nProcessing command-line arguments...\n");

    //-------------------------------------------------------------------
    // command-line arguments:
    //  [-n wordLen] = sets word length for word ladder;
    //                 if wordLen is not a valid input
    //                 (cannot be less than 2 or greater than 20),
    //                 or missing from command-line arguments,
    //                 then let user set it using interactive user input
    // [-d dictFile] = sets dictionary file;
    //                 if dictFile is invalid (file not found) or
    //                 missing from command-line arguments, then let
    //                 user set it using interactive user input
    // [-s startWord] = sets the starting word;
    //                  if startWord is invalid
    //                  (not in dictionary or incorrect length) or
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-f finalWord] = sets the final word;
    //                  if finalWord is invalid
    //                  (not in dictionary or incorrect length) or
    //                  missing from command-line arguments, then let
    //                  user set it using interactive user input
    // [-p playModeSwitch] = turns playMode ON if playModeSwitch is "ON"
    //                       or leaves playMode OFF if playModeSwitch is
    //                       anything else, including "OFF"
    //-------------------------------------------------------------------

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i],"-n") == 0 && argc > i+1) {
            wordSize = atoi(argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-d") == 0 && argc > i+1) {
            strcpy(dict, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-s") == 0 && argc > i+1) {
            strcpy(startWord, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-f") == 0 && argc > i+1) {
            strcpy(finalWord, argv[i+1]);
            ++i;
        } else if (strcmp(argv[i],"-p") == 0 && argc > i+1) {
            playMode = (strcmp(argv[i+1],"ON") == 0);
            ++i;
        }
    }
    
    srand((int)time(0));
    
    // set word length using interactive user-input
    //  if wordSize == -2114430, it was NOT set with command-line args
    while (wordSize < 2 || wordSize > 20) {
        if (wordSize != -2114430) printf("Invalid word size for the ladder: %d\n", wordSize);
        printf("Enter the word size for the ladder: ");
        scanf("%d",&wordSize);
        printf("\n");
    }

    printf("This program is a word ladder building game and a solver that\n");
    printf("finds the shortest possible ");
    printf("word ladder between two %d-letter words.\n\n",wordSize);
    
    // interactive user-input to set the dictionary file;
    //  check that file exists; if not, user enters another file
    //  if file exists, count #words of desired length [wordSize];
    //  if dict == "notAfile", it was NOT set with command-line args
    int numWords = countWordsOfLength(dict,wordSize);
    while (numWords < 0) {
        if (strcmp(dict, "notAfile") != 0) {
            printf("  Dictionary %s not found...\n",dict);
        }
        printf("Enter filename for dictionary: ");
        scanf("%s", dict);
        printf("\n");
        numWords = countWordsOfLength(dict,wordSize);
    }
    
    // end program if file does not have at least two words of desired length
    if (numWords < 2) {
        printf("  Dictionary %s contains insufficient %d-letter words...\n",dict,wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    
    // allocate heap memory for the word array; only words with desired length
    char** words = (char**)malloc(numWords*sizeof(char*));
    for (int i = 0; i < numWords; ++i) {
        words[i] = (char*)malloc((wordSize+1)*sizeof(char));
    }
    
    // [usedWord] bool array has same size as word array [words];
    //  all elements initialized to [false];
    //  later, usedWord[i] will be set to [true] whenever
    //      words[i] is added to ANY partial word ladder;
    //      before adding words[i] to another word ladder,
    //      check for previous usage with usedWord[i]
    bool* usedWord = (bool*)malloc(numWords*sizeof(bool));
    for (int i = 0; i < numWords; ++i) {
        usedWord[i] = false;
    }
    
    // build word array (only words with desired length) from dictionary file
    printf("Building array of %d-letter words... ", wordSize);
    bool status = buildWordArray(dict,words,numWords,wordSize);
    if (!status) {
        printf("  ERROR in building word array.\n");
        printf("  File not found or incorrect number of %d-letter words.\n",wordSize);
        printf("Terminating program...\n");
        return -1;
    }
    printf("Done!\n\n");

    // set the two ends of the word ladder using interactive user-input
    //  make sure start and final words are in the word array,
    //  have the correct length (implicit by checking word array), AND
    //  that the two words are not the same
    // start/final words may have already been set using command-line arguments
    // the start/final word can also be set to "RAND" resulting in a random
    //  assignment from any element of the words array
    if (strcmp(startWord,"RAND")==0) {
        printf("Setting the start word randomly...\n");
        setWordRand(words, numWords, wordSize, startWord);
    } else if (findWord(words, startWord,0, numWords-1) < 0 || strlen(startWord) != wordSize) {
        if (strcmp(startWord,"notAword")==0) {
            printf("Setting the start %d-letter word... \n", wordSize);
        } else {
            printf("Invalid start word %s. Resetting the start %d-letter word... \n", startWord, wordSize);
        }
        setWord(words, numWords, wordSize, startWord);
    }
    printf("\n");
    
    if (strcmp(finalWord,"RAND")==0) {
        printf("Setting the final word randomly...\n");
        setWordRand(words, numWords, wordSize, finalWord);
    } else if (findWord(words, finalWord,0, numWords-1) < 0 || strlen(finalWord) != wordSize) {
        if (strcmp(finalWord,"notValid")==0) {
            printf("Setting the final %d-letter word... \n", wordSize);
        } else {
            printf("Invalid final word %s. Resetting the final %d-letter word... \n", finalWord, wordSize);
        }
        setWord(words, numWords, wordSize, finalWord);
    }
    while (strcmp(finalWord,startWord) == 0) {
        printf("  The final word cannot be the same as the start word (%s).\n",startWord);
        printf("Setting the final %d-letter word... \n", wordSize);
        setWord(words, numWords, wordSize, finalWord);
    }
    printf("\n");
    
    //----------------- ^^^ END OF PROGRAM SETTINGS ^^^ -----------------
    
    
    //-------------- \/\/\/ TOP OF GAME PLAY SECTION \/\/\/ --------------
    
    if (!playMode) {
        printf("\n");
        printf("---------------------------------------------\n");
        printf("No Word Ladder Builder Game; Play Mode is OFF\n");
        printf("---------------------------------------------\n");
        printf("\n");
    } else {
        printf("\n");
        printf("-----------------------------------------------\n");
        printf("Welcome to the CS 211 Word Ladder Builder Game!\n");
        printf("-----------------------------------------------\n");
        printf("\n");

        printf("Your goal is to make a word ladder between two ");
        printf("%d-letter words: \n  %s -> %s\n\n",wordSize, startWord,finalWord);
        
        WordNode* userLadder = NULL;
        int ladderHeight = 0; // initially, the ladder is empty
        int startInd = findWord(words, startWord, 0, numWords-1);
        insertWordAtFront(&userLadder, words[startInd]);
        ladderHeight++; // Now, the ladder has a start word
            
        char aWord[30] = "XYZ";
        printf("\n");
        
        // Let the user build a word ladder interactively & iteratively.
        // First, check that ladder is not too long AND not complete.
        //-------------------------------------------------------------------
        // TODO - PART OF Task 4 (GAME PLAY): modify the while loop condition
        //          such that the word ladder building process continues only
        //          if BOTH of the following conditions are met:
        //              1. the user is NOT attempting to stop the word ladder
        //                 building process, which occurs if the entered word
        //                 [aWord] from the last iteration is "DONE"
        //              2. the ladder is still incomplete; i.e. the last word
        //                 added to ladder is not the final word;
        //                 note: this should use a call to isLadderComplete()
        //-------------------------------------------------------------------
        while (strcmp(aWord, "DONE") != 0 && !isLadderComplete(userLadder, finalWord)) {   // modify this line
            printf("The goal is to reach the final word: %s\n",finalWord);
            printf("The ladder is currently: \n");
            displayIncompleteLadder(userLadder);
            printf("Current ladder height: %d\n",ladderHeight);
            printf("Enter the next word (or DONE to stop): ");
            scanf("%s",aWord);
            printf("\n");
            
            // Make sure the entered word is valid for the next ladder rung;
            // if not, repeatedly allow user to enter another word until one is valid
            while (!checkForValidWord(words, numWords, wordSize, userLadder, aWord)) {
                printf("Enter another word (or DONE to stop): ");
                scanf("%s",aWord);
                printf("\n");
            }

            // add the entered word to the ladder (unless it is "DONE")
            if (strcmp(aWord,"DONE") != 0) {
                int currInd = findWord(words, aWord, 0, numWords-1);
                insertWordAtFront(&userLadder, words[currInd]);
                ladderHeight++;
            }
            printf("\n");
        }

        // Check if the built word ladder is complete and
        // display the word ladder appropriately.
        if (isLadderComplete(userLadder, finalWord)) {
            printf("Word Ladder complete!\n\n");
            displayCompleteLadder(userLadder);
            printf("\nWord Ladder height = %d\n\n", ladderHeight);
            printf("Can you find a shorter Word Ladder next time??? \n");
        } else {
            printf("The final Word Ladder is incomplete:\n");
            displayIncompleteLadder(userLadder);
            printf("Word Ladder height = %d\n\n", ladderHeight);
            printf("Can you complete the Word Ladder next time??? \n");
        }
        freeLadder(userLadder);
    }
    
    //----------------- ^^^ END OF GAME PLAY SECTION ^^^ -----------------
    
    
    //-------------- \/\/\/ TOP OF WORD LADDER SOLVER \/\/\/ --------------
    
    printf("\n\n");
    printf("-----------------------------------------\n");
    printf("Welcome to the CS 211 Word Ladder Solver!\n");
    printf("-----------------------------------------\n");
    printf("\n");
    

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // OPTIONAL EXTENTION TO FIND LONGEST WORD LADDER:
    //  program must end with finding the shortest word ladder
    //  (& the associated print statements); if you choose to
    //  extend your program to find the longest word ladder,
    //  put the long word ladder algorithm (& the associated
    //  print statements) BEFORE the short word ladder algorithm
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    
    // run the algorithm to find the shortest word ladder
    WordNode* myLadder = findShortestWordLadder(words, usedWord, numWords, wordSize, startWord, finalWord);

    // display word ladder and its height if one was found
    if (myLadder == NULL) {
        printf("There is no possible word ladder from %s to %s\n",startWord,finalWord);
    } else {
        printf("Shortest Word Ladder found!\n\n");
        displayCompleteLadder(myLadder);
        //printLadder(myLadder);
    }
    printf("\nWord Ladder height = %d\n\n",getLadderHeight(myLadder));

    //----------------- ^^^ END OF WORD LADDER SOLVER ^^^ -----------------
    
    
    //-------------- \/\/\/ TOP OF CLEAN-UP \/\/\/ --------------
    
    // TODO - Part of ALL Tasks:
    //      free all heap-allocated memory to avoid potential
    //      memory leaks. Since the word length for the word
    //      ladder is variable (i.e. set by a command-line
    //      argument or interactive user-input) any array
    //      whose size depends on the word length should be
    //      dynamically heap-allocated, and thus, must be
    //      tracked and freed before program termination.
    //      A big part of the memory management & freeing
    //      is handled by the following functions, but
    //      you may have introduced additional heap-memory
    //      allocations, especially as part of the game play.
    
    // free the heap-allocated memory for the shortest ladder
    freeLadder(myLadder);
    // free the heap-allocated memory for the words array
    freeWords(words,numWords);
    free(usedWord);
    
    //----------------- ^^^ END OF CLEAN-UP ^^^ -----------------

    
    return 0;
}
