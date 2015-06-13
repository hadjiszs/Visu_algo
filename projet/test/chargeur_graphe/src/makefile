all: testfile

testfile: file.o main_test.o
	gcc -Wall -Werror -Wextra -g $^ -o $@

file.o: file.c file.h
	gcc -c -Wall -Werror -Wextra -g $< -o $@

main_test.o: main_test.c file.h
	gcc -c -Wall -Werror -Wextra -g $< -o $@

clean:
	rm -rf testfile
	
mrproper: clean
	rm -rf *.o

re: mrproper all
	