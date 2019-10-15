SRC=./../
LODE_PNG_PATH=./../lodepng/
BIN=./../
OBJ=./../obj/
CC=g++ --std=c++0x
CFLAG=-g -c -Wall
IFLAG=-I
OFLAG=-o
LFLAG=	-lGL -lGLU -lglut

all: directory $(BIN)snake

directory:
	mkdir -p $(BIN)
	mkdir -p $(OBJ)

# link object files
$(BIN)snake: $(OBJ)lodepng.o $(OBJ)main.o
	$(CC) $(OFLAG) $(BIN)snake $(OBJ)lodepng.o $(OBJ)main.o $(LFLAG)

# create object file for lode_png
$(OBJ)lodepng.o: $(LODE_PNG_PATH)lodepng.cpp $(LODE_PNG_PATH)lodepng.h 
	$(CC) $(CFLAG) $(LODE_PNG_PATH)lodepng.cpp $(IFLAG) $(LODE_PNG_PATH)
	mv lodepng.o $(OBJ)

# create object file for main
$(OBJ)main.o: $(SRC)main.cpp $(LODE_PNG_PATH)lodepng.h
	$(CC) $(LFLAG) $(CFLAG) $(SRC)main.cpp $(IFLAG) $(LODE_PNG_PATH)
	mv main.o $(OBJ)

# remove object and binary files
clean:
	rm -rf $(OBJ)*.o $(BIN)snake ./../obj
