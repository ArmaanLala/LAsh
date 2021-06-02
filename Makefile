lash: shell.o trim.o
	g++ -o lash shell.o trim.o
shell.o: shell.cpp lash.h
	g++ -c -pedantic-errors  -Wall -o shell.o shell.cpp
trim.o: trim.cpp lash.h
	g++ -c -pedantic-errors  -Wall -o trim.o trim.cpp