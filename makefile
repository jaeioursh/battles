FLAGS= -O3 -std=c++11
CC=g++

default:
	clear
	g++  ${FLAGS} game.cpp -o run.exe -lsfml-graphics -lsfml-window -lsfml-system
	./run.exe   

fast:
	clear
	g++  ${FLAGS} -ffast-math game.cpp -o run.exe -lsfml-graphics -lsfml-window -lsfml-system
	./run.exe   

debug:
	clear
	g++  ${FLAGS} -g test.cpp -o run.exe -lsfml-graphics -lsfml-window -lsfml-system
	gdb run.exe  

test:
	clear
	g++  ${FLAGS} -g test.cpp -o run.exe -lsfml-graphics -lsfml-window -lsfml-system
	./run.exe  
