/** const.h *****************************************************************
 * 27/01/2013               CWG - Crossword generator                       *
 * Content of this file:                                                    *
 *   Definition of constants that can be altered to change the behaviour of *
 *   the crossword generator. In particular the maximum length of a word,   *
 *   and the maximum length of the clue associated to a word can be changed *
 *   here.                                                                  *
 * Author: Rodolphe Lepigre <rlepigre@gmail.com>                            *
 ****************************************************************************/
#ifndef __CONST_H__
#define __CONST_H__

// Words, clues
#define MAX_WORD_SIZE 14   // Max size of a word.
#define MIN_WORD_SIZE 1    // Min size of a word.
#define MAX_DEF_SIZE 200   // Max size of the clue.
#define MIN_DEF_SIZE 1     // Min size of the clue.
#define INIT_NB 8          // Size of the word buffer at first (8 is fine).

// Grid
#define DEFAULT_WIDTH 15   // Default width of a crossword.
#define DEFAULT_HEIGHT 15  // Default height of a crossword.
#define MIN_SIZE 6         // Min width / height of a crossword.
#define MAX_SIZE 30        // Max width / height of a crossword.
#define EMPTY_CHAR ' '     // Empty char in the grid in text-mode.
#define SPACE_CHAR '_'     // Space character to be used in word list..

// Algorithm
#define NB_LOOP 20         // Number of loops for looking for words to place.
#define NB_RAND_CH 12      // Maximum number of random choice for insertion.

#endif
