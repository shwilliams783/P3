main: main.o scanner.o parser.o testtree.o statsem.o
        g++ main.o scanner.o parser.o testtree.o statsem.o -o statSem

main.o: main.c token.h node.h
        g++ -c main.c

scanner.o: scanner.c scanner.h token.h
        g++ -c scanner.c

parser.o: parser.c parser.h token.h node.h
        g++ -c parser.c

testtree.o: testtree.c testtree.h token.h node.h
        g++ -c testtree.c

statsem.o: statsem.c statsem.h token.h node.h
        g++ -c statsem.c

clean:
        rm *.o statSem
#github probably ruined the tabs, so make sure to replace them.
