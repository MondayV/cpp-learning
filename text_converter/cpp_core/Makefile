CXX := g++
CXXFLAGS := -std=c++17 -O3 -Wall
TARGET := text_transform

all: $(TARGET)

$(TARGET): text_transform.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^
	
clean:
	rm -f $(TARGET)
	
.PHONY: all clean