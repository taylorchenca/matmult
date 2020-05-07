SRC := matmult_omp.c

default: matmult_omp

matmult: matmult.c
	gcc -O3 -Wall -Wextra -o $@ matmult.c

matmult_omp: $(SRC)
	gcc -O3 -Wall -Wextra -fopenmp -std=c99 -o $@ $(SRC)

clean:
	-rm -f matmult
	-rm -f matmult_omp
