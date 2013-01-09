CC 	= gcc
CFLAGS	= -ansi -pedantic -std=c99 -Wall
PROGRAM = program

$(PROGRAM): $(OBJS)
	$(CC) -c dijkstra.c -o dijkstra.o
	$(CC) -c jimp.c -o jimp.o
	$(CC) -c wczytywanie.c -o wczytywanie.o
	$(CC) -c pomocnicze.c -o pomocnicze.o
	$(CC) jimp.o dijkstra.o wczytywanie.o pomocnicze.o -o $(PROGRAM)
	
run:
	./program dane

clean:
	rm -fr *.o
	rm -fr $(PROGRAM)
	

