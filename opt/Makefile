CC=gcc
PREFIX=$PWD
SRC=opt.c
EXE=a.out

.PHONY: all install clean distclean

all:
	$(CC) $(SRC) -o $(EXE)
install:
	cp $(EXE) $(PREFIX)/bin/
clean:
	rm -rf *.o $(EXE)
distclean:clean
	rm -rf $(EXE) 