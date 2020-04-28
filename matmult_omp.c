/* assert */
#include <assert.h>

/* errno */
#include <errno.h>

/* fopen, fscanf, fprintf, fclose */
#include <stdio.h>

/* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */
#include <stdlib.h>

#include <time.h>
static int create_mat(size_t const nrows, size_t const ncols, double ** const matp)
{
    double * mat=NULL;
    if (!(mat=malloc(nrows*ncols*sizeof(*mat)))) {
        goto cleanup;
    }
    srand(time(NULL));
    /** Initialize matrix with random values **/
    size_t i, j;
    for (i=0; i<nrows; ++i) {
        for (j=0; j<ncols; ++j) {
            mat[i*nrows+j] = (double)rand();
        }
    }
    /** End random initialization **/

    *matp = mat;

    return 0;

    cleanup:
    free(mat);
    return -1;
}

int alg_matmul2D(int m, int n, int p, double** a, double** b, double** c)
{
    int i,j,k;
#pragma omp parallel shared(a,b,c) private(i,j,k)
    {
#pragma omp for  schedule(static)
        for (i=0; i<m; i=i+1){
            for (j=0; j<n; j=j+1){
                a[i][j]=0.;
                for (k=0; k<p; k=k+1){
                    a[i][j]=(a[i][j])+((b[i][k])*(c[k][j]));
                }
            }
        }
    }
    return 0;
}



int main(int argc, char * argv[])
{
    // size_t stored an unsigned integer
    size_t nrows, ncols, ncols2;
    double * A=NULL, * B=NULL, * C=NULL;

    if (argc != 4) {
        fprintf(stderr, "usage: matmult_omp nrows ncols ncols2\n");
        goto failure;
    }

    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);
    ncols2 = atoi(argv[3]);

    if (create_mat(nrows, ncols, &A)) {
        perror("error");
        goto failure;
    }

    if (create_mat(ncols, ncols2, &B)) {
        perror("error");
        goto failure;
    }

    clock_t start = clock();
    if (alg_matmul2D(nrows, ncols, ncols2, A, B, &C)) {
        perror("error");
        goto failure;
    }

    clock_t elapsed = clock() - start;
    size_t data_size = nrows * ncols + ncols * ncols2;
//  printf("data_size, elapsed(microsecond)\n");
    printf("%zu, %zu, %zu, %zu, %lu\n", nrows, ncols, ncols2, data_size, elapsed);

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
