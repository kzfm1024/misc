TARGET = TestLL _fifo.so testff


all: $(TARGET)


TestLL: Node.o LinkedList.o TestLL.o
	gcj -g --main=TestLL TestLL.o Node.o LinkedList.o -o TestLL

Node.o: Node.java
	gcj -c -g --encoding=UTF-8 Node.java

LinkedList.o: LinkedList.java
	gcj -c -g --encoding=UTF-8 LinkedList.java

TestLL.o: TestLL.java
	gcj -c -g --encoding=UTF-8 TestLL.java


_fifo.so: fifo.o fifo_wrap.o
	gcc -shared fifo.o fifo_wrap.o -o _fifo.so

fifo.o: fifo.c
	gcc -fPIC -g -c fifo.c -I/usr/include/python2.4

fifo_wrap.o: fifo_wrap.c
	gcc -fPIC -g -c fifo_wrap.c -I/usr/include/python2.4

fifo.py fifo_wrap.c: fifo.i
	swig -python fifo.i

testff:	testff.o
	ld -o testff testff.o

testff.o: testff.s
	as -a --gstabs -o testff.o testff.s

clean:
	-rm *.o $(TARGET) fifo_wrap.c fifo.py fifo.pyc


test: $(TARGET) textcount.pl test.txt tf.py
	@echo
	-./TestLL 8 5 12
	@echo
	perl textcount.pl test.txt
	@echo
	python tf.py test.txt
	@echo
	python testfifo.py
	gdb testff -x gdbcmd.txt
	@echo
