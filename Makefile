FLAGS = -g -std=c++11
LIBS = -levent
TARGET_DIR = bin
SOURCE_DIR = src

build: webserver

clean:
	rm src/*.o

webserver: $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o
	g++ $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o $(LIBS) -o $(TARGET_DIR)/webserver

property: $(SOURCE_DIR)/property.o

utils: $(SOURCE_DIR)/utils.o

filemt: $(SOURCE_DIR)/file_mt.o

$(SOURCE_DIR)/webserver.o: $(SOURCE_DIR)/webserver.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/webserver.cpp -o $(SOURCE_DIR)/webserver.o

$(SOURCE_DIR)/property.o: $(SOURCE_DIR)/property.h $(SOURCE_DIR)/property.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/property.cpp -o $(SOURCE_DIR)/property.o

$(SOURCE_DIR)/utils.o: $(SOURCE_DIR)/utils.h $(SOURCE_DIR)/utils.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/utils.cpp -o $(SOURCE_DIR)/utils.o

$(SOURCE_DIR)/file_mt.o: $(SOURCE_DIR)/file_mt.h $(SOURCE_DIR)/file_mt.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/file_mt.cpp -o $(SOURCE_DIR)/file_mt.o

# experiments
echoserver: $(SOURCE_DIR)/echoserver.o
	gcc $(SOURCE_DIR)/echoserver.o -levent -o bin/echoserver

httpserver: $(SOURCE_DIR)/http.o
	gcc $(SOURCE_DIR)/http.o -levent -o bin/http

fileserver: $(SOURCE_DIR)/fileserver.o
	gcc $(SOURCE_DIR)/fileserver.o -levent -o bin/fileserver

$(SOURCE_DIR)/echoserver.o: $(SOURCE_DIR)/echoserver.c
	gcc -g -c $(SOURCE_DIR)/echoserver.c -o $(SOURCE_DIR)/echoserver.o

$(SOURCE_DIR)/http.o: $(SOURCE_DIR)/http.c
	gcc -g -c $(SOURCE_DIR)/http.c -o $(SOURCE_DIR)/http.o

$(SOURCE_DIR)/fileserver.o: $(SOURCE_DIR)/fileserver.c
	gcc -g -c $(SOURCE_DIR)/fileserver.c -o $(SOURCE_DIR)/fileserver.o
