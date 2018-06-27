all: marathon

SortedIntList.o: SortedIntList.c SortedIntList.h
	gcc -c SortedIntList.c

TwoWayListTree.o: TwoWayListTree.c TwoWayListTree.h
	gcc -c TwoWayListTree.c 

Viewer.o: Viewer.c Viewer.h
	gcc -c Viewer.c

main.o: main.c
	gcc -c main.c

marathon: SortedIntList.o TwoWayListTree.o Viewer.o main.o
	gcc -Wall -Wextra -O2 -o marathon SortedIntList.o TwoWayListTree.o Viewer.o main.o
