# Compiler options
CC = gcc
CFLAGS = -Wall -O3 -g

# Source directories
SDIR = src
IDIR = inc
ODIR = obj

# Include directories
INC = -I $(IDIR)

# Library flags
LDFLAGS = -lm -lpthread

# Main files
SRC = $(wildcard $(SDIR)/*.c)
OBJ = $(patsubst %.c,$(ODIR)/%.o,$(notdir $(SRC)))

# Addiotnal stuff
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CFLAGS += -framework iio
else
	LDFLAGS += -liio
endif

# Make targets
all: $(ODIR) xcvr_demo

xcvr_demo: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o xcvr_demo

# Object targets
$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) -c $< -o $@

# Clean
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f xcvr_demo
