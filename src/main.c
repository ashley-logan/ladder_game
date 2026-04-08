#include "helpers.h"
#include "print_utils.h"
#include "word_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]) {

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
    if (strcmp(argv[i], "-n") == 0 && argc > i + 1) {
      wordSize = atoi(argv[i + 1]);
      ++i;
    } else if (strcmp(argv[i], "-d") == 0 && argc > i + 1) {
      strcpy(dict, argv[i + 1]);
      ++i;
    } else if (strcmp(argv[i], "-s") == 0 && argc > i + 1) {
      strcpy(startWord, argv[i + 1]);
      ++i;
    } else if (strcmp(argv[i], "-f") == 0 && argc > i + 1) {
      strcpy(finalWord, argv[i + 1]);
      ++i;
    } else if (strcmp(argv[i], "-p") == 0 && argc > i + 1) {
      playMode = (strcmp(argv[i + 1], "ON") == 0);
      ++i;
    }
  }

  srand((int)time(0));

  // set word length using interactive user-input
  //  if wordSize == -2114430, it was NOT set with command-line args
  while (wordSize < 2 || wordSize > 20) {
    if (wordSize != -2114430)
      printf("Invalid word size for the ladder: %d\n", wordSize);
    printf("Enter the word size for the ladder: ");
    scanf("%d", &wordSize);
    printf("\n");
  }

  printf("This program is a word ladder building game and a solver that\n");
  printf("finds the shortest possible ");
  printf("word ladder between two %d-letter words.\n\n", wordSize);

  // interactive user-input to set the dictionary file;
  //  check that file exists; if not, user enters another file
  //  if file exists, count #words of desired length [wordSize];
  //  if dict == "notAfile", it was NOT set with command-line args
  int numWords = countWordsOfLength(dict, wordSize);
  while (numWords < 0) {
    if (strcmp(dict, "notAfile") != 0) {
      printf("  Dictionary %s not found...\n", dict);
    }
    printf("Enter filename for dictionary: ");
    scanf("%s", dict);
    printf("\n");
    numWords = countWordsOfLength(dict, wordSize);
  }

  // end program if file does not have at least two words of desired length
  if (numWords < 2) {
    printf("  Dictionary %s contains insufficient %d-letter words...\n", dict,
           wordSize);
    printf("Terminating program...\n");
    return -1;
  }

  // allocate heap memory for the word array; only words with desired length
  char **words = (char **)malloc(numWords * sizeof(char *));
  for (int i = 0; i < numWords; ++i) {
    words[i] = (char *)malloc((wordSize + 1) * sizeof(char));
  }

  // [usedWord] bool array has same size as word array [words];
  //  all elements initialized to [false];
  //  later, usedWord[i] will be set to [true] whenever
  //      words[i] is added to ANY partial word ladder;
  //      before adding words[i] to another word ladder,
  //      check for previous usage with usedWord[i]
  bool *usedWord = (bool *)malloc(numWords * sizeof(bool));
  for (int i = 0; i < numWords; ++i) {
    usedWord[i] = false;
  }

  // build word array (only words with desired length) from dictionary file
  printf("Building array of %d-letter words... ", wordSize);
  bool status = buildWordArray(dict, words, numWords, wordSize);
  if (!status) {
    printf("  ERROR in building word array.\n");
    printf("  File not found or incorrect number of %d-letter words.\n",
           wordSize);
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
  if (strcmp(startWord, "RAND") == 0) {
    printf("Setting the start word randomly...\n");
    setWordRand(words, numWords, wordSize, startWord);
  } else if (findWord(words, startWord, 0, numWords - 1) < 0 ||
             strlen(startWord) != wordSize) {
    if (strcmp(startWord, "notAword") == 0) {
      printf("Setting the start %d-letter word... \n", wordSize);
    } else {
      printf("Invalid start word %s. Resetting the start %d-letter word... \n",
             startWord, wordSize);
    }
    setWord(words, numWords, wordSize, startWord);
  }
  printf("\n");

  if (strcmp(finalWord, "RAND") == 0) {
    printf("Setting the final word randomly...\n");
    setWordRand(words, numWords, wordSize, finalWord);
  } else if (findWord(words, finalWord, 0, numWords - 1) < 0 ||
             strlen(finalWord) != wordSize) {
    if (strcmp(finalWord, "notValid") == 0) {
      printf("Setting the final %d-letter word... \n", wordSize);
    } else {
      printf("Invalid final word %s. Resetting the final %d-letter word... \n",
             finalWord, wordSize);
    }
    setWord(words, numWords, wordSize, finalWord);
  }
  while (strcmp(finalWord, startWord) == 0) {
    printf("  The final word cannot be the same as the start word (%s).\n",
           startWord);
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
    printf("%d-letter words: \n  %s -> %s\n\n", wordSize, startWord, finalWord);

    WordNode *userLadder = NULL;
    int ladderHeight = 0; // initially, the ladder is empty
    int startInd = findWord(words, startWord, 0, numWords - 1);
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
    while (strcmp(aWord, "DONE") != 0 &&
           !isLadderComplete(userLadder, finalWord)) { // modify this line
      printf("The goal is to reach the final word: %s\n", finalWord);
      printf("The ladder is currently: \n");
      displayIncompleteLadder(userLadder);
      printf("Current ladder height: %d\n", ladderHeight);
      printf("Enter the next word (or DONE to stop): ");
      scanf("%s", aWord);
      printf("\n");

      // Make sure the entered word is valid for the next ladder rung;
      // if not, repeatedly allow user to enter another word until one is valid
      while (!checkForValidWord(words, numWords, wordSize, userLadder, aWord)) {
        printf("Enter another word (or DONE to stop): ");
        scanf("%s", aWord);
        printf("\n");
      }

      // add the entered word to the ladder (unless it is "DONE")
      if (strcmp(aWord, "DONE") != 0) {
        int currInd = findWord(words, aWord, 0, numWords - 1);
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
  WordNode *myLadder = findShortestWordLadder(words, usedWord, numWords,
                                              wordSize, startWord, finalWord);

  // display word ladder and its height if one was found
  if (myLadder == NULL) {
    printf("There is no possible word ladder from %s to %s\n", startWord,
           finalWord);
  } else {
    printf("Shortest Word Ladder found!\n\n");
    displayCompleteLadder(myLadder);
    // printLadder(myLadder);
  }
  printf("\nWord Ladder height = %d\n\n", getLadderHeight(myLadder));

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
  freeWords(words, numWords);
  free(usedWord);

  //----------------- ^^^ END OF CLEAN-UP ^^^ -----------------

  return 0;
}