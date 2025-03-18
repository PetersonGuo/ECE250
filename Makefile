all:
	g++ *.cpp -std=c++17

clean:
	rm -f a.out $(OBJ)
