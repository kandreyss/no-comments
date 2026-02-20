CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -O3

TARGET = no-comments
SRC = no-comments.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) $(TARGET).exe *.o *.tmp
