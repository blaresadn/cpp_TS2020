all: test

test: tests.o
	g++ -o test tests.o

tests.o: tests.cpp alloc.h
	g++ -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -pedantic -g -fsanitize=address -std=c++17 tests.cpp
