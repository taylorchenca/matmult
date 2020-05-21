/* assert */
#include <assert.h>

#ifdef _OPENMP
/* omp_get_wtime */
#include <omp.h>
#else
#define omp_get_wtime() 0.0
#endif

/* printf, fopen, fclose, fscanf */
#include <stdio.h>

/* EXIT_SUCCESS, malloc, free */
#include <stdlib.h>

/* getopt */
#include <unistd.h>

/* bool */
#include <stdbool.h> 


#define ROWMJR(R,C,NR,NC) (R*NC+C)
#define COLMJR(R,C,NR,NC) (C*NR+R)
/* define access directions for matrices */
#define a(R,C) a[ROWMJR(R,C,n,m)]
#define b(R,C) b[ROWMJR(R,C,m,p)]
#define c(R,C) c[ROWMJR(R,C,n,p)]

#define MIN(X,Y) ((X)<(Y)?(X):(Y))



static int create_mat(size_t const nrows, size_t const ncols, double ** const matp)
{
    double * mat=NULL;
    if (!(mat=malloc(nrows*ncols*sizeof(*mat)))) {
        goto cleanup;
    }

    /** Initialize matrix with random values **/
    for(size_t i = 0; i < nrows; i++){
        for (size_t j = 0; j < ncols; j++){
            mat[(i * ncols) + j] = (double)(rand() % 1000) / 353.0;
        }
    }
    /** End random initialization **/

    *matp = mat;

    return 0;

    cleanup:
    free(mat);
    return -1;
}


int
main(int argc, char * argv[])
{
  int opt, num_threads;
  double cv, ts, te, t1;
  size_t n, m, p, y, x, z;
  size_t i, j, k;
  size_t is, ie, js, je, ks, ke;
  double * a = NULL, * b = NULL, * c = NULL;
  bool compact = false;

  m = n = p = x = y = z = 1;
  num_threads = 1;
  while (-1 != (opt=getopt(argc, argv, "cm:n:p:x:y:z:t:"))) {
    switch (opt) {
      case 't':
      num_threads = atoi(optarg);
      break;

      case 'n':
      n = (size_t)atol(optarg);
      break;

      case 'm':
      m = (size_t)atol(optarg);
      break;

      case 'p':
      p = (size_t)atol(optarg);
      break;

      case 'x':
      x = (size_t)atol(optarg);
      break;

      case 'y':
      y = (size_t)atol(optarg);
      break;

      case 'z':
      z = (size_t)atol(optarg);
      break;

      case 'c':
      compact = true;
      break;

      default: /* '?' */
      fprintf(stderr, "Usage: %s [-m nrows] [-n ncols] [-p nrows2] "\
        "[-x x dim] [-y y dim] [-z z dim] [-t num_threads]\n", argv[0]);
      return EXIT_FAILURE;
    }
  }

  assert(optind == argc);

  /* Validate input. */
  assert(num_threads > 0);
  assert(n > 0);
  assert(m > 0);
  assert(p > 0);
  assert(x > 0);
  assert(y > 0);
  assert(z > 0);

  omp_set_num_threads(num_threads);

  if (create_mat(m, n, &a)) {
    perror("error");
    goto failure;
  }

  if (create_mat(n, p, &b)) {
    perror("error");
    goto failure;
  }


  c = calloc(n*p, sizeof(*c));
  assert(c);

  /* Fix-up input. */
  x = (x < m) ? x : m;
  y = (y < n) ? y : n;
  z = (z < p) ? z : p;

  if(compact){
    printf("%zu, %zu, %zu, %zu, %zu, %zu, %d, ", m, n, p, x, y, z, num_threads);
  } else {
    printf("A(%zu, %zu), B(%zu, %zu), a(%zu, %zu), b(%zu, %zu), t %d\n", m, n, n, p, x, y, y, z, num_threads);
  }

  ts = omp_get_wtime();
  for (is=0; is<n; is+=y) {
    ie = MIN(is+y, n);
    for (js=0; js<p; js+=z) {
      je = MIN(js+z, p);
      for (ks=0; ks<m; ks+=x) {
        ke = MIN(ks+x, m);
        #pragma omp parallel for private(j,k,cv)
        for (i=is; i<ie; ++i) {
          for (j=js; j<je; ++j) {
            cv = c(i,j);
            for (k=ks; k<ke; ++k) {
              cv += a(i,k) * b(k,j);
            }
            c(i,j) = cv;
          }
        }
      }
    }
  }
  te = omp_get_wtime();
  t1 = te-ts;

  if(compact){
    printf("%0.04fs\n", t1);
  } else {
    printf("Matrix search time: %0.04fs\n", t1);
  }

  /* Free remaining memory. */
  free(a);
  free(b);
  free(c);

  return EXIT_SUCCESS;

  failure:
  if(a){
    free(a);
  }
  if(b){
    free(b);
  }
  if(c){
    free(c);
  }
  return EXIT_FAILURE;

}
