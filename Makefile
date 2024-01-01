compile: worldToScreen.o MapConversion.o inputHandler.o linedrawing.o
	gcc cZero.c -o cZeroOut worldToScreen.o MapConversion.o inputHandler.o linedrawing.o -lncurses -lm -lc -lpthread
	./cZeroOut
worldToScreen.o:
	gcc -c worldToScreen.c 
MapConversion.o:
	gcc -c MapConversion.c
inputHandler.o:
	gcc -c inputHandler.c -lpthread

linedrawing.o:
	gcc -c linedrawing.c
run:
	./cZeroOut
clean:
	rm worldToScreen.o
	rm MapConversion.o
	rm inputHandler.o
	rm linedrawing.o
	rm cZeroOut
