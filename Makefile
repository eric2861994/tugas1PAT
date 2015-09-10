FLAGS = -g -std=c++11
LIBS = -levent
TARGET_DIR = bin
SOURCE_DIR = src

build: webserver

clean:
	rm src/*.o

webserver: $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o $(SOURCE_DIR)/fileservice.o $(SOURCE_DIR)/file_mt.o
	g++ $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o $(SOURCE_DIR)/fileservice.o $(SOURCE_DIR)/file_mt.o $(LIBS) -o $(TARGET_DIR)/webserver

property: $(SOURCE_DIR)/property.o

utils: $(SOURCE_DIR)/utils.o

fileservice: $(SOURCE_DIR)/fileservice.o

filemt: $(SOURCE_DIR)/file_mt.o

$(SOURCE_DIR)/webserver.o: $(SOURCE_DIR)/webserver.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/webserver.cpp -o $(SOURCE_DIR)/webserver.o

$(SOURCE_DIR)/property.o: $(SOURCE_DIR)/property.h $(SOURCE_DIR)/property.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/property.cpp -o $(SOURCE_DIR)/property.o

$(SOURCE_DIR)/utils.o: $(SOURCE_DIR)/utils.h $(SOURCE_DIR)/utils.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/utils.cpp -o $(SOURCE_DIR)/utils.o

$(SOURCE_DIR)/fileservice.o: $(SOURCE_DIR)/fileservice.h $(SOURCE_DIR)/fileservice.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/fileservice.cpp -o $(SOURCE_DIR)/fileservice.o

$(SOURCE_DIR)/file_mt.o: $(SOURCE_DIR)/file_mt.h $(SOURCE_DIR)/file_mt.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/file_mt.cpp -o $(SOURCE_DIR)/file_mt.o
