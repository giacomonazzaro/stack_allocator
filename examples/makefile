COMPILE = g++ -std=c++14

test: 
	$(COMPILE) test.cpp -o bin/test
	bin/test

main: 
	$(COMPILE) main.cpp -o bin/main
	bin/main

clean:
	rm -rf ./bin

$(shell mkdir bin)