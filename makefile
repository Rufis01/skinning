CPPFLAGS := -I./include
CFLAGS := -Wall -Wpedantic
CXXFLAGS := -Wall -Wpedantic -std=c++17 -Wno-unused-function -Wno-unused-variable
LDFLAGS := 
LDLIBS := -lGL -lGLEW -lglfw

SRC := ./src
OBJ := ./obj
BIN := ./bin
INC := ./include

all : main

#zip : 
#	tar -czf progetto.tar.gz makefile include src .vscode Relazione.pdf

clean : 
	rm -rf bin
	rm -rf obj

dir : 
	mkdir -p obj
	mkdir -p bin
      
log.o : $(SRC)/log.c\
        $(INC)/log.h\
        dir 
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SRC)/log.c -o $(OBJ)/$@
      
application.o : $(SRC)/application.cpp\
                $(INC)/application.hpp\
                dir 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC)/application.cpp -o $(OBJ)/$@
      
renderer.o : $(SRC)/renderer.cpp\
             $(INC)/renderer.hpp\
             dir 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC)/renderer.cpp -o $(OBJ)/$@

scene.o : $(SRC)/scene.cpp\
             $(INC)/scene.hpp\
             dir 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(SRC)/scene.cpp -o $(OBJ)/$@

main : $(SRC)/main.cpp\
       log.o application.o scene.o dir 
	$(CXX) $(LDFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o $(BIN)/$@ $(LDLIBS) $(SRC)/main.cpp $(OBJ)/log.o $(OBJ)/scene.o $(OBJ)/application.o


.PHONY: all dir zip clean
