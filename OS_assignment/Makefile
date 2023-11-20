CFLAGS = -Wall -ansi -pedantic  
EXEC = cq

all: cq.o queue.o teller.o mutexes.o $(EXEC)

cq.o: cq.c cq.h queue.h teller.h mutexes.h
	gcc $(CFLAGS) -c cq.c -o cq.o

queue.o: queue.c queue.h teller.h mutexes.h cq.h
	gcc $(CFLAGS) -c queue.c -o queue.o

teller.o: teller.c teller.h queue.h mutexes.h cq.h
	gcc $(CFLAGS) -c teller.c -o teller.o

mutexes.o: mutexes.c mutexes.h
	gcc $(CFLAGS) -c mutexes.c -o mutexes.o

$(EXEC): cq.o queue.o teller.o mutexes.o
	gcc $(CFLAGS) cq.o queue.o teller.o mutexes.o -o $(EXEC)

clean:
	rm -f $(EXEC) *.o

