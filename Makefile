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
