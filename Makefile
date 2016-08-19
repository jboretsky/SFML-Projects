LIBS=-lsfml-graphics -lsfml-window -lsfml-system
CC=g++
CPP_FILES=$(wildcard Source/*.cpp)
OBJ_FILES=$(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

sfml-app: $(OBJ_FILES)
	@echo "Linking..."
	$(CC) -std=c++11 -o $@ $^ $(LIBS)

obj/%.o: Source/%.cpp 
	@echo "Compiling..."
	$(CC) -std=c++11 -c -o $@ $<

clean:
	rm -rf **/*.o sfml-app
