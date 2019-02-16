# COMPILE = g++ -std=c++17 -O2
COMPILE = g++ -std=c++17
NAME = main

all: 
	$(COMPILE) $(NAME).cpp -o bin/$(NAME)
	./bin/$(NAME)
