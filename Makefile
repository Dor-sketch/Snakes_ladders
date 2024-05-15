# Variables
CXX = g++
CXXFLAGS = `pkg-config --cflags gtk+-3.0` -std=c++17 -I/usr/local/include/cairomm-1.0 -I/usr/local/include/cairomm-1.0/cairomm -I/usr/local/include/sigc++-2.0 -I/usr/local/lib/sigc++-2.0/include
ifdef USE_MIDIFILE
CXXFLAGS += -DUSE_MIDIFILE
endif
LIBS = `pkg-config --libs gtk+-3.0` -lsfml-audio -lsfml-system -lrtmidi
OBJS = main.o snakes.o game.o tm.o sound.o
# add midifile library
LIBS += -Lmidifile/lib -lmidifile
# Targets
all: app

app: $(OBJS)
	$(CXX) -o $@ $^ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o app

.PHONY: all clean