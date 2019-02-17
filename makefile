# COMPILE = g++ -std=c++17 -O2
COMPILE = g++ -std=c++17
NAME = test

all: 
	$(COMPILE) $(NAME).cpp -o bin/$(NAME)
	./bin/$(NAME)
