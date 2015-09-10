FLAGS = -g -std=c++11
LIBS = -levent
TARGET_DIR = bin
SOURCE_DIR = src

build: webserver

clean:
	rm src/*.o

webserver: $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o $(LIBS)
	g++ $(SOURCE_DIR)/webserver.o $(SOURCE_DIR)/property.o $(SOURCE_DIR)/utils.o $(LIBS) -o $(TARGET_DIR)/webserver

property: $(SOURCE_DIR)/property.o

utils: $(SOURCE_DIR)/utils.o

$(SOURCE_DIR)/webserver.o: $(SOURCE_DIR)/webserver.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/webserver.cpp -o $(SOURCE_DIR)/webserver.o

$(SOURCE_DIR)/property.o: $(SOURCE_DIR)/property.h $(SOURCE_DIR)/property.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/property.cpp -o $(SOURCE_DIR)/property.o

$(SOURCE_DIR)/utils.o: $(SOURCE_DIR)/utils.h $(SOURCE_DIR)/utils.cpp
	g++ $(FLAGS) -c $(SOURCE_DIR)/utils.cpp -o $(SOURCE_DIR)/utils.o
