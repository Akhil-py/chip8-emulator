# Compiler and flags
CC = g++
CFLAGS = -I/usr/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lSDL2
SRCS = Window.cpp chip8.cpp
OUT = chip8

# Default target
all: $(OUT)

# Build target
$(OUT): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean target
clean:
	rm -f $(OUT)
