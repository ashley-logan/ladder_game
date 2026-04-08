#include "word_node.h"
#include "helpers.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertWordAtFront(WordNode **ladder, char *newWord) {
  /*
  Allocate space for a new WordNode, set its myWord subitem
  using newWord and insert it to the front of ladder, which
  is a pointer-passed-by-pointer since the head node of the
  ladder changes inside this function;  newWord is a pointer
  to a C-string from the full words array, which is already heap-allocated
  */
  if (ladder == NULL) {
    return;
  }

  WordNode *prevHead = *ladder;
  *ladder = (WordNode *)malloc(sizeof(WordNode));
  (*ladder)->myWord = newWord;
  (*ladder)->next = prevHead;
  return;
}

int getLadderHeight(WordNode *ladder) {
  int height = 0;
  if (ladder == NULL) {
    return 0;
  }
  while (ladder != NULL) {
    height++;
    ladder = ladder->next;
  }
  return height;
}

bool checkForValidWord(char **words, int numWords, int wordSize,
                       WordNode *ladder, char *aWord) {
  /*
  Copy-pasted directions from Programming Tasks:
  Write the checkForValidWord() function, which checks if a queried word is a
  valid option for the next word in an incomplete word ladder. This is a Boolean
  function that should return false if the word is invalid or return true if the
  word is valid. There are three ways in which a word can be invalid: (1) the
  queried word is invalid if it does not have the correct character count, (2)
  the queried word is invalid if it is not in the dictionary, and (3) the
  queried word is invalid if it is not a single-character difference (more than
  one character changed OR no character change) from the previous word in the
  ladder. This function should call findWord(), strCmpCnt(), & string.h
  function(s). Note that many words may break more than one of these invalidity
  measures; so, the order of the above list is the order in which the invalid
  checks must be done. For example, if the queried word is both: too long (1)
  and not a single-character difference from the top of the current ladder (3),
  then it would break on invalidity measure (1), since the length would be
  checked first. There is one more special case that this function must handle:
      the user can enter DONE at any point to stop the process of building the
  word ladder. The primary application is written such that this function should
  return TRUE if the queried word is DONE. This must take precedence over all
  other validity/invalidity checks.
  */

  // Case 1: user entered "DONE"
  if (strcmp(aWord, "DONE") == 0) {
    printf("Stopping with an incomplete word ladder...\n");
    return true;
  }

  // Case 2: word is incorrect length
  if (strlen(aWord) != wordSize) {
    printf("Entered word does NOT have the correct length. Try again...\n");
    return false;
  }

  // Case 3: word is not in the dictionary
  if (findWord(words, aWord, 0, numWords - 1) == -1) {
    printf("Entered word NOT in dictionary. Try again...\n");
    return false;
  }

  // Case 4: word is not a single character difference from previous word
  // from parameters it can be inferred that someLadder->topWord will be passed
  // in as ladder
  if (strCmpCnt(ladder->myWord, aWord) != 1) {
    printf("Entered word is NOT a one-character change from the previous word. "
           "Try again...\n");
    return false;
  }

  printf("Entered word is valid and will be added to the word ladder.\n");
  return true;
}

bool isLadderComplete(WordNode *ladder, char *finalWord) {
  if (ladder == NULL || ladder->myWord == NULL || finalWord == NULL) {
    return false;
  }
  return (strcmp(ladder->myWord, finalWord) == 0);
}

void freeLadder(WordNode *ladder) {
  if (ladder == NULL) {
    return;
  }
  WordNode *toFree;
  while (ladder != NULL) {
    toFree = ladder;
    ladder = ladder->next;
    free(toFree);
    toFree = NULL;
  }
}
