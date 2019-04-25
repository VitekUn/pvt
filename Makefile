dgemv: main.o hpctimer.o
	gcc -fopenmp -o dgemv main.o hpctimer.o

main.o: main.c
	gcc -c -fopenmp main.c

hpctimer.o: hpctimer.c
	gcc -c hpctimer.c

clean:
	rm -f *.o main

