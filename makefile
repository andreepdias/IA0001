CC = g++
FILES = ACO.cpp
LINKS = -Wall -lsfml-graphics -lsfml-window -lsfml-system -std=c++11 -O2

all:
	$(CC) -o s $(FILES) $(LINKS) -o s && ./s
