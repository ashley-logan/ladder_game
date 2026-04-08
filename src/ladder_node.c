#include "ladder_node.h"
#include "helpers.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void insertLadderAtBack(LadderNode **list, WordNode *newLadder) {
  LadderNode *thisLadder = (LadderNode *)malloc(
      sizeof(LadderNode)); // allocates memory for new ladder node
  thisLadder->topWord = newLadder;
  thisLadder->next = NULL;
  // allocates memory for a new LadderNode and sets its topWord field to
  // newLadder parameter
  if (*list == NULL) {
    // if list is empty, sets the head node to thisLadder
    *list = thisLadder;
    return;
  }
  // else, finds tail of list and sets tail->next to thisLadder
  LadderNode *ladder = *list;
  while (ladder->next != NULL) {
    ladder = ladder->next;
  }
  ladder->next = thisLadder;
}

WordNode *popLadderFromFront(LadderNode **list) {
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
  if (*list == NULL) {
    // if empty list, pop nothing
    return NULL;
  }
  LadderNode *popLadder = *list;
  WordNode *popNode = popLadder->topWord;
  (*list) = (*list)->next; // move head node back one
  free(popLadder);         // free the ladderNode
  return popNode;          // return the wordNode
}

void freeLadderList(LadderNode *myList) {
  /*
  Free up all heap-allocated space
  for myList. For each ladder in myList,
  first free the space allocated for
  each WordNode in the ladder using
  freeLadder(), then, free the space
  allocated for the LadderNode itself.
  */
  if (myList == NULL) {
    return;
  }
  LadderNode *temp = NULL;
  while (myList != NULL) {
    freeLadder(myList->topWord);
    myList->topWord = NULL;
    temp = myList;
    myList = myList->next;
    free(temp);
    temp = NULL;
  }
}

WordNode *copyLadder(WordNode *ladder) {
  if (ladder == NULL) {
    return NULL;
  }
  WordNode *newHead = (WordNode *)malloc(sizeof(WordNode));
  WordNode *newLadder = newHead;
  while (ladder->next != NULL) {
    newLadder->myWord = ladder->myWord;
    newLadder->next = (WordNode *)malloc(sizeof(WordNode));
    newLadder = newLadder->next;
    ladder = ladder->next;
  }
  newLadder->myWord = ladder->myWord;
  newLadder->next = NULL;
  return newHead;
}

WordNode *findShortestWordLadder(char **words, bool *usedWord, int numWords,
                                 int wordSize, char *startWord,
                                 char *finalWord) {
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
        For each word in the words array that is a neighbor of the head myWord
  of myLadder: If the neighbor word has not already been used in a ladder to
  this point: If the neighbor word is w2: Prepend w2 to the front of myLadder
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
  char tryWord[wordSize + 1];

  while (thisList != NULL) {
    thisLadder = popLadderFromFront(&thisList);
    int wordLen = strlen(thisLadder->myWord);

    for (int i = 0; i < wordLen; i++) {
      strcpy(tryWord, thisLadder->myWord); // copy current word on ladder into
                                           // tryWord for modification
      for (char c = 'a'; c <= 'z'; c++) {  // loop through the alphabet

        if (c == tryWord[i]) {
          // if character is current character, then words are identical, no
          // need to check
          continue;
        }

        tryWord[i] = c; // modify single character

        int tryInd = findWord(
            words, tryWord, 0,
            numWords -
                1); // position of word in dictionary if found, -1 if not found

        if (tryInd >= 0 && !usedWord[tryInd]) {
          // is neighbor and not tried before
          if (strcmp(tryWord, finalWord) == 0) {
            // shortest ladder found
            insertWordAtFront(&thisLadder, words[tryInd]);
            freeLadderList(thisList);
            return thisLadder;
          }

          WordNode *ladderCopy = copyLadder(thisLadder);
          insertWordAtFront(&ladderCopy, words[tryInd]);
          insertLadderAtBack(
              &thisList,
              ladderCopy); // store in list for next while loop iteration
          usedWord[tryInd] = true; // no need to check this word again
        }
      }
    }
    freeLadder(thisLadder);
    thisLadder = NULL;
  }
  return NULL;
}