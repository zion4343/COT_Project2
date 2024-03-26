align: align.cpp
	g++ -std=c++17 alignCopy.cpp -o align

test:
	./align

clean:
	rm align