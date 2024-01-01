buildMain:
	gcc cZero.c -o cZeroOut worldToScreen.o MapConversion.o inputHandler.o linedrawing.o -lncurses -lm -lc -lpthread
worldToScreen:
	gcc -c worldToScreen.c 
mapConversion:
	gcc -c MapConversion.c
inputHandler:
	gcc -c inputHandler.c -lpthread

lineDrawer:
	gcc -c linedrawing.c
run:
	./cZeroOut
