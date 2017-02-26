CC = gcc
CFLAGS = -g

TARGET = insert_sort

all: $(TARGET)

insert_sort: ins.c
	$(CC) $(CFLAGS) -o insert_sort ins.c

clean:
	-rm *.o $(TARGET)

test: $(TARGET)
	@echo
	./insert_sort 12 5 19 22 6 1
	@echo

