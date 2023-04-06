CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra
LDFLAGS=-lcpprest -lboost_system -lpthread -lssl -lcrypto -larchive

all: server

server: server.o
	$(CXX) $(CXXFLAGS) -o server server.o $(LDFLAGS)

server.o: server.cc
	$(CXX) $(CXXFLAGS) -c server.cc -o server.o

clean:
	rm -f server server.o