CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O2

TARGET = health_check
SRC = health_check.c

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o