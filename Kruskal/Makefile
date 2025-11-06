CC = gcc
CFLAGS = -O2 -std=c11 -Wall
TARGET = kruskal

all: $(TARGET)

$(TARGET): Kruskal.c
	$(CC) $(CFLAGS) Kruskal.c -o $(TARGET).exe

clean:
	del /Q $(TARGET).exe 2>nul || rm -f $(TARGET).exe

.PHONY: all clean
