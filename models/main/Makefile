# Compiler options
CC = gcc
CFLAGS = -Wall -g

# Source directories
ODIR = obj
TEST_SDIR = tst
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

# Test files
TEST_SRC = $(wildcard $(TEST_SDIR)/*.c)
TEST_OBJ = $(patsubst %.c,$(ODIR)/%.o,$(notdir $(TEST_SRC)))

# Other commands
MKDIR = $(mkdir -p)

# Make targets
all: main

test: $(GEN_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) $(GEN_OBJ) $(TEST_OBJ) -lm -levent -o v2x_tx_bb_test

main: $(GEN_OBJ) $(MAIN_OBJ) 
	$(CC) $(CFLAGS) $(GEN_OBJ) $(MAIN_OBJ) -lm -levent -o v2x_tx_main

# Object targets
$(ODIR)/%.o: $(GEN_SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(ODIR)/%.o: $(MAIN_SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

$(ODIR)/%.o: $(TEST_SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Clean
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f v2x_tx_main
	rm -f v2x_tx_bb_test
