SRC := matmult_omp.c

default: matmult_omp

matmult_serial: matmult_serial.cpp
	g++ -O3 -Wall -Wextra -o $@ matmult_serial.cpp

matmult: matmult.c
	gcc -Wall -Wextra -o $@ matmult.c
	#gcc -O3 -Wall -Wextra -o $@ matmult.c

matmult_omp: $(SRC)
	gcc -O3 -Wall -Wextra -fopenmp -o $@ $(SRC)

clean:
	-rm -f matmult
	-rm -f matmult_omp
