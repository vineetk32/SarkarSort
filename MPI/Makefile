CFLAGS= -Wall -g
LIBS= -lpthread

BINDIR=bin/

all:    clean splitter chotaSplitter chunker choteSarkar simplySarkar threadedBucketSarkar

splitter:
	gcc splitter.c -o ${BINDIR}splitter ${CFLAGS} ${LIBS}

chotaSplitter:
	gcc chotaSplitter.c -o ${BINDIR}chotaSplitter ${CFLAGS} ${LIBS}

chunker:
	gcc chunker.c -o ${BINDIR}chunker ${CFLAGS} ${LIBS}

choteSarkar:
	gcc choteSarkar.c -o ${BINDIR}choteSarkar ${CFLAGS} ${LIBS}

simplySarkar:
	gcc simplySarkar.c -o ${BINDIR}simplySarkar ${CFLAGS} ${LIBS}

threadedBucketSarkar:
	gcc threadedBucketSarkar.c -o ${BINDIR}threadedBucketSarkar ${CFLAGS} ${LIBS}

clean:
	rm -Rf filechunk_* fileoffsets_* bin/splitter bin/chotaSplitter bin/simplySarkar bin/choteSarkar bin/threadedBucketSarkar
