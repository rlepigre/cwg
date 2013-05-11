## Copyright (c) 2012-2013 Rodolphe Lepigre ##################################
# This file is part of CWG.                                                  #
#                                                                            #
# CWG is free software: you can redistribute it and/or modify it under the   #
# terms of the GNU General Public License as published by the Free Software  #
# Foundation, either version 3 of the License, or (at your option) any later #
# version.                                                                   #
#                                                                            #
# This program is distributed in the hope that it will be useful, but WITH-  #
# OUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or  #
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for   #
# more details.                                                              #
#                                                                            #
# You should have received a copy of the GNU General Public License along    #
# with this program. If not, see <http://www.gnu.org/licenses/>.             #
#                                                                            #
## Makefile ##################################################################
# 27/01/2013               CWG - Crossword generator                         #
# Content of this file: Makefile, configuration for the installation, see    #
#   README for more information.                                             #
# Author: Rodolphe Lepigre <rlepigre@gmail.com>                              #
##############################################################################
FLAGS = -Wall -Wextra -Werror
GCC = gcc $(FLAGS)
INSTALL_DIR = /usr/local/bin
LATEX_DIR = /usr/share/texmf/tex/latex

all: cwg clean

cwg : main.o word.o cw.o output.o
	$(GCC) main.o word.o cw.o output.o -o cwg

main.o : main.c
	$(GCC) -c main.c

word.o : word.c
	$(GCC) -c word.c

cw.o : cw.c
	$(GCC) -c cw.c

output.o : output.c
	$(GCC) -c output.c

clean:
	rm -f *.o *~

dist_clean: clean
	rm -f cwg

install: cwg
	mv cwg $(INSTALL_DIR)/
	mkdir $(LATEX_DIR)/cwpuzzle
	cp cwpuzzle.sty $(LATEX_DIR)/cwpuzzle/
	rm -f *.o *~

uninstall:
	rm -f $(INSTALL_DIR)/cwg
	rm -rf $(LATEX_DIR)/cwpuzzle

reinstall: uninstall install
