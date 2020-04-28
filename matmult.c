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

  failure:
  return -1;
}

static int mult_mat(size_t const n, size_t const m, size_t const p,
                    double const * const A, double const * const B,
                    double ** const Cp)
{
  size_t i, j, k;
  double sum;
  double * C=NULL;

  if (!(C=malloc(n*p*sizeof(*C)))) {
    goto cleanup;
  }

  for (i=0; i<n; ++i) {
    for (j=0; j<p; ++j) {
      for (k=0, sum=0.0; k<m; ++k) {
        sum += A[i*m+k] * B[k*p+j];
      }
      C[i*p+j] = sum;
    }
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

  clock_t start = clock();
  if (mult_mat(nrows, ncols, ncols2, A, B, &C)) {
    perror("error");
    goto failure;
  }

  clock_t elapsed = clock() - start;
  size_t data_size = nrows * ncols + ncols * ncols2;
//  printf("data_size, elapsed(microsecond)\n");
  printf("%d, %d, %d, %d, %d\n", nrows, ncols, ncols2, data_size, elapsed);

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
