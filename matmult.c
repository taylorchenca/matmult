/* assert */
#include <assert.h>

/* errno */
#include <errno.h>

/* fopen, fscanf, fprintf, fclose */
#include <stdio.h>

/* EXIT_SUCCESS, EXIT_FAILURE, malloc, free */
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

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
            mat[i*nrows+j] = val;
        }
    }
  /** End random initialization **/

  *matp = mat;

  return 0;

  cleanup:
  free(mat);
  return -1;
}

static int mult_mat(size_t const n, size_t const m, size_t const p,
                    double const * const A, double const * const B,
                    double ** const Cp, bool measure)
{
  size_t i, j, k;
  double sum;
  double * C=NULL;

  if (!(C=malloc(n*p*sizeof(*C)))) {
    goto cleanup;
  }

    struct timeval start, end;
    gettimeofday(&start,NULL);
  for (i=0; i<n; ++i) {
    for (j=0; j<p; ++j) {
      for (k=0, sum=0.0; k<m; ++k) {
        sum += A[i*m+k] * B[k*p+j];
      }
      C[i*p+j] = sum;
    }
  }
    gettimeofday(&end,NULL);
    int elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    size_t data_size = n * m + m * p;
    if (measure) {
        printf("%zu, %zu, %zu, %zu, %d\n", n, m, p, data_size, elapsed);
    }

  *Cp = C;

  return 0;

  cleanup:
  free(C);

  /*failure:*/
  return -1;
}


int main(int argc, char * argv[])
{
  // size_t stored an unsigned integer
  size_t nrows, ncols, ncols2;
  double * A=NULL, * B=NULL, * C=NULL;

  if (argc != 4) {
      fprintf(stderr, "argc: %d", argc);
    fprintf(stderr, "usage: matmult nrows ncols ncols2\n");
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

        if (mult_mat(nrows, ncols, ncols2, A, B, &C, true)) {
            perror("error");
            goto failure;
        }
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
