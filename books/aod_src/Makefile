
SUBDIR = ch01 ch02 ch03 ch04 ch05 ch06 ch07 ch08 

all:
	@for dir in $(SUBDIR); do (cd $$dir ; make all) ; done

clean:
	@for dir in $(SUBDIR); do (cd $$dir ; make clean) ; done

test:
	@for dir in $(SUBDIR); do (cd $$dir ; make test) ; done

