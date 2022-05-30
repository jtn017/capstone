# Compiler options
CC = gcc
CFLAGS = -Wall -g

# Source directories
ODIR = obj
MAIN_SDIR = src
GEN_SDIR = gen

# Include directories
INC = -I inc -I $(GEN_SDIR)

# Generated files
# NOTE: Additional auto-generated files require some massaging of extern vars...
GEN_SRC = $(wildcard $(GEN_SDIR)/*.c)
GEN_OBJ = $(patsubst %.c,$(ODIR)/%.o,$(notdir $(GEN_SRC)))

# Main files
MAIN_SRC = $(wildcard $(MAIN_SDIR)/*.c)
MAIN_OBJ = $(patsubst %.c,$(ODIR)/%.o,$(notdir $(MAIN_SRC)))

# Other commands
MKDIR = $(mkdir -p)

# Make targets
all: $(ODIR) main

main: $(GEN_OBJ) $(MAIN_OBJ) 
	$(CC) $(CFLAGS) $(GEN_OBJ) $(MAIN_OBJ) -lm -levent -lcurl -o v2x_tx_main

mv_gen_code:
	cp ../src/V2X_TX_Baseband_ert_rtw/V2X_TX_Baseband.c gen/.
	cp ../src/V2X_TX_Baseband_ert_rtw/V2X_TX_Baseband.h gen/.
	cp ../src/V2X_TX_Baseband_ert_rtw/V2X_TX_Baseband_data.c gen/.
	cp ../src/V2X_TX_Modulator_ert_rtw/V2X_TX_Modulator.c gen/.
	cp ../src/V2X_TX_Modulator_ert_rtw/V2X_TX_Modulator.h gen/.
	cp ../src/V2X_TX_Modulator_ert_rtw/V2X_TX_Modulator_data.c gen/.
	cp ../src/V2X_TX_Modulator_ert_rtw/rtwtypes.h gen/.
	cp ../src/V2X_RX_Baseband_ert_rtw/V2X_RX_Baseband.c gen/.
	cp ../src/V2X_RX_Baseband_ert_rtw/V2X_RX_Baseband.h gen/.
	cp ../src/V2X_RX_Baseband_ert_rtw/V2X_RX_Baseband_data.c gen/.

# Object targets
$(ODIR)/%.o: $(GEN_SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(ODIR)/%.o: $(MAIN_SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(ODIR):
	mkdir -p $(ODIR)

run:
	./v2x_tx_main	
# Clean
.PHONY: clean 

clean:
	rm -f $(ODIR)/*.o
	rm -f v2x_tx_main
	rm -f v2x_tx_bb_test
