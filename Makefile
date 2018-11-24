all:
	g++ -std=c++17 -O2 -Wall tester.cpp -lcryptominisat5 && ./a.out < vstupy/cubic.txt
