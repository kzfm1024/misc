CC = gcc
CFLAGS = -g

TARGET = vmtest signal buserror abort sigsegv snpr cstring

all: $(TARGET)

vmtest: vmtest.c
	$(CC) $(CFLAGS) -o vmtest vmtest.c

signal: signal.c
	$(CC) $(CFLAGS) -o signal signal.c

buserror: buserror.c
	$(CC) $(CFLAGS) -o buserror buserror.c

abort: abort.c
	$(CC) $(CFLAGS) -o abort abort.c

sigsegv: sigsegv.c
	$(CC) $(CFLAGS) -o sigsegv sigsegv.c

snpr: snpr.c
	$(CC) $(CFLAGS) -o snpr snpr.c

cstring: cstring.c
	$(CC) $(CFLAGS) -o cstring cstring.c


clean:
	-rm *.o $(TARGET)

test: $(TARGET)
	@echo
	echo 5 | ./vmtest
	@echo
	-./buserror
	@echo
	-echo 4 | ./signal
	@echo
	-./abort
	@echo
	-./sigsegv
	@echo
	./snpr
	@echo
	-./cstring
	@echo

