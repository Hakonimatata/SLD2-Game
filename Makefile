# Definer kildefilene
SOURCES = main.cpp function.cpp Game.cpp
# Definer objektfilene
OBJECTS = $(SOURCES:.cpp=.o)
# Definer navnet på den endelige kjørbare filen
TARGET = main

# Standard mål for make
all: $(TARGET)

# Regel for å lage den endelige kjørbare filen
$(TARGET): $(OBJECTS)
	g++ -o $@ $(OBJECTS) -L src/lib -lmingw32 -lSDL2main -lSDL2

# Regel for å kompilere .cpp til .o
%.o: %.cpp
	g++ -c -I src/include $< -o $@

# Regel for å rydde opp midlertidige filer
clean:
	del /Q $(OBJECTS) $(TARGET)