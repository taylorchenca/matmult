#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

void matrix_matrix_mult_tile (
        int nr, int nc, int nq,
        double dst[][nq], double src1[][nc], double src2[][nq],
        int rstart, int rend, int cstart, int cend, int qstart, int qend) {
    int r, c, q;
    for (r = rstart; r <= rend; r++) {
        for (c = cstart; c <= cend; c++) {
            if (qstart == 0) dst[r][c] = 0.0;
            for (q = qstart; q <= qend; q++) {
                dst[r][c] = dst[r][c] + src1[r][q] * src2[q][c];
            } /* for q */
        } /* for c */
    } /* for r */
} /* matrix_matrix_mult_tile */

void matrix_matrix_mult_by_tiling (
         int nr, int nc, int nq,
         double dst[][nq], double src1[][nc], double src2[][nq],
            int rtilesize, int ctilesize, int qtilesize, size_t thread_num) {
    int rstart, rend, cstart, cend, qstart, qend;
    omp_set_num_threads(thread_num);
    #pragma omp parallel for collapse(3) schedule(static)
    for (rstart = 0; rstart < nr; rstart += rtilesize) {
        for (cstart = 0; cstart < nc; cstart += ctilesize) {
            for (qstart = 0; qstart < nq; qstart += qtilesize) {
                rend = rstart + rtilesize - 1;
                if (rend >= nr) rend = nr - 1;
                cend = cstart + ctilesize - 1;
                if (cend >= nc) cend = nc - 1;
                qend = qstart + qtilesize - 1;
                if (qend >= nq) qend = nq - 1;
                matrix_matrix_mult_tile(nr, nc, nq, dst, src1, src2, rstart, rend, cstart, cend, qstart, qend);
            }
        }
    }
}


void parallel_matmult(size_t const n, size_t const m, size_t const p,
        size_t thread_num, size_t tile_size,
                  const double *A, const double *B, double ** const Cp) {
    omp_set_num_threads(thread_num);
    size_t i, j, k, outer_i, outer_j;
    double curr;
    double * C=NULL;
    C=malloc(n*p*sizeof(*C));
    printf("tile_size: %ld\n", tile_size);
    struct timeval start, end;
    gettimeofday(&start,NULL);
    #pragma omp parallel for collapse(2) schedule(static)
    for(outer_i = 0; outer_i < n; outer_i+= tile_size) {
        for(outer_j = 0; outer_j < p; outer_j+= tile_size) {
            for (i = outer_i; i < outer_i + tile_size; ++i) {
                for (j = outer_j; j < outer_j + tile_size; ++j) {
                    printf("i: %ld j: %ld\n", i, j);
                    curr = C[i*p+j];
                    for (k = 0; k < m; ++k) {
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
static int create_mat(size_t const nrows, size_t const ncols, double ** const matp) {
    double * mat=NULL;
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

static int create_2d_mat(size_t const nrows, size_t const ncols, double *** const matp) {
    double * mat=NULL;
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

void initialize_mat(size_t const nrows, size_t const ncols, double mat[][ncols]) {
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            double val = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
            mat[i][j] = val;
        }
    }
}

void print_matrix(double * const A, size_t const n, size_t const m) {
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            printf("%f ", A[i*m+j]);
        }
        printf("\n");
    }
}

void print_2d_mat(size_t const nrows, size_t const ncols, double mat[][ncols]) {
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            printf("%f ", mat[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char * argv[]) {
    size_t nrows, ncols, ncols2, tile_size, thread_num;
//    double * A=NULL, * B=NULL, * C=NULL;

    if (argc != 6) {
        fprintf(stderr, "argc: %d", argc);
        fprintf(stderr, "usage: matmult_omp nrows ncols ncols2 tile_size thread_num\n");
//        goto failure;
    }

    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);
    ncols2 = atoi(argv[3]);
    tile_size = atoi(argv[4]);
    thread_num = atoi(argv[5]);
//    double A[nrows][ncols], B[ncols][ncols2], C[nrows][ncols2];
    double ** A = malloc(sizeof(double) * nrows * ncols);
    double ** B = malloc(sizeof(double) * ncols * ncols2);
    double ** C = malloc(sizeof(double) * nrows * ncols2);
    initialize_mat(nrows, ncols, A);
    initialize_mat(ncols, ncols2, B);

//    if (create_mat(nrows, ncols, &A)) {
//        perror("error");
//        goto failure;
//    }
//
//    if (create_mat(ncols, ncols2, &B)) {
//        perror("error");
//        goto failure;
//    }
//    parallel_matmult(nrows, ncols, ncols2, thread_num, tile_size, A, B, &C);
//    print_matrix(C, nrows, ncols2);

    struct timeval start, end;
    gettimeofday(&start,NULL);
    matrix_matrix_mult_by_tiling(nrows, ncols, ncols2, C, A, B, tile_size, tile_size, tile_size, thread_num);
    gettimeofday(&end,NULL);
    int elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    size_t data_size = nrows * ncols + ncols * ncols2;
    printf("%zu, %zu, %zu, %zu, %zu, %zu, %d\n", nrows, ncols, ncols2, data_size, thread_num, tile_size, elapsed);

//    free(A);
//    free(B);
//    free(C);

    return EXIT_SUCCESS;

//    failure:
//    if(A){
//        free(A);
//    }
//    if(B){
//        free(B);
//    }
//    if(C){
//        free(C);
//    }
//    return EXIT_FAILURE;
}
