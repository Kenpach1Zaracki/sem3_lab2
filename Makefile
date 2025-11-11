CC = gcc
CXX = g++
CXXFLAGS = -Wall -g -std=c++17

TARGET = lab2

OBJS = main.o zad1.o zad2.o zad3.o zad4.o zad5.o zad6.o zad7.o \
       stack.o array.o hash_simple.o set.o map_simple.o \
       bst.o lfu.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp zad1.h zad2.h zad3.h zad4.h zad5.h zad6.h zad7.h
	$(CXX) $(CXXFLAGS) -c main.cpp

zad1.o: zad1.cpp zad1.h stack.h
	$(CXX) $(CXXFLAGS) -c zad1.cpp

zad2.o: zad2.cpp zad2.h set.h map_simple.h
	$(CXX) $(CXXFLAGS) -c zad2.cpp

zad3.o: zad3.cpp zad3.h set.h
	$(CXX) $(CXXFLAGS) -c zad3.cpp

zad4.o: zad4.cpp zad4.h
	$(CXX) $(CXXFLAGS) -c zad4.cpp

zad5.o: zad5.cpp zad5.h bst.h
	$(CXX) $(CXXFLAGS) -c zad5.cpp

zad6.o: zad6.cpp zad6.h
	$(CXX) $(CXXFLAGS) -c zad6.cpp

zad7.o: zad7.cpp zad7.h lfu.h
	$(CXX) $(CXXFLAGS) -c zad7.cpp

stack.o: stack.cpp stack.h structures.h
	$(CXX) $(CXXFLAGS) -c stack.cpp

array.o: array.cpp array.h
	$(CXX) $(CXXFLAGS) -c array.cpp

hash_simple.o: hash_simple.cpp hash_simple.h
	$(CXX) $(CXXFLAGS) -c hash_simple.cpp

set.o: set.cpp set.h hash_simple.h
	$(CXX) $(CXXFLAGS) -c set.cpp

map_simple.o: map_simple.cpp map_simple.h set.h
	$(CXX) $(CXXFLAGS) -c map_simple.cpp

bst.o: bst.cpp bst.h
	$(CXX) $(CXXFLAGS) -c bst.cpp

lfu.o: lfu.cpp lfu.h
	$(CXX) $(CXXFLAGS) -c lfu.cpp

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
