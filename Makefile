FLAGS = -std=c++11
LIBS = -levent
TARGET_DIR = bin

build: webserver

webserver: src/webserver.o
	g++ src/webserver.o $(LIBS) -o $(TARGET_DIR)/webserver

src/webserver.o: src/webserver.cpp
	g++ $(FLAGS) -c src/webserver.cpp
