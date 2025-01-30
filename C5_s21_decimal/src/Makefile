CC=					gcc -g
CTFLAGS=				-std=c11
CFLAGS=				-Wall -Wextra -Werror -std=c11 -pedantic
GCOVFLAGS=			-fprofile-arcs -ftest-coverage
SOURSES_DECIMAL=		s21*.c s21*.h
OBJECT_DECIMAL=		$(patsubst %*.c, %*.o, $(SOURSES_DECIMAL))
SOURSES_TEST=		Tests/*.c 
OS=					$(shell uname -s)

ifeq ($(OS), Linux)
  CFLAGS+=			-D OS_LINUX
  CHECKFLAGS=			-lpthread -lcheck -pthread -lrt -lm -lsubunit
endif
ifeq ($(OS), Darwin)
  CFLAGS+=			-D OS_MAC
  CHECKFLAGS=		-lcheck
endif

all: clean s21_decimal.a

s21_decimal.a: $(SOURSES_DECIMAL)
	$(CC) $(CFLAGS) -c $(SOURSES_DECIMAL)
	@ar rcs s21_decimal.a $(OBJECT_DECIMAL)
	@rm -rf ./*.o

test: rebuild_lib $(SOURSES_TEST)
	$(CC) $(CTFLAGS) $(SOURSES_TEST) s21_decimal.a $(CHECKFLAGS) -o test
	./test

valgrind: test
	valgrind -s  --track-origins=yes --tool=memcheck --leak-check=yes ./test

gcov_report: rebuild_lib
	@gcc $(GCOVFLAGS) $(SOURSES_TEST) $(SOURSES_DECIMAL) $(CHECKFLAGS) -o GcovReport
	@./GcovReport
	@lcov -t "GcovReport" -o GcovReport.info -c -d .
	@genhtml -o report GcovReport.info
	@open ./report/index-sort-f.html
	@rm -rf ./*.gcno ./*gcda ./GcovReport.*

clang:
	@cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.c *.h
	@rm .clang-format

leaks: test
	leaks -atExit -- ./test

rebuild_lib: clean s21_decimal.a

clean:
	@rm -rf ./a.out ./*.o ./*.a ./test ./GcovReport ./report ./*.info ./*.gcno ./*gcda