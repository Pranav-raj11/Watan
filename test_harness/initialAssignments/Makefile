CXX = g++-14.2.0 -std=c++20
CXXFLAGS = -fmodules-ts -Wall -g -O0 -MMD  # Use -MMD to generate dependencies

# things to import
IMPORTS = memory iostream vector random fstream sstream stdexcept algorithm unordered_map string

# List all module partitions separately and include Models.cc first
MODELS_MODULE = ../../models/Models.cc


# Add concrete models
PARTITIONS_MODELS_HEADERS = ../../models/base/headers/Dice.cc \
                     ../../models/base/headers/GenericModel.cc \
                     ../../models/base/headers/Player.cc \
                     ../../models/base/headers/Objective.cc \
                     ../../models/concrete/headers/Criterion.cc \
                     ../../models/concrete/headers/Goal.cc \
                     ../../models/concrete/headers/HexTile.cc \
                     ../../models/concrete/headers/Board.cc \
                     ../../models/concrete/headers/FairDice.cc \
                     ../../models/concrete/headers/Student.cc \
                     ../../models/concrete/headers/WeightedDice.cc \
                     ../../models/concrete/headers/ModelFacade.cc



TYPES_MODULE = ../../types/Types.cc
RANDOMNESS_MODULE = ../../random/Randomness.cc ../../random/RandomGenerator-impl.cc

# Source and object files
SOURCES = $(RANDOMNESS_MODULE) $(TYPES_MODULE) $(PARTITIONS_MODELS_HEADERS)

# adds board views
SOURCES += ../../views/headers/GenericView.cc ../../views/headers/BoardView.cc ../../views/implementations/BoardView-impl.cc ../../views/headers/GameView.cc ../../views/implementations/GameView-impl.cc ../../views/ViewProxy.cc
# Adds the builder and state managers
SOURCES += ../../builders/GameBuilder.cc ../../builders/GameBuilder-impl.cc ../../managers/GenericManager.cc ../../managers/GenericManager-impl.cc ../../managers/headers/GameStateManager.cc ../../managers/implementations/GameStateManager-impl.cc

# adds manager facade, game controller
SOURCES += ../../managers/facade/ManagerFacade.cc ../../managers/facade/ManagerFacade-impl.cc ../../controller/GameController.cc ../../controller/GameController-impl.cc

SOURCES += suite.cc

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