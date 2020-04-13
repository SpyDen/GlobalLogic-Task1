result: menu.o overlay.o creating.o
	gcc -o result menu.o overlay.o creating.o -lm
menu.o: menu.c menu.h
	gcc -c menu.c
overlay.o : overlay.c overlay.h
	gcc -c overlay.c
creating.o: creating.c creating.h
	gcc -c creating.c
clear: rm -rf *.o
