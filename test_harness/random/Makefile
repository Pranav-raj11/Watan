CXX = g++ -std=c++20
CXXFLAGS = -fmodules-ts -Wall -g -O0 -MMD  # Use -MMD to generate dependencies

# things to import
IMPORTS = iostream random sstream string

# Source and object files
SOURCES = ../../random/Randomness.cc ../../random/RandomGenerator-impl.cc random.cc
OBJECTS = $(SOURCES:.cc=.o)  
DEPENDS = $(OBJECTS:.o=.d)   
EXEC = exec            

# default when make is called
# import first then link / build the source files
all: imports $(EXEC)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)


%.o: %.cc
	$(CXX) -c $(CXXFLAGS) -o $@ $<

gcm.cache/%.gcm:
	$(CXX) -fmodules-ts -c -x c++-system-header $*


imports: $(IMPORTS:%=gcm.cache/%.gcm)

-include $(DEPENDS)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)
	rm -rf gcm.cache
