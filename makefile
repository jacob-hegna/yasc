TARGET=yasc
TEST=yasc-test
OBJDIR=./obj
BINDIR=./bin

CC=/usr/local/bin/g++-8
LL=/usr/local/bin/g++-8

CCFLAGS= -std=c++17 -O3 -Wall

.PHONY: $(TARGET) $(TEST) clean

all: $(BINDIR)/$(TARGET) $(BINDIR)/$(TEST)

$(BINDIR)/$(TARGET): $(OBJDIR)/main.cpp.o $(OBJDIR)/parse.cpp.o
	$(CC) -o $@ $(OBJDIR)/main.cpp.o $(OBJDIR)/parse.cpp.o

$(BINDIR)/$(TEST): $(OBJDIR)/test.cpp.o $(OBJDIR)/parse.cpp.o
	$(CC) -o $@ $(OBJDIR)/test.cpp.o -lgtest -lgtest_main $(OBJDIR)/parse.cpp.o

$(OBJDIR)/main.cpp.o:
	$(CC) -o $@ $(CCFLAGS) -c main.cpp

$(OBJDIR)/parse.cpp.o:
	$(CC) -o $@ $(CCFLAGS) -c parse.cpp

$(OBJDIR)/test.cpp.o:
	$(CC) -o $@ $(CCFLAGS) -c test.cpp

clean:
	rm -rf $(OBJDIR)/* $(BINDIR)/*
