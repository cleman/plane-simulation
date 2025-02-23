# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++17 -I/usr/include/eigen3 -I./Crow/include  # Ajouter l'inclusion de Crow
LDFLAGS = -L./Crow/lib -lpthread  # Ajouter les liens vers la bibliothèque Crow (si nécessaire)
TARGET = main
FILES = dynamique.cpp plane.cpp server.cpp simulation.cpp controlleurs.cpp

# Règles
all: $(TARGET)

$(TARGET): $(TARGET).cpp $(FILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(TARGET).cpp $(FILES) $(LDFLAGS)
clean:
	rm -f *.o $(TARGET)

run: $(TARGET)
	./$(TARGET)
