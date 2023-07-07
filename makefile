CPPFLAGS := -I./include
CFLAGS := -Wall -Wpedantic -g
CXXFLAGS := -Wall -Wpedantic -std=c++20 -g
LDFLAGS := 
LDLIBS := -lGL -lGLEW -lglfw ./lib/log.o

SRC := ./src
OBJ := ./obj
BIN := ./bin
INC := ./include

TARGET = $(BIN)/skinning
OBJECTS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(shell find . -name "*.cpp"))

all: $(TARGET)

# You don't even need to be explicit here,
# compiling C files is handled automagically by Make.
$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^ -o $@

$(TARGET): $(OBJECTS)
		$(CXX) $(LDFLAGS) $(CPPFLAGS) $(CXXFLAGS) -o $@ $(LDLIBS) $^

clean:
	rm $(TARGET) $(OBJECTS)

.PHONY: all clean
