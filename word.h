/** word.h ******************************************************************
 * 27/01/2013               CWG - Crossword generator                       *
 * Content of this file:                                                    *
 *   Definition of a word, and a list of word. Initialization function and  *
 *   freeing function.                                                      *
 *   Definition of the functions for reading words from an input file. The  *
 *   format of the file is given bellow.                                    *
 * Format of input file:                                                    *
 *   WORD : Definition of the ord.                                          *
 * Author: Rodolphe Lepigre <rlepigre@gmail.com>                            *
 ****************************************************************************/
#ifndef __WORD_H__
#define __WORD_H__

#include "const.h"

/*
 * Structure representing a word.
 */
typedef struct __word__{
  char w[MAX_WORD_SIZE + 1]; // The word.
  char d[MAX_DEF_SIZE + 1];  // The clue.
  int used;              // Set to 1 if used in the crossword. 0 otherwise.
}word;

/*
 * Structure representing a list of words.
 */
typedef struct __word_list__{
  word* l;               // The actual list.
  int next_free;         // The next index where to write a word.
  size_t size;           // The capacity of the list.
}*word_list;

/*
 * Function that scanning a file for words.
 * input : the file.
 * list  : the list of words scanned.
 * verbose : if set to 1 verbose mode. O otherwise.
 * Failures are handled internally.
 */
void fscan_words(FILE* input, word_list* list, int verbose);

/*
 * Free the memory inside a word_list.
 * wl : the word list.
 */
void free_words(word_list wl);

#endif
