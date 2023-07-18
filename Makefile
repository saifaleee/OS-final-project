CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++17
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -pthread -lcurses -lX11 -lpthread

SRCS = Code.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = game

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(EXEC)

code.o: code.cpp
	$(CC) $(CFLAGS) -c code.cpp -o code.o

clean:
	rm -f $(OBJS) $(EXEC)

