# Makefile for compiling Jean-Mi Simulator natively
# Usage: make

CC = gcc
TARGET = jeu

# TMX library sources
TMX_SOURCES = src/tmx.c src/tmx_xml.c src/tmx_utils.c src/tmx_err.c src/tmx_mem.c src/tmx_hash.c

# Game module sources
GAME_SOURCES = main.c \
	src/game/core/game_loop.c \
	src/game/core/save_load.c \
	src/game/entities/player.c \
	src/game/entities/pnj.c \
	src/game/rendering/renderer.c \
	src/game/rendering/ui.c \
	src/game/world/map.c \
	src/game/world/interactions.c

# All sources
SOURCES = $(GAME_SOURCES) $(TMX_SOURCES)

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Compiler flags
CFLAGS = -Wall -O2 -I./src -I./src/game $(shell pkg-config --cflags libxml-2.0 sdl2 SDL2_image SDL2_ttf)

# Linker flags
LDFLAGS = $(shell pkg-config --libs libxml-2.0 sdl2 SDL2_image SDL2_ttf) -lz -lm

# Build target
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the game
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
