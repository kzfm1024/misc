CC = gcc
CFLAGS = -g

TARGET = lib88s.a lib88d.so

all: $(TARGET)

lib88s.a: b.c
	$(CC) $(CFLAGS) -c b.c
	ar rc lib88s.a b.o

lib88d.so: b.c
	$(CC) $(CFLAGS) -fPIC -c b.c
	$(CC) -shared -o lib88d.so b.o

clean:
	rm *.o $(TARGET)

test:
	@echo
