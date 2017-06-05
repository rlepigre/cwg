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
 ** cw.c ********************************************************************
 * 31/01/2013               CWG - Crossword generator                       *
 * Content of this file:                                                    *
 *   Implementation of the functions defined in "cw.h".                     *
 * Author: Rodolphe Lepigre <rlepigre@gmail.com>                            *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cw.h"
#include "const.h"

/*
 * Initialize a crossword.
 * cw : the crossword.
 * w  : the width of the crossword.
 * h  : the height of the crossword.
 * t  : the title of the crossword.
 * Returns 0 if all went well, -1 otherwise.
 */
int cw_init(crossword* cw, int w, int h, char* t){
  int i, j;

  if(NULL == ((*cw) = malloc(sizeof(struct __crossword__)))){
    fprintf(stderr, "Malloc error in cw_init !\n");
    return -1;
  }

  (*cw)->width = w;
  (*cw)->height = h;

  if (NULL == ((*cw)->title = malloc(strlen(t) * sizeof(char)))) {
    fprintf(stderr, "Malloc error in cw_init !\n");
    return -1;
  }
  
  strcpy((*cw)->title, t);
  
  if(NULL == ((*cw)->grid = malloc((*cw)->width * sizeof(char*)))){
    fprintf(stderr, "Malloc error in cw_init !\n");
    return -1;
  }
  for(i = 0; i < (*cw)->width; i++){
    if(NULL == ((*cw)->grid[i] = malloc((*cw)->height * sizeof(char)))){
      fprintf(stderr, "Malloc error in cw_init !\n");
      return -1;
    }
  }
  for(i = 0; i < (*cw)->width; i++)
    for(j = 0; j < (*cw)->height; j++)
      (*cw)->grid[i][j] = EMPTY_CHAR;
  if(NULL == ((*cw)->words = malloc(INIT_NB * sizeof(placed_word)))){
    fprintf(stderr, "Malloc error in cw_init !\n");
    return -1;
  }
  (*cw)->nb_words = 0;
  (*cw)->size_wlist = INIT_NB;
  return 0;
}

/*
 * Free the memory allocated to a crossword.
 * cw : the crossword.
 */
void cw_free(crossword cw){
  int i;
  for(i = 0; i < cw->width; i++) free(cw->grid[i]);
  free(cw->grid);
  free(cw->words);
  free(cw->title);
  free(cw);
}

/*
 * Add a word to the crossword.
 * cw : the crossword.
 * wl : the list of words.
 * i  : the index of the word in wl.
 * x  : x coordinate where to insert.
 * y  : y coordinate where to insert.
 * or : orietation of the new word.
 * l  : label.
 * Return 0 if all went well. -1 otherwise.
 */
int cw_add_word(crossword cw, word_list wl,
                int i, int x, int y, int or, int* l){
  int j, k;
  placed_word* wltmp = NULL;

  // The word is now used.
  cw->words[cw->nb_words].w = wl->l[i];
  wl->l[i].used = 1;

  // Orientation and position.
  cw->words[cw->nb_words].orient = or;
  cw->words[cw->nb_words].posx = x;
  cw->words[cw->nb_words].posy = y;

  // Label. Chech first if there is not a label already ar the position.
  k = -1;
  for(j = 0; j < cw->nb_words; j++)
    if((cw->words[j].posx == x) && (cw->words[j].posy == y))
      k = j;
  if(-1 == k){
    cw->words[cw->nb_words].label = *l;
    (*l)++;
  }else{
    cw->words[cw->nb_words].label = cw->words[k].label;
  }
  
  // Add the word to the grid. 
  if(or == HORIZONTAL){
    for(k = x; k < (int) (x + strlen(wl->l[i].w)); k++)
      cw->grid[k][y] = wl->l[i].w[k - x];
  }else{
    for(k = y; k < (int) (y + strlen(wl->l[i].w)); k++)
       cw->grid[x][k] = wl->l[i].w[k - y];
  }

  // A new word has bee added.
  cw->nb_words++;

  // We verify that there is enough space in the list.
  if(cw->nb_words >= cw->size_wlist){
    cw->size_wlist *= 2;
    wltmp = realloc(cw->words, cw->size_wlist * sizeof(placed_word));
    if(NULL == wltmp){
      fprintf(stderr, "Realloc error in cw_compute !\n");
      return -1;
    }
    cw->words = wltmp;
  }

  return 0;
}

/*
 * Tells weather a word can be added at a specific position in the crossword.
 * cw : the crossword.
 * wl : the list of words.
 * i  : the index of the word in wl.
 * x  : x coordinate where we want to insert the word.
 * y  : y coordinate where we want to insert the word.
 * or : orietation of the word.
 * Return 1 if it can be inserted, 0 otherwise.
 */
int cw_ok_add_word_at_pos(crossword cw, word_list wl,
                          int i, int x, int y, int or){
  int word_length = (int) strlen(wl->l[i].w);
  int k;
  int ok;

  // Check if in min range.
  if((x < 0) || (y < 0)) return 0;

  if(or == HORIZONTAL){
    // Check if in max range.
    if((y >= cw->height) || (x + word_length >= cw->width)) return 0;

    // Check if we do not touch an other word at the extremities.
    if(x > 0)
      if(cw->grid[x - 1][y] != EMPTY_CHAR) return 0;
    if(x + word_length < cw->width - 1)
      if(cw->grid[x + word_length][y] != EMPTY_CHAR) return 0;

    // Check that there is no overlap or that we don't touch on the sides.
    for(k = x; k < (int) (x + strlen(wl->l[i].w)); k++){
      ok = cw->grid[k][y] != EMPTY_CHAR;
      if(y > 0) ok = ok || (cw->grid[k][y - 1] != EMPTY_CHAR);
      if(y < cw->width - 1) ok = ok || (cw->grid[k][y + 1] != EMPTY_CHAR);

      if(ok && (cw->grid[k][y] != wl->l[i].w[k - x])) return 0;
    }
  }else{
    // Check if in max range.
    if((x >= cw->width) || (y + word_length >= cw->height)) return 0;

    // Check if we do not touch an other word at the extremities.
    if(y > 0)
      if(cw->grid[x][y - 1] != EMPTY_CHAR) return 0;
    if(y + word_length < cw->height - 1)
      if(cw->grid[x][y + word_length] != EMPTY_CHAR) return 0;

    // Check that there is no overlap or that we don't touch on the sides.
    for(k = y; k < (int) (y + strlen(wl->l[i].w)); k++){
      ok = cw->grid[x][k] != EMPTY_CHAR;
      if(x > 0) ok = ok || (cw->grid[x - 1][k] != EMPTY_CHAR);
      if(x < cw->height - 1) ok = ok || (cw->grid[x + 1][k] != EMPTY_CHAR);

      if(ok  && (cw->grid[x][k] != wl->l[i].w[k - y])) return 0;
    }
  }

  return 1;
}
 
/*
 * Compute a crossword.
 * cw : the crossword. Must be initialized.
 * wl : the list of words.
 * Returns 0 if all went well. -1 otherwise.
 */
int cw_compute(crossword cw, word_list wl){
  int o, len;
  int i, j;
  int nb_loop, nb_rand_ch;
  int l = 1;
  int r;
  char ch;
  int pos_word;
  int x, y;
  int nb_pl;

  // No words to place.
  if(0 == wl->next_free) return 0;

  // Pick a word at random and place it at a random position.
  i = rand() % wl->next_free;
  o = rand() % 2;
  len = strlen(wl->l[i].w);
  wl->l[i].used = 1;
  if(o == VERTICAL){
    x = rand() % cw->width;
    y = rand() % (cw->height - len);
  } else { // HORIZONTAL
    x = rand() % (cw->width - len);
    y = rand() % cw->height;
  }
  if(0 != cw_add_word(cw, wl, i, x, y, o, &l))
    return -1;

  // Place the other words.
  for(nb_loop = 0; nb_loop < NB_LOOP; nb_loop++)
    for(nb_pl = 0; nb_pl < wl->next_free; nb_pl++){
      // We choose a word at random.
      i = rand() % wl->next_free;

      if(0 == wl->l[i].used){
        for(nb_rand_ch = 0; nb_rand_ch < NB_RAND_CH; nb_rand_ch++){
          // We try to insert the word using the word number r.
          r = rand() % cw->nb_words;

          for(pos_word = 0; pos_word < (int) strlen(cw->words[r].w.w); pos_word++){
            ch = cw->words[r].w.w[pos_word];

            // We look for a match.
            for(j = 0; j < (int) strlen(wl->l[i].w); j++){
              if(ch == wl->l[i].w[j]){
                if(VERTICAL == cw->words[r].orient){
                  // In this case, the new word will have to be horizontal.
                  x = cw->words[r].posx - j;
                  y = pos_word + cw->words[r].posy;

                  if(1 == cw_ok_add_word_at_pos(cw, wl, i, x, y, HORIZONTAL)){
                    // We place the word.
                    if(0 != cw_add_word(cw, wl, i, x, y, HORIZONTAL, &l)){
                      return -1;
                    }
                    /////// TODO improve...
                    nb_rand_ch = NB_RAND_CH;
                    pos_word = MAX_WORD_SIZE + 1;
                    ///////
                    break;
                  }
                }else{
                  // In this case, the new word will have to be vertical.
                  x = pos_word + cw->words[r].posx;
                  y = cw->words[r].posy - j;

                  if(1 == cw_ok_add_word_at_pos(cw, wl, i, x, y, VERTICAL)){
                    // We place the word.
                    if(0 != cw_add_word(cw, wl, i, x, y, VERTICAL, &l)){
                      return -1;
                    }
                    /////// TODO improve...
                    nb_rand_ch = NB_RAND_CH;
                    pos_word = MAX_WORD_SIZE + 1;
                    ///////
                    break;
                  }
                }
              }
            }
          }
        }
      }
    }
  return 0;
}
