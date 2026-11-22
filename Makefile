CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -MMD -MP
EXEC = chess

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(OBJECTS:.o=.d)

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEPENDS)

.PHONY: clean

clean:
	rm -f $(OBJECTS) $(DEPENDS) $(EXEC)
