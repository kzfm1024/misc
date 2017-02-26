CC = gcc
CFLAGS = -g

TARGET = a.out s.out d.out psax

all: $(TARGET)

a.out: a.c z/b.c
	(cd z && $(CC) $(CFLAGS) -c b.c)
	$(CC) $(CFLAGS) -o a.out a.c z/b.o

s.out: a.c z/lib88s.a
	$(CC) $(CFLAGS) -o s.out a.c -Lz -l88s

d.out: a.c z/lib88d.so
	$(CC) $(CFLAGS) -o d.out a.c -Lz -l88d

z/lib88s.a: 
	(cd z && make all)

z/lib88d.so:
	(cd z && make all)


psax: psax.c
	$(CC) $(CFLAGS) -o psax psax.c -lcurses

clean:
	-rm *.o $(TARGET)
	(cd z && make clean)


test: $(TARGET)
	@echo
	-echo 2 | ./a.out
	@echo
	-echo 2 | ./s.out
	@echo
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./z ldd d.out
	-echo 2 | LD_LIBRARY_PATH=$LD_LIBRARY_PATH:./z ./d.out
	@echo
	-./psax
	@echo
