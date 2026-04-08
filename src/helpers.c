#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countWordsOfLength(char *filename, int wordSize) {
  char dict[100];
  snprintf(dict, sizeof(dict), "dictionaries/%s", filename);
  FILE *f = fopen(dict, "r");
  if (f == NULL) {
    return -1;
  }
  int count = 0;
  char currWord[30];
  while (fscanf(f, "%s", currWord) > 0) {
    count += (strlen(currWord) == wordSize);
  }
  fclose(f);
  return count;
}

bool buildWordArray(char *filename, char **words, int numWords, int wordSize) {
  char dict[100];
  snprintf(dict, sizeof(dict), "dictionaries/%s", filename);
  FILE *f = fopen(dict, "r");
  if (f == NULL) {
    return false;
  }
  char currWord[30];

  unsigned int i = 0;
  while (fscanf(f, "%s\n", currWord) > 0) {
    if (strlen(currWord) == wordSize) {
      strcpy(words[i], currWord);
      i++;
    }
  }
  fclose(f);
  return (i == numWords);
}

int strCmpInd(char *str1, char *str2) {
  char *shortStr = (strlen(str1) < strlen(str2)) ? str1 : str2;
  char *longStr = (strlen(str1) >= strlen(str2)) ? str1 : str2;
  int i;
  for (i = 0; shortStr[i] != '\0'; i++) {
    if (shortStr[i] != longStr[i]) {
      return i;
    }
  }
  return (longStr[i] == '\0') ? -1 : i;
}

int findWord(char **words, char *aWord, int loInd, int hiInd) {
  if (loInd > hiInd) {
    return -1;
  }
  int midInd = (loInd + hiInd) / 2;

  if (strcmp(words[midInd], aWord) == 0) {
    return midInd;
  }

  int diffInd = strCmpInd(words[midInd], aWord);

  if ((words[midInd])[diffInd] > aWord[diffInd]) {
    // too high, search bottom of current list
    return findWord(words, aWord, loInd, midInd - 1);
  } else {
    return findWord(words, aWord, midInd + 1, hiInd);
  }
}

void freeWords(char **words, int numWords) {
  if (words == NULL) {
    return;
  }
  for (int i = 0; i < numWords; i++) {
    free(words[i]);
  }
  free(words);
}

int strCmpCnt(char *str1, char *str2) {
  int count = 0;
  char *shortStr = (strlen(str1) < strlen(str2)) ? str1 : str2;
  char *longStr = (strlen(str1) >= strlen(str2)) ? str1 : str2;

  int i;
  for (i = 0; i < strlen(shortStr) && shortStr[i] != '\0'; i++) {
    if (shortStr[i] != longStr[i]) {
      count++;
    }
  }
  count += strlen(longStr) - strlen(shortStr);
  return count;
}

void setWordRand(char **words, int numWords, int wordSize, char *aWord) {
  printf("  Picking a random word for you...\n");
  strcpy(aWord, words[rand() % numWords]);
  printf("  Your word is: %s\n", aWord);
}

void setWord(char **words, int numWords, int wordSize, char *aWord) {
  bool valid = false;
  if (strcmp(aWord, "RAND") != 0)
    printf("  Enter a %d-letter word (enter RAND for a random word): ",
           wordSize);
  int count = 0;
  while (!valid) {
    if (strcmp(aWord, "RAND") != 0)
      scanf("%s", aWord);
    count++;
    valid = (strlen(aWord) == wordSize);
    if (valid) {
      int wordInd = findWord(words, aWord, 0, numWords - 1);
      if (wordInd < 0) {
        valid = false;
        printf("    Entered word %s is not in the dictionary.\n", aWord);
        printf("  Enter a %d-letter word (enter RAND for a random word): ",
               wordSize);
      }
    } else if (strcmp(aWord, "RAND") != 0) {
      printf("    Entered word %s is not a valid %d-letter word.\n", aWord,
             wordSize);
      printf("  Enter a %d-letter word (enter RAND for a random word): ",
             wordSize);
    }
    if (!valid &&
        (count >= 5 ||
         strcmp(aWord, "RAND") == 0)) { // too many tries, picking random word
      setWordRand(words, numWords, wordSize, aWord);
      valid = true;
    }
  }
}