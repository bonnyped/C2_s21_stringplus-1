CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -c
OS := $(shell uname)
TEST_FLAGS = $(shell pkg-config --cflags --libs check)

all: clean s21_string.a test gcov_report

test: test.o s21_sprintf_funcs.o s21_sprintf_parser.o s21_string.a
	$(CC) test.o s21_sprintf_funcs.o s21_sprintf_parser.o s21_string.a -o test -L. $(TEST_FLAGS)

test.o: test.c
	$(CC) $(CFLAGS) test.c -o test.o

test.c:
	checkmk clean_mode=1 include.file *.check > test.c

s21_string.a: s21_sprintf_funcs.o s21_sprintf_parser.o s21_string.o 
	ar rcs s21_string.a s21_sprintf_funcs.o s21_sprintf_parser.o s21_string.o -lm
	ranlib s21_string.a

s21_string.o: s21_string.c 
	$(CC) $(CFLAGS) s21_string.c -o s21_string.o

s21_sprintf_funcs.o: s21_sprintf_funcs.c
	$(CC) $(CFLAGS) s21_sprintf_funcs.c -o s21_sprintf_funcs.o -lm

s21_sprintf_parser.o: s21_sprintf_parser.c
	$(CC) $(CFLAGS) s21_sprintf_parser.c -o s21_sprintf_parser.o

clean:
	rm -rf *.o *.a *.out *.info *.log *.html *.gc* test gcov_report s21_string report test.c

gcov_report: test.c
	$(CC) -fprofile-arcs -ftest-coverage test.c s21_string.c s21_sprintf_funcs.c s21_sprintf_parser.c -o gcov_report $(TEST_FLAGS)
	./gcov_report
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info

open:
ifeq ($(OS),Darwin)
	open -a "Google Chrome" report/src/index.html
else
	xdg-open report/src/index.html
endif

clang:
	clang-format -n -style=google *.c *.h

clangI:
	clang-format -i -style=google *.c *.h

leak: test
ifeq ($(OS),Darwin)
	CK_FORK=no leaks -atExit -- ./test
else
	CK_FORK=no valgrind --leak-check=full -s --track-origins=yes --log-file=leak.log ./test
endif	

rebuild: all
