# variables
CXX=gcc 			# compiler
#CXXFLAGS=-O3 	# compiler flags go here
#CXXFLAGS=-Wall -g 
CXXFLAGS=-O0 -g 
EXEC=client  	# output file name

# include, source, and object directories
INC=include gen
INCLUDEDIR=$(foreach d, $(INC), -I$d)
SRCDIR=src
OBJDIR=objs

# libraries
LIBS=-lm -levent -lcurl

# wildcard adds all the *.c files in source directory to SRCS
SRCS=$(wildcard $(SRCDIR)/*.c)
# patsubst finds $(SRCDIR)/%.c in $(SRCS) and replaces them with $(OBJDIR)/%.o
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
# includes + opencv include path
INCLUDES=$(INCLUDEDIR) 

all: $(EXEC)

$(EXEC): $(OBJDIR) $(OBJS)
	@echo "in exec dependency"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(EXEC) $(LIBS)

# compiles object files from source files
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "in obj dependency"
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJDIR):
	mkdir -p $(OBJDIR)

# removing object files and output file
clean:
	rm -f $(EXEC)
	rm -f $(OBJDIR)/*.o

run:
	./$(EXEC)
#taskset -c 1 ./$(EXEC)

# prints variables
debug:
	@echo compiler: $(CXX)
	@echo flags: $(CXXFLAGS)
	@echo includes: $(INCLUDES)
	@echo libs: $(LIBS)
	@echo source directory: $(SRCDIR)
	@echo objects directoy: $(OBJDIR)
	@echo source files $(SRCS)
	@echo object files: $(OBJS)
