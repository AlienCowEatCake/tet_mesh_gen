CXX ?= g++
CXXFLAGS_EXTRA ?= -Wall -Wextra -ansi -pedantic -pipe
CXXFLAGS_OPTIMIZE ?= -O3 -DNDEBUG
LDFLAGS_EXTRA ?= -s
EXECUTABLE = tet_mesh_gen

LINK.o = $(LINK.cc)
CXXFLAGS += $(CXXFLAGS_EXTRA) $(CXXFLAGS_OPTIMIZE)
LDFLAGS += $(LDFLAGS_EXTRA)

SOURCES = src/main.cpp src/mesh_generator.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

.PHONY: clean distclean

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(EXECUTABLE)

