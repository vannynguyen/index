CC= gcc
CFLAGS= -Wall -std=c11 -pedantic -g


indexer: indexer.o hashtable.o web.o list.o
	$(CC) $(CFLAGS) -o indexer indexer.o hashtable.o web.o list.o -lcurl -lm

indexer.o: indexer.c common.h hashtable.h list.h utils.h web.h
	$(CC) $(CFLAGS) -c indexer.c

hashtable.o: hashtable.c hashtable.h
	$(CC) $(CFLAGS) -c hashtable.c

web.o: web.c web.h
	$(CC) $(CFLAGS) -c web.c

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c




clean:
	rm -f *~
	rm -f *#
	rm -f ./indexer
	rm -f *.o

