all: hello echoservert sharing cnt echo psum race

hello: hello.c csapp.o
	gcc hello.c csapp.o -o hello

echoservert: echoservert.c csapp.c
	gcc echoservert.c csapp.o -o echoservert

sharing: sharing.c csapp.o
	gcc sharing.c csapp.o -o sharing

cnt: cnt.c csapp.o
	gcc cnt.c csapp.o -o cnt

echo: echo.c csapp.o sbuf.o
	gcc echo.c csapp.o sbuf.o -o echo

psum: psum_mutex.c csapp.o
	gcc psum_mutex.c csapp.o -o psum

race: race.c csapp.o
	gcc race.c csapp.o -o race
sbuf.o: sbuf.c csapp.o
	gcc -c sbuf.c csapp.o -o sbuf.o -lpthread

csapp.o: csapp.c
	gcc -c csapp.c -o csapp.o -lpthread



clean:
	rm *.o hello echoservert sharing cnt echo