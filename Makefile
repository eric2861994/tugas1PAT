FLAGS = -g -std=c++11
LIBS = -levent
TARGET_DIR = bin
SOURCE_DIR = src

build: webserver

webserver: $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o
	g++ $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o $(LIBS) -o $(TARGET_DIR)/webserver

$(SOURCE_DIR)/webserver.o: $(SOURCE_DIR)/webserver.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/webserver.cpp -o $(SOURCE_DIR)/webserver.o

property: $(SOURCE_DIR)/property.o

$(SOURCE_DIR)/property.o: $(SOURCE_DIR)/property.h $(SOURCE_DIR)/property.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/property.cpp -o $(SOURCE_DIR)/property.o

utils: $(SOURCE_DIR)/utils.o

$(SOURCE_DIR)/utils.o: $(SOURCE_DIR)/utils.h $(SOURCE_DIR)/utils.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/utils.cpp -o $(SOURCE_DIR)/utils.o

echoserver: $(SOURCE_DIR)/echoserver.o
	gcc $(SOURCE_DIR)/echoserver.o -levent -o bin/echoserver

$(SOURCE_DIR)/echoserver.o: $(SOURCE_DIR)/echoserver.c
	gcc -c $(SOURCE_DIR)/echoserver.c -o $(SOURCE_DIR)/echoserver.o
