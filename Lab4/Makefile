bin/sequential: src/sequential.c bin
	gcc -Wall -fopenmp -lm src/sequential.c -o bin/sequential

bin/first: src/first.c bin
	gcc -Wall -fopenmp -lm src/first.c -o bin/first

bin/second: src/second.c bin
	gcc -Wall -fopenmp -lm src/second.c -o bin/second

bin/third: src/third.c bin
	gcc -Wall -fopenmp -lm src/third.c -o bin/third

bin/fourth: src/fourth.c bin
	gcc -Wall -fopenmp -lm src/fourth.c -o bin/fourth

bin/fifth: src/fifth.c bin
	gcc -Wall -fopenmp -lm src/fifth.c -o bin/fifth

bin:
	mkdir bin

clean:
	rm -rf bin