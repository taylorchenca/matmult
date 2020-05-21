SRC := matmult_omp.c
REFSRC := matmult_omp_ref.c

default: matmult_omp

matmult: matmult.c
	gcc -O3 -Wall -Wextra -o $@ matmult.c

matmult_omp: $(SRC)
	gcc -O3 -Wall -Wextra -fopenmp -o $@ $(SRC)

matmult_omp_ref: $(REFSRC)
	gcc -O3 -Wall -Wextra -fopenmp -o $@ $(REFSRC)

clean:
	-rm -f matmult
	-rm -f matmult_omp
