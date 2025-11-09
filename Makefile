# MinGW64-focused Makefile for simple-opengl MD2 Loader
# Explicit targets for Windows development

LIBS = -L. \
	   -LC:\msys64\mingw64\lib \
	   -lglew32 \
	   -lglfw3 \
	   -lopengl32 \
	   -lgdi32

INCLUDES = -IC:\msys64\mingw64\include

WARNINGS = -Wall

FLAGS = -std=c++17 -DGLEW_STATIC -DGLM_ENABLE_EXPERIMENTAL -DGLM_FORCE_RADIANS

all: bin/main.exe

bin/main.exe: bin/ShaderProgram.o bin/Texture2D.o bin/TgaLoader.o bin/Md2.o bin/OpenGLHandler.o bin/main.o
	g++ bin/ShaderProgram.o bin/Texture2D.o bin/TgaLoader.o bin/Md2.o bin/OpenGLHandler.o bin/main.o $(LIBS) -o bin/main.exe $(WARNINGS) $(FLAGS)

bin/ShaderProgram.o: src/ShaderProgram.cpp src/ShaderProgram.h
	g++ -c src/ShaderProgram.cpp -o bin/ShaderProgram.o $(INCLUDES) $(WARNINGS) $(FLAGS)

bin/Texture2D.o: src/Texture2D.cpp src/Texture2D.h src/TgaLoader.h
	g++ -c src/Texture2D.cpp -o bin/Texture2D.o $(INCLUDES) $(WARNINGS) $(FLAGS)

bin/TgaLoader.o: src/TgaLoader.cpp src/TgaLoader.h
	g++ -c src/TgaLoader.cpp -o bin/TgaLoader.o $(INCLUDES) $(WARNINGS) $(FLAGS)

bin/Md2.o: src/Md2.cpp src/Md2.h src/ShaderProgram.h src/Texture2D.h
	g++ -c src/Md2.cpp -o bin/Md2.o $(INCLUDES) $(WARNINGS) $(FLAGS)

bin/OpenGLHandler.o: src/OpenGLHandler.cpp src/OpenGLHandler.h
	g++ -c src/OpenGLHandler.cpp -o bin/OpenGLHandler.o $(INCLUDES) $(WARNINGS) $(FLAGS)

bin/main.o: src/main.cpp src/OpenGLHandler.h src/Md2.h
	g++ -c src/main.cpp -o bin/main.o $(INCLUDES) $(WARNINGS) $(FLAGS)

clean:
	del bin\*.o bin\main.exe
