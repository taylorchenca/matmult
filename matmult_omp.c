#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

void parallel_matmult(size_t const n, size_t const m, size_t const p,
        size_t thread_num, size_t tile_size,
                  const double *A, const double *B, double ** const Cp)
{
    omp_set_num_threads(thread_num);

    size_t i, j, k, outer_i, outer_j;
    double curr;
    double * C=NULL;
    C=malloc(n*p*sizeof(*C));
    struct timeval start, end;
    gettimeofday(&start,NULL);
    #pragma omp parallel for collapse(2) schedule(static)
    for(outer_i = 0; outer_i < n; outer_i+= tile_size){
        for(outer_j = 0; outer_j < p; outer_j+= tile_size){
            for (i = outer_i; i < outer_i + tile_size; ++i) {
                for (j = outer_j; j < outer_j + tile_size; ++j) {
                    curr = C[i*p+j];
                    for (k = 0; k < m; ++k){
                        curr += A[i*m+k] * B[k*p+j];
                    }
                    C[i*p+j] = curr;
                }
            }
        }
    }
    gettimeofday(&end,NULL);
    int elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    size_t data_size = n * m + m * p;
    printf("%zu, %zu, %zu, %zu, %zu, %zu, %d\n", n, m, p, data_size, thread_num, tile_size, elapsed);
    *Cp = C;
}


static const double min = -10;
static const double max = 10;
static int create_mat(size_t const nrows, size_t const ncols, double ** const matp)
{
    double * mat=NULL;
    size_t allocation_size = nrows*ncols*sizeof(*mat);
    if (!(mat=malloc(nrows*ncols*sizeof(*mat)))) {
        goto cleanup;
    }
    srand(time(NULL));
    /** Initialize matrix with random values **/
    size_t i, j;
    for (i=0; i<nrows; ++i) {
        for (j=0; j<ncols; ++j) {
            //Generate random value between min and max
            double val = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
            mat[i*ncols+j] = val;
        }
    }
    /** End random initialization **/

    *matp = mat;

    return 0;

    cleanup:
    free(mat);
    return -1;
}

int main(int argc, char * argv[])
{
    // size_t stored an unsigned integer
    size_t nrows, ncols, ncols2, tile_size, thread_num;
    double * A=NULL, * B=NULL, * C=NULL;

    if (argc != 6) {
        fprintf(stderr, "argc: %d", argc);
        fprintf(stderr, "usage: matmult_omp nrows ncols ncols2 tile_size thread_num\n");
        goto failure;
    }

    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);
    ncols2 = atoi(argv[3]);
    tile_size = atoi(argv[4]);
    thread_num = atoi(argv[5]);

    if (create_mat(nrows, ncols, &A)) {
        perror("error");
        goto failure;
    }

    if (create_mat(ncols, ncols2, &B)) {
        perror("error");
        goto failure;
    }

    parallel_matmult(nrows, ncols, ncols2, tile_size, thread_num, A, B, &C);

    free(A);
    free(B);
    free(C);

    return EXIT_SUCCESS;

    failure:
    if(A){
        free(A);
    }
    if(B){
        free(B);
    }
    if(C){
        free(C);
    }
    return EXIT_FAILURE;
}
