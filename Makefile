all:	tabulka

tabulka: vysledok
	g++ -std=c++17 -O2 -Wall tabulka.cpp -lcryptominisat5 && ./a.out < vystupy/out.txt > vystupy/tabulka.txt

vysledok: vystupy
	g++ -std=c++17 -pthread -O2 -Wall tester.cpp -lcryptominisat5 && ./a.out < vstupy/cubic.txt > vystupy/out.txt

vystupy:
	mkdir vystupy
