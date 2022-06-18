#Toja makefile e prednaznachen za git terminal, ako platformata e Windows.
#This makefile is runnable only from a git terminal if the platform (OS) in use is Windows.

CC=g++
CFLAGS=-Wall -g

BIN=./bin
SRC=./src

INCLUDE=$(wildcard ./include/*.h)
SRCF=$(wildcard $(SRC)/*.cc)
OBJ=$(patsubst %.cc, %.o, $(SRCF))

ifdef __WIN64
$(BIN)/esm.exe: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
else
$(BIN)/esm: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
endif

$(SRC)/%.o: $(SRC)/%.cc $(INCLUDE)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm $(BIN)/esm*
	rm $(SRC)/*.o