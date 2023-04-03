CXX = g++
CXXFLAGS = -std=c++11 -Wall -O2 -I./home/g83wang/polycube-SL/server/Crow/include -Iinclude
LDFLAGS = -larchive -lpthread -lboost_system

SRC = server.cc
OBJ = $(SRC:.cc=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(TARGET)
