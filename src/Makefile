FLAGS = -std=c++11

build: webserver.o
	g++ webserver.o -o webserver

webserver.o: webserver.cpp
	g++ $(FLAGS) -c webserver.cpp