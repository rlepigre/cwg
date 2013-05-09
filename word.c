/** word.c ******************************************************************
 * 27/01/2013               CWG - Crossword generator                       *
 * Content of this file:                                                    *
 *   Implementation of the functions defined in "word.h".                   *
 * Author: Rodolphe Lepigre <rlepigre@gmail.com>                            *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "const.h"
#include "word.h"

/*
 * Function scanning a file for words.
 * input : the file.
 * list  : the list of words scanned.
 * verbose : if set to 1 verbose mode. 0 otherwise.
 * Failures are handled internally.
 */
void fscan_words(FILE* input, word_list* list, int verbose){
  // For getline
  char* line = NULL;
  size_t nbyte = 0;
  ssize_t nb_read;

  // For parsing
  char wrd[MAX_WORD_SIZE + MAX_DEF_SIZE + 1];
  char def[MAX_WORD_SIZE + MAX_DEF_SIZE + 1];
  int ret_scan;

  // For length checking
  size_t len;

  // Others
  int lnum = 1;
  char* wordpt;
  word* wltmp = NULL;
  int i;

  // Initial memory allocation.
  *list = malloc(sizeof(struct __word_list__));
  if(NULL == *list){
    fprintf(stderr, "Memory allocation error in fscan_words...\n");
    exit(EXIT_FAILURE);
  }
  (*list)->l = malloc(INIT_NB * sizeof(word));
  if(NULL == (*list)->l){
    fprintf(stderr, "Memory allocation error in fscan_words...\n");
    exit(EXIT_FAILURE);
  }
  (*list)->next_free = 0;
  (*list)->size = INIT_NB;

  // Looping through the file.
  nb_read = getline(&line, &nbyte, input);
  while(1 < nb_read){
    // Check if line not too long
    if(nb_read > MAX_WORD_SIZE + MAX_DEF_SIZE + 1){
      fprintf(stderr, "Parsing error, at line %i... (line too long)\n", lnum);
      free(line);
      free_words(*list);
      exit(EXIT_FAILURE);
    }

    // Parsing into word and def.
    ret_scan = sscanf(line, "%s : %[^\n]s\n", wrd, def);
    if(EOF == ret_scan || 2 > ret_scan){
      fprintf(stderr, "Parsing error, at line %i... (no parse)\n", lnum);
      free(line);
      free_words(*list);
      exit(EXIT_FAILURE);
    }

    // Checking sizes.
    len = strlen(wrd);
    if(len < MIN_WORD_SIZE || len > MAX_WORD_SIZE){
      fprintf(stderr, "Parsing error, at line %i... (wrong word size)\n", lnum);
      fprintf(stderr, "Size must be between %i and %i.\n", MIN_WORD_SIZE,
              MAX_WORD_SIZE);
      free(line);
      free_words(*list);
      exit(EXIT_FAILURE);
    }
    len = strlen(def);
    if(len < MIN_DEF_SIZE || len > MAX_DEF_SIZE){
      fprintf(stderr, "Parsing error, at line %i... (wrong clue size)\n", lnum);
      fprintf(stderr, "Size must be between %i and %i.\n", MIN_DEF_SIZE,
              MAX_DEF_SIZE);
      free(line);
      free_words(*list);
      exit(EXIT_FAILURE);
    }

    // Do the write and update
    strncpy((*list)->l[(*list)->next_free].w, wrd, MAX_WORD_SIZE + 1);
    strncpy((*list)->l[(*list)->next_free].d, def, MAX_DEF_SIZE + 1);
    (*list)->l[(*list)->next_free].used = 0;
    if(verbose){
      fprintf(stderr, "Word scaned : ");
      wordpt = (*list)->l[(*list)->next_free].w;
      for(i = 0; i < MAX_WORD_SIZE && wordpt[i] != '\0'; i++)
        putc(wordpt[i], stderr);
      for( ; i < MAX_WORD_SIZE; i++) putc(' ', stderr);
      fprintf(stderr, " = %s\n", (*list)->l[(*list)->next_free].d);
    }
    (*list)->next_free++;

    // Check if need to increase array size.
    if((unsigned) (*list)->next_free >= (*list)->size){
      (*list)->size *= 2;
      if(NULL == (wltmp = realloc((*list)->l, (*list)->size * sizeof(word)))){
        fprintf(stderr, "Memory reallocation error in fscan_words...\n");
	free(line);
	free_words(*list);
        exit(EXIT_FAILURE);
      }
      (*list)->l = wltmp;
    }

    // Reinit
    nb_read = getline(&line, &nbyte, input);
    lnum++;
  }

  // Free memory
  free(line);
}

/*
 * Free the memory inside a word_list.
 * wl : the word list.
 */
void free_words(word_list wl){
  free(wl->l);
  free(wl);
}
