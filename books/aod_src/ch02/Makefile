CC = gcc
CFLAGS = -g3 -Wall -Wextra

TARGET = swap swapflaw until-anomaly fibonacci

all: $(TARGET)

swap: main.o swapper.o
	$(CC) -o swap main.o swapper.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

swapper.o: swapper.c
	$(CC) $(CFLAGS) -c swapper.c


swapflaw: swapflaw.c
	$(CC) $(CFLAGS) -o swapflaw swapflaw.c

until-anomaly: until-anomaly.c
	$(CC) $(CFLAGS) -o until-anomaly until-anomaly.c

fibonacci: fibonacci.c
	$(CC) $(CFLAGS) -o fibonacci fibonacci.c


clean:
	-rm *.o $(TARGET)

test: $(TARGET)
	@echo
	./swap
	@echo
	./swapflaw
	@echo
	./until-anomaly
	@echo
	@echo
	./fibonacci
