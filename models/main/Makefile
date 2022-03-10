CC = gcc
CFLAGS = -Wall -g
SDIR = src
ODIR = obj
INC_SDIR = ../src/V2X_TX_Baseband_ert_rtw
INC = -I inc -I $(INC_SDIR)
SRC = $(SDIR)/v2x_tx_bb_main.c
OBJ = $(ODIR)/v2x_tx_bb_main.o

all: v2x_tx_bb_obj
	$(CC) -o v2x_tx_bb_main $(ODIR)/v2x_tx_bb_main.o

v2x_tx_bb_obj:
	$(CC) $(CFLAGS) $(INC) -c $(INC_SDIR)/V2X_TX_Baseband.c -o $(ODIR)/V2X_TX_Baseband.o
	$(CC) $(CFLAGS) $(INC) -c $(INC_SDIR)/V2X_TX_Baseband_data.c -o $(ODIR)/V2X_TX_Baseband_data.o
	$(CC) $(CFLAGS) $(INC) -c $(SDIR)/v2x_tx_bb_main.c -o $(ODIR)/v2x_tx_bb_main.o

# ar rcs $(ODIR)/v2x_tx_bb_lib.a $(ODIR)/V2X_TX_Baseband.o $(ODIR)/V2X_TX_Baseband_data.o
# $(CC) $(CFLAGS) $(INC) -c $(SDIR)/v2x_tx_bb_main.c -o $(ODIR)/v2x_tx_bb_main.o $(ODIR)/v2x_tx_bb_lib.a
# $(CC) $(CFLAGS) $(INC) -c $(SDIR)/v2x_tx_bb_main.c $(INC_SDIR)/V2X_TX_Baseband.c \
# 					   $(INC_SDIR)/V2X_TX_Baseband_data.c -o $(ODIR)/v2x_tx_bb_main.o

clean:
	rm -f $(ODIR)/*.o
	rm -f v2x_tx_bb_main
