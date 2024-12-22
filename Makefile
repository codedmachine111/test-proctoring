CC = g++
CFLAGS = -Wall -Wextra -I include -I /usr/local/include/opencv4/

BINDIR = bin
BUILDDIR = build
SRCDIR = src

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(patsubst $(SRCDIR)/%.cpp, $(BUILDDIR)/%.o, $(SRCFILES))

OPENCV_LIBS = -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs -lopencv_objdetect
TARGET = $(BINDIR)/main

all: $(TARGET)

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create final executable
$(TARGET): $(OBJFILES)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJFILES) -o $(TARGET) $(OPENCV_LIBS) -pthread 

clean:
	rm -rf $(BINDIR) $(BUILDDIR)

.PHONY: all clean