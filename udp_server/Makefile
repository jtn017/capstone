# variables
CXX=gcc 			# compiler
CXXFLAGS=-O3 	# compiler flags go here
EXEC=server  	# output file name

# include, source, and object directories
INCLUDEDIR=include
SRCDIR=src
OBJDIR=objs

# getting opencv include path from pkg-config
#OPENCV_INCLUDE_PATH = `pkg-config --cflags opencv`
# getting opencv libs path from pkg-config
#OPENCV_LIBS = `pkg-config --libs opencv`

# libraries + opencv libs
#LIBS=-lc $(OPENCV_LIBS)

# libraries + opencv libs
#LIBS=-lc

# wildcard adds all the *.c files in source directory to SRCS
SRCS=$(wildcard $(SRCDIR)/*.c)
# patsubst finds $(SRCDIR)/%.c in $(SRCS) and replaces them with $(OBJDIR)/%.o
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
# includes + opencv include path
#INCLUDES=-I$(INCLUDEDIR) $(OPENCV_INCLUDE_PATH)
INCLUDES=-I$(INCLUDEDIR)

all: $(EXEC)

$(EXEC): $(OBJDIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(OBJS) -o $(EXEC) $(LIBS)

# compiles object files from source files
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c
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
