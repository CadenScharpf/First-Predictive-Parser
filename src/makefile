IDIR=../include
CC=g++
CFLAGS=-I$(IDIR)

ODIR=../obj

_DEPS = inputbuf.h lexer.h parser.h REG.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = inputbuf.o lexer.o REG.o parser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

../bin/Project2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 