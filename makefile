CC = g++
CFLAGS = -g -Wall -std=c++11
TARGET = emu

CPLUSPLUSSRC = main.cpp \
               operations.cpp

SRC = $(CPLUSPLUSSRC)
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	$(RM) $(TARGET)
