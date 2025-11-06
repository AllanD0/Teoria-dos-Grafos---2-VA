CC = gcc
CFLAGS = -O2 -std=c11 -Wall
TARGET = dijkstra

all: $(TARGET)

$(TARGET): Dijkstra.c
	$(CC) $(CFLAGS) Dijkstra.c -o $(TARGET).exe

clean:
	del /Q $(TARGET).exe 2>nul || rm -f $(TARGET).exe

.PHONY: all clean
