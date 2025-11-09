CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

TARGET = lab2

SOURCES = main.cpp \
          stack.cpp \
          array.cpp \
          hash_simple.cpp \
          map_simple.cpp \
          set.cpp \
          huffman.cpp \
          lfu.cpp \
          zad1.cpp \
          zad2.cpp \
          zad3.cpp \
          zad4.cpp \
          zad5.cpp \
          zad6.cpp \
          zad7.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) *.gch

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
