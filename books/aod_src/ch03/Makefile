CC = gcc
CFLAGS = -g
CXX = g++
CXXFLAGS = -g

TARGET = bintree artif dyn bintree++

all: $(TARGET)

bintree: bintree.c
	$(CC) $(CFLAGS) -o bintree bintree.c

artif: artif.c
	$(CC) $(CFLAGS) -o artif artif.c

dyn: dyn.c
	$(CC) $(CFLAGS) -o dyn dyn.c

bintree++: bintree.cc
	$(CXX) $(CXXFLAGS) -o bintree++ bintree.cc


clean:
	-rm *.o $(TARGET)

test: $(TARGET)
	@echo
	./bintree 12 8 5 19 16
	@echo
	-./artif
	@echo
	-echo 1 2 | ./dyn
	@echo
	./bintree++ 12 8 5 19 16
	@echo
