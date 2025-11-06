CC = gcc
CFLAGS = -O2 -std=c11 -Wall
TARGET = kosaraju

all: $(TARGET)

$(TARGET): Kosaraju.c
	$(CC) $(CFLAGS) Kosaraju.c -o $(TARGET).exe

clean:
	del /Q $(TARGET).exe 2>nul || rm -f $(TARGET).exe

.PHONY: all clean
