CC = g++
CFLAGS = -Wall -Wextra -I include

BINDIR = bin
BUILDDIR = build
SRCDIR = src

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCFILES))

TARGET = $(BINDIR)/main

all: $(TARGET)

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create final executable
$(TARGET): $(OBJFILES)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJFILES) -o $(TARGET)

clean:
	rm -rf $(BINDIR) $(BUILDDIR)

.PHONY: all clean