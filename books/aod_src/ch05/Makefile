CC = gcc
CFLAGS = -g

# set MPICH bin directory
MPIBIN = /usr/local/mpi/bin

TARGET = srvr clnt prime dij primepipe

all: $(TARGET)


srvr: srvr.c
	$(CC) $(CFLAGS) -o srvr srvr.c

clnt: clnt.c
	$(CC) $(CFLAGS) -o clnt clnt.c

prime: prime.c
	$(CC) $(CFLAGS) -o prime prime.c -lpthread -lm

dij: dijkstra.c
	$(CC) $(CFLAGS) -fopenmp -o dij dijkstra.c

primepipe: primepipe.c
	$(MPIBIN)/mpicc -o primepipe primepipe.c


clean:
	-rm *.o $(TARGET)


test: $(TARGET)
	@echo
	./srvr &
	./clnt localhost
	sleep 1 ; killall srvr
	@echo
	-./prime 100 2
	@echo
	-./dij 6 1
	@echo
	$(MPIBIN)/mpirun -np 3 primepipe 100 0
	@echo

