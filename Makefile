all: Entregable2

Entregable2: Fifo.o Hijo.o Nieto.o ASlib.o 
	gcc -Wall -pedantic -g Fifo.o Hijo.o Nieto.o ASlib.o -o Entregable2

Fifo.o: Fifo.c Hijo.h Nieto.h ASlib.h
	gcc -Wall -pedantic -g -c Fifo.c

Hijo.o: Hijo.c Hijo.h ASlib.h
	gcc -Wall -pedantic -g -c Hijo.c

Nieto.o: Nieto.c Nieto.h ASlib.h
	gcc -Wall -pedantic -g -c Nieto.c

ASlib.o: ASlib.c ASlib.h 
	gcc -Wall -pedantic -g -c ASlib.c

clean:
	$(RM) Entregable2 *.o