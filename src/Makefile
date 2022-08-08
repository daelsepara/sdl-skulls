CC = clang++
SKULLS_SOURCE = NecklaceOfSkulls.cpp
SKULLS_OUTPUT = NecklaceOfSkulls.exe
LINKER_FLAGS=-O3 -std=c++17 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES=-I/usr/include/SDL2

UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	LINKER_FLAGS += -stdlib=libc++
	INCLUDES += -I/usr/local/include/SDL2
else
	LINKER_FLAGS += -lstdc++fs
endif

all: clean skulls

skulls:
	$(CC) $(SKULLS_SOURCE) $(LINKER_FLAGS) $(INCLUDES) -o $(SKULLS_OUTPUT)

clean:
	rm -f *.exe *.o
