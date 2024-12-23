# Compiler and flags
CC = g++
CFLAGS = -I/usr/include/SDL2 -D_REENTRANT
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lSDL2
SRCS = window.cpp
OUT = window

# Default target
all: $(OUT)

# Build target
$(OUT): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Clean target
clean:
	rm -f $(OUT)
