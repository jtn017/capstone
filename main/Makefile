# Compiler options
CC = gcc
CFLAGS = -Wall -O3 -g

# Source directories
MAIN_SDIR = src
GEN_SDIR = gen
IDIR = inc
ODIR = obj

# Include directories
INC = -I $(IDIR) -I $(GEN_SDIR)

# Library flags
LDFLAGS = -lm -lpthread -lcurl

# Generated files
# NOTE: Additional auto-generated files require some massaging of extern vars...
GEN_SRC = $(wildcard $(GEN_SDIR)/*.c)
GEN_OBJ = $(patsubst %.c,$(ODIR)/%.o,$(notdir $(GEN_SRC)))

# Main files
MAIN_SRC = $(wildcard $(MAIN_SDIR)/*.c)
MAIN_SRC += $(wildcard $(MAIN_SDIR)/*.cpp)
MAIN_OBJ = $(patsubst %.c,$(ODIR)/%.o,$(notdir $(MAIN_SRC)))

# Addiotnal stuff
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CFLAGS += -framework iio
else
	LDFLAGS += -liio
endif

# Make targets
all: $(ODIR) xcvr_demo

xcvr_demo: $(GEN_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $(GEN_OBJ) $(MAIN_OBJ) $(LDFLAGS) -o xcvr_demo

# Object targets
$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(GEN_SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) -c $< -o $@

$(ODIR)/%.o: $(MAIN_SDIR)/%.c
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) -c $< -o $@

$(ODIR)/%.o: $(MAIN_SDIR)/%.cpp
	$(CC) $(CFLAGS) $(INC) $(LDFLAGS) -c $< -o $@

run:
	sudo ./xcvr_demo

# Clean
.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f xcvr_demo
