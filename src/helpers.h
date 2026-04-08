#ifndef HELPERS_H
#define HELPERS_H
#include <stdbool.h>

int countWordsOfLength(char *filename, int wordSize);
bool buildWordArray(char *filename, char **words, int numWords, int wordSize);
int strCmpInd(char *str1, char *str2);
int findWord(char **words, char *aWord, int loInd, int hiInd);
void freeWords(char **words, int numWords);
int strCmpCnt(char *str1, char *str2);
void setWordRand(char **words, int numWords, int wordSize, char *aWord);
void setWord(char **words, int numWords, int wordSize, char *aWord);

#endif