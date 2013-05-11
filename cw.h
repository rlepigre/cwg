/** Copyright (c) 2012-2013 Rodolphe Lepigre ********************************
 * This file is part of CWG.                                                *
 *                                                                          *
 * CWG is free software: you can redistribute it and/or modify it under the *
 * terms of the GNU General Public License as published by the Free Softwa- *
 * re Foundation, either version 3 of the License, or (at your option) any  *
 * later version.                                                           *
 *                                                                          *
 * This program is distributed in the hope that it will be useful, but      *
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABI-  *
 * LITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public Li- *
 * cense for more details.                                                  *
 *                                                                          *
 * You should have received a copy of the GNU General Public License along  *
 * with this program. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                          *
 ** cw.h ********************************************************************
 * 31/01/2013               CWG - Crossword generator                       *
 * Content of this file:                                                    *
 *   Definition of the crossword structure, initialization and memory free  *
 *   of a crossword.                                                        *
 *   The core function of the simulator is also defined here (the function  *
 *   to compute a crossword).                                               *
 * Author: Rodolphe Lepigre <rlepigre@gmail.com>                            *
 ****************************************************************************/
#ifndef __CW_H__
#define __CW_H__

#include "const.h"
#include "word.h"

/*
 * Constants for defining the orientation of a word in the grid.
 */
#define VERTICAL 0
#define HORIZONTAL 1

/*
 * Structure representing a word placed in the grid.
 */
typedef struct __placed_word__{
  word w;                // The word with its clue.
  int posx;              // X coordinate in the grid.
  int posy;              // Y coordinate in the grid.
  int orient;            // Orientation.
  int label;             // Label.
}placed_word;

/*
 * Stucture representing a crossword.
 */
typedef struct __crossword__{
  int width;             // Width of the grid.
  int height;            // Height of the grid.
  char** grid;           // The grid.
  placed_word* words;    // The words in the grid.
  int nb_words;          // Number of words in the grid.
  int size_wlist;        // Number of words that can be stored in the list.
}*crossword;

/*
 * Initialize a crossword.
 * cw : the crossword.
 * w  : the width of the crossword.
 * h  : the height of the crossword.
 * Returns 0 if all went well, -1 otherwise.
 */
int cw_init(crossword* cw, int w, int h);

/*
 * Free the memory allocated to a crossword.
 * cw : the crossword.
 */
void cw_free(crossword cw);

/*
 * Compute a crossword.
 * cw : the crossword. Must be initialized.
 * wl : the list of words.
 * Returns 0 if all went well. -1 otherwise.
 */
int cw_compute(crossword cw, word_list wl);

#endif
