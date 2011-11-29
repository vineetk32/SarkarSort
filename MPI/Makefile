CFLAGS= -Wall -ggdb
LIBS= -lpthread

BINDIR=bin/

all:    clean splitter chunker sarkarSort simplySarkar threadedBucketSarkar

splitter:
	gcc splitter.c -o ${BINDIR}splitter ${CFLAGS} ${LIBS}

chunker:
	gcc chunker.c -o ${BINDIR}chunker ${CFLAGS} ${LIBS}

sarkarSort:
	gcc sarkarSort.c -o ${BINDIR}sarkarSort ${CFLAGS} ${LIBS}

simplySarkar:
	gcc simplySarkar.c -o ${BINDIR}simplySarkar ${CFLAGS} ${LIBS}

threadedBucketSarkar:
	gcc threadedBucketSarkar.c -o ${BINDIR}threadedBucketSarkar ${CFLAGS} ${LIBS}

clean:
	rm -Rf filechunk_* fileoffsets_* bin/splitter bin/chunker bin/sarkarSort bin/simplySarkar bin/threadedBucketSarkar
