# Compiler and flags
CXX = g++ 
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 
PKG_CONFIG = pkg-config
GTKMM_CFLAGS = $(shell $(PKG_CONFIG) gtkmm-4.0 --cflags gtkmm-4.0)
GTKMM_LIBS = $(shell $(PKG_CONFIG) gtkmm-4.0 --libs gtkmm-4.0)

# Automatically gather all header paths under src/private/**/(headers|include)
INCLUDE_DIRS = $(shell find src/private -type d \( -name headers -o -name include \))
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))
CXXFLAGS += $(INCLUDES) 
CXXFLAGS += $(GTKMM_LIBS) 

# Source and object files
SRCS = $(shell find src/private -name '*.cpp')
OBJS = $(patsubst src/private/%.cpp, src/private/%.o, $(SRCS))

# Output binary
TARGET = main

# Linker flags
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lGL
LDFLAGS += $(GTKMM_LIBS)

# Default rule
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile each .cpp to .o in corresponding directory
src/private/%.o: src/private/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find src/private -name '*.o' -delete
	rm -f $(TARGET)



runv: $(TARGET)
	valgrind \
	--leak-check=full \
	--show-leak-kinds=reachable \
	--track-origins=yes \
	--log-file=valgrind.log \
   ./$(TARGET)

run: $(TARGET)
	./$(TARGET)

#valgrind --leak-check=full --track-origins=yes 