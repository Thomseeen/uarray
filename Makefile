all: test

test: test.c uarray.c
	gcc test.c uarray.c -g -o test -pthread

clean:
	rm -rf test
