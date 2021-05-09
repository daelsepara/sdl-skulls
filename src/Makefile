CC = clang++
SKULLS_SOURCE = skulls.cpp
SKULLS_OUTPUT = skulls.exe
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -O3
INCLUDES = -I/usr/include/SDL2

all: clean skulls

skulls:
	$(CC) $(SKULLS_SOURCE) $(LINKER_FLAGS) $(INCLUDES) -o $(SKULLS_OUTPUT)

clean:
	rm -f *.exe *.o
