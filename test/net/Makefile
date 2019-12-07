hex2add: hex2add.c csapp.o
	gcc hex2add.c csapp.o -o hex2add

add2hex: add2hex.c csapp.o
	gcc add2hex.c csapp.o -o add2hex

hostinfo: hostinfo.c csapp.o
	gcc hostinfo.c csapp.o -o hostinfo

echoclient: echoclient.c csapp.o
	gcc echoclient.c csapp.o -o echoclient

echoserveri: echoserveri.c csapp.o
	gcc echoserveri.c csapp.o -o echoserveri

echoserverp: echoserverp.c csapp.o
	gcc echoserverp.c csapp.o -o echoserverp

echoservers: echoservers.c csapp.o
	gcc echoservers.c csapp.o -o echoservers

tiny: tiny.c adder.o csapp.o
	gcc tiny.c csapp.o -o tiny

adder.o:
	gcc ./cgi-bin/adder.c -o adder

csapp: csapp.c
	gcc csapp.c -o csapp.o

clean: 
	rm -r *.o
