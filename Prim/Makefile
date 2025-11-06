CC = gcc
CFLAGS = -Wall -g -std=c99

TARGET = prim
SRCS = Prim.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o
