
#ifndef WORDNODE_H
#define WORDNODE_H

#include <stdbool.h>

typedef struct WordNode_struct {
  char *myWord;
  struct WordNode_struct *next;
} WordNode;

void insertWordAtFront(WordNode **ladder, char *newWord);
int getLadderHeight(WordNode *ladder);
bool isLadderComplete(WordNode *ladder, char *finalWord);
bool checkForValidWord(char **words, int numWords, int wordSize,
                       WordNode *ladder, char *aWord);
void freeLadder(WordNode *ladder);
#endif
