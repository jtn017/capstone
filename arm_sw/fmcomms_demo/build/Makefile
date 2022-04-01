# Make file for fmcomms4 demo
SRC=../src
INCLUDE=-I$(SRC)
CC=gcc
#OPTIONS= -DFFTMTX
OBS=sdrinit.o sdrxcvr.o sdrmain.o 
TARGETS := xcvr_demo

CFLAGS = -Wall -O3 -g  $(INCLUDE) $(OPTIONS)
LDFLAGS=-lm -lpthread

UNAME_S := $(shell uname -s)
BIN= xcvr_demo


ifeq ($(UNAME_S),Darwin)
	CFLAGS += -framework iio
else
	LDFLAGS += -liio
endif

.PHONY: all clean

all: $(TARGETS)


$(BIN) : $(OBS)
	$(CC) -o $(BIN) $(OBS)  $(CFLAGS) $(LDFLAGS)
	#@cp $(BIN) /../bin/
sdrinit.o: $(SRC)/sdrinit.c
	$(CC) -c $(CFLAGS) $(SRC)/sdrinit.c
sdrxcvr.o : $(SRC)/sdrxcvr.c
	$(CC) -c $(CFLAGS) $(SRC)/sdrxcvr.c
sdrmain.o: $(SRC)/sdrmain.c
	$(CC) -c $(CFLAGS) $(SRC)/sdrmain.c
sdrcmn.o : $(SRC)/sdrcmn.c
	$(CC) -c $(CFLAGS) $(SRC)/sdrcmn.c


sdrmain.o: $(SRC)/sdr.h
sdrinit.o: $(SRC)/sdr.h
sdrxcvr.o: $(SRC)/sdr.h
sdrcmn.o : $(SRC)/sdr.h

clean:
	rm -f $(TARGETS) $(TARGETS:%=%.o) *.o
