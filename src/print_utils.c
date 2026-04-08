#include "print_utils.h"
#include "helpers.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

void printLadder(WordNode *ladder) {
  WordNode *currNode = ladder;
  while (currNode != NULL) {
    printf("\t\t\t%s\n", currNode->myWord);
    currNode = currNode->next;
  }
}

// helpful debugging function to print the entire list of Ladders
void printList(LadderNode *list) {
  printf("\n");
  printf("Printing the full list of ladders:\n");
  LadderNode *currList = list;
  while (currList != NULL) {
    printf("  Printing a ladder:\n");
    printLadder(currList->topWord);
    currList = currList->next;
  }
  printf("\n");
}

void displayCompleteLadder(WordNode *ladder) {
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
  if (ladder == NULL) {
    return;
  }

  int wordSize = strlen(ladder->myWord);
  while (ladder->next != NULL) {
    printf("  %s\n", ladder->myWord);
    int caretInd = strCmpInd(ladder->myWord, ladder->next->myWord);
    printf("  ");
    for (int i = 0; i < wordSize; i++) {
      if (i == caretInd) {
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

void displayIncompleteLadder(WordNode *ladder) {
  //-------------------------------------------------------------------
  // displayIncompleteLadder() should display the C-strings in the
  //      [ladderWords] array with the first word at the bottom, and
  //      each successive C-string one rung higher on the ladder.
  //      The ladder [height] is the number of words it contains.
  //      To signify the ladder as incomplete, display three lines of
  //      "..." at the top of the ladder. The ladder must be displayed
  //      with an indentation of two whitespaces on every line;
  if (ladder == NULL) {
    return;
  }
  for (int i = 0; i < 3; i++) {
    printf("  ...\n");
  }

  while (ladder->next != NULL && ladder->myWord != NULL) {
    printf("  %s\n", ladder->myWord);
    ladder = ladder->next;
  }
  if (ladder->myWord != NULL) {
    printf("  %s\n", ladder->myWord);
  }
  return;
}