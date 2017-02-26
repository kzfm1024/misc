CC = gcc
CFLAGS = -g

TARGET = double-trouble perror-example strerror-example memprobs \
	outOfBound_without_efence outOfBound_with_efence \
	malloc-check-0 mcheckTest mtrace1 mtrace2

all: $(TARGET)


double-trouble: double-trouble.c
	$(CC) $(CFLAGS) -o double-trouble double-trouble.c -lm

perror-example: perror-example.c
	$(CC) $(CFLAGS) -o perror-example perror-example.c

strerror-example: strerror-example.c
	$(CC) $(CFLAGS) -o strerror-example strerror-example.c

memprobs: memprobs.c
	$(CC) $(CFLAGS) -std=c99 -o memprobs memprobs.c


outOfBound_without_efence: outOfBound.c
	gcc -g3 -Wall -std=c99 outOfBound.c -o outOfBound_without_efence

outOfBound_with_efence: outOfBound.c
	gcc -g3 -Wall -std=c99 outOfBound.c -o outOfBound_with_efence -lefence

malloc-check-0: malloc-check-0.c
	$(CC) $(CFLAGS) -std=c99 -o malloc-check-0 malloc-check-0.c 

mcheckTest: mcheckTest.c
	gcc -g -Wall -std=c99 -o mcheckTest mcheckTest.c

mtrace1: mtrace1.c
	gcc -g -Wall -std=c99 -o mtrace1 mtrace1.c -lmcheck

mtrace2: mtrace2.c
	gcc -g -Wall -std=c99 -o mtrace2 mtrace2.c

clean:
	-rm *.o $(TARGET)


test: $(TARGET) scan.c
	@echo
	-./double-trouble
	@echo
	-./perror-example
	@echo
	-./strerror-example
	@echo
	-splint scan.c
	@echo
	-./outOfBound_without_efence
	@echo
	-./outOfBound_with_efence
	@echo
	-MALLOC_CHECK_=1 ./malloc-check-0
	@echo
	-./mcheckTest
	@echo
	-MALLOC_TRACE="mtrace1.log" ./mtrace1
	@echo
	-mtrace mtrace1 mtrace1.log
	@echo
	-MALLOC_TRACE="mtrace2.log" ./mtrace2
	@echo
	-mtrace mtrace2 mtrace2.log
	@echo
