CC = g++
CFLAGS = -Wall -Wextra -std=c++11
LIBS = -lcpprest -lboost_system -lssl -lcrypto -larchive -lcurl -lpthread

SRCS = upload.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = upload

.PHONY: clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
