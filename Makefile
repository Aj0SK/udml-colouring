all:
	g++ -std=c++17 -Wall tester.cpp -lcryptominisat5 && ./a.out < zadanie/cubic.txt
