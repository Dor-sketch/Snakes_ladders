# Compiler and flags
CC = g++ -std=c++17
CFLAGS = -Wall -Wextra -g

# add cv2 library and qt5 library
CFLAGS += `pkg-config --cflags opencv`
CFLAGS += `pkg-config --cflags Qt5Core`

# Source files
SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

# Target executable
TARGET = music_parser

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)
