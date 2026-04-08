#ifndef LADDERNODE_H
#define LADDERNODE_H
#include "word_node.h"

typedef struct LadderNode_struct {
  WordNode *topWord;
  struct LadderNode_struct *next;
} LadderNode;

void insertLadderAtBack(LadderNode **list, WordNode *newLadder);
WordNode *popLadderFromFront(LadderNode **list);
void freeLadderList(LadderNode *myList);
WordNode *copyLadder(WordNode *ladder);
WordNode *findShortestWordLadder(char **words, bool *usedWord, int numWords,
                                 int wordSize, char *startWord,
                                 char *finalWord);

#endif
