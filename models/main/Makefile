CC = gcc
CFLAGS = -Wall -g
SDIR = src
ODIR = obj
TDIR = tst
INC_SDIR = ../src/V2X_TX_Baseband_ert_rtw
INC = -I inc -I $(INC_SDIR)

all: v2x_tx_bb_obj
	mkdir -p bin
	$(CC) -o v2x_tx_bb_main $(ODIR)/v2x_tx_bb_main.o
	$(CC) -o v2x_tx_bb_test $(ODIR)/v2x_tx_bb_test.o

v2x_tx_bb_obj:
	$(CC) $(CFLAGS) $(INC) -c $(INC_SDIR)/V2X_TX_Baseband.c -o $(ODIR)/V2X_TX_Baseband.o
	$(CC) $(CFLAGS) $(INC) -c $(INC_SDIR)/V2X_TX_Baseband_data.c -o $(ODIR)/V2X_TX_Baseband_data.o
	$(CC) $(CFLAGS) $(INC) -c $(SDIR)/v2x_tx_bb_main.c -o $(ODIR)/v2x_tx_bb_main.o
	$(CC) $(CFLAGS) $(INC) -c $(TDIR)/v2x_tx_bb_test.c -o $(ODIR)/v2x_tx_bb_test.o

clean:
	rm -f $(ODIR)/*.o
	rm -f v2x_tx_bb_main
	rm -f v2x_tx_bb_test
