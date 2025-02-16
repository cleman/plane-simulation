# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I/usr/include/eigen3
TARGET = main
FILES = dynamique.cpp plane.cpp

# Règles
all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp $(FILES)

clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)