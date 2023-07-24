CPPFLAGS = /I ./include /I ../include /D DISABLE_LOG /D GLEW_STATIC
CXXFLAGS = /std:c++20 /O2
LDFLAGS = 
LDLIBS = opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib "./lib/x64/glew32s.lib" "./lib/x64/glfw3_mt.lib"

SRC = ../src
OBJ = ./obj
BIN = ./bin
INC = ../include

TARGET = skinning.exe
OBJECTS = application.o\
main.o\
sceneloader.o\
renderer/renderer.o\
renderer/shader.o\
scene/camera.o\
scene/node.o\
scene/scene.o\
scene/skin.o\
scene/v_attachable.o\
scene/v_node_observer.o\
scene/animation/animation.o\
scene/animation/quatAnimation.o\
scene/animation/vec3Animation.o\
scene/mesh/mesh.o\
scene/mesh/primitive.o\
scene/mesh/texture.o


all: $(TARGET)

$(OBJECTS): $(SRC)/$*.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) /c $(SRC)/$*.cpp /Fo$*.o

$(TARGET): $(OBJECTS)
		$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDLIBS) $** /Fe:$(BIN)/$* /link $(LDFLAGS)

clean:
	rm $(TARGET) $(OBJECTS)

.PHONY: all clean
