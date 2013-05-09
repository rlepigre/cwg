/** output.h ****************************************************************
 * 31/01/2013               CWG - Crossword generator                       *
 * Content of this file:                                                    *
 *   Definition of the functions printing a crossword in different formats. *
 *   The most simple way to visualize a crossword is through the command    *
 *   line in mode text, but crosswords can also be printed in the LaTeX and *
 *   HTML formats.
 * Author: Rodolphe Lepigre <rlepigre@gmail.com>                            *
 ****************************************************************************/
#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "cw.h"

/*
 * Print a crossword.
 * f  : the file where to print the crossword.
 * cw : the crossword. Must be initialized.
 */
void cw_print(FILE *f, crossword cw);

/*
 * Print a crossword in LaTeX mode.
 * cw : the crossword. Must be initialized.
 * Returns 0 if all went well. -1 otherwise.
 */
int cw_print_latex(crossword cw);

/*
 * Print a crossword in HTML mode.
 * cw : the crossword. Must be initialized.
 * Returns 0 if all went well. -1 otherwise.
 */
int cw_print_html(crossword cw);

#endif
