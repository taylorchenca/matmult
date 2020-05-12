#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

void matrix_matrix_mult_tile (
        double ** dst, double ** src1, double ** src2,
        int istart, int iend, int jstart, int jend, int kstart, int kend) {
    int i, j, k;
    for (i = istart; i <= iend; i++) {
        for (j = jstart; j <= jend; j++) {
            double curr = 0.0;
            for (k = kstart; k <= kend; k++) {
                curr += src1[i][k] * src2[k][j];
            } /* for k */
            #pragma omp atomic
            dst[i][j] = dst[i][j] + curr;
        } /* for j */
    } /* for i */
} /* matrix_matrix_mult_tile */

void matrix_matrix_mult_by_tiling (
        int nrows, int ncols, int ncols2,
        double ** dst, double ** src1, double ** src2,
        int itilesize, int jtilesize, int ktilesize, size_t thread_num) {
    int istart, iend, jstart, jend, kstart, kend;
    omp_set_num_threads(thread_num);
    #pragma omp parallel for collapse(3) schedule(static) private(istart, jstart, kstart, iend, jend, kend) shared(dst, src1, src2)
    for (istart = 0; istart < nrows; istart += itilesize) {
        for (jstart = 0; jstart < ncols2; jstart += jtilesize) {
            for (kstart = 0; kstart < ncols; kstart += ktilesize) {
                iend = istart + itilesize - 1;
                if (iend >= nrows) iend = nrows - 1;
                jend = jstart + jtilesize - 1;
                if (jend >= ncols2) jend = ncols2 - 1;
                kend = kstart + ktilesize - 1;
                if (kend >= ncols) kend = ncols - 1;
                matrix_matrix_mult_tile(dst, src1, src2, istart, iend, jstart, jend, kstart, kend);
            }
        }
    }
}

static const double min = -10;
static const double max = 10;
void initialize_mat(size_t const nrows, size_t const ncols, double ** mat) {
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            double val = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
            mat[i][j] = val;
        }
    }
}

void set_zeros(size_t const nrows, size_t const ncols, double ** mat) {
    size_t i = 0;
    size_t j = 0;
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            mat[i][j] = 0.0;
        }
    }
}

void print_2d_mat(size_t const nrows, size_t const ncols, double ** mat) {
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

    if (argc != 6) {
        fprintf(stderr, "argc: %d", argc);
        fprintf(stderr, "usage: matmult_omp nrows ncols ncols2 tile_size thread_num\n");
        return -1;
    }

    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);
    ncols2 = atoi(argv[3]);
    tile_size = atoi(argv[4]);
    thread_num = atoi(argv[5]);
    size_t i = 0;
    double ** A = (double **)malloc(nrows * sizeof(double *));
    for (i = 0; i < nrows; i++) {
        A[i] = (double *)malloc(ncols * sizeof(double));
    }
    double ** B = (double **)malloc(ncols * sizeof(double *));
    for (i = 0; i < ncols; i++) {
        B[i] = (double *)malloc(ncols2 * sizeof(double));
    }
    double ** C = (double **)malloc(nrows * sizeof(double *));
    for (i = 0; i < nrows; i++) {
        C[i] = (double *)malloc(ncols2 * sizeof(double));
    }

    initialize_mat(nrows, ncols, A);
    initialize_mat(ncols, ncols2, B);
    set_zeros(nrows, ncols2, C);

    printf("A\n");
    print_2d_mat(nrows, ncols, A);
    printf("B\n");
    print_2d_mat(ncols, ncols2, B);

    struct timeval start, end;
    gettimeofday(&start,NULL);
    matrix_matrix_mult_by_tiling(nrows, ncols, ncols2, C, A, B, tile_size, tile_size, tile_size, thread_num);
    gettimeofday(&end,NULL);
    int elapsed = ((end.tv_sec - start.tv_sec) * 1000000) + (end.tv_usec - start.tv_usec);
    size_t data_size = nrows * ncols + ncols * ncols2;
    printf("%zu, %zu, %zu, %zu, %zu, %zu, %d\n", nrows, ncols, ncols2, data_size, thread_num, tile_size, elapsed);
    printf("C\n");
    print_2d_mat(nrows, ncols2, C);
    for (i = 0; i < nrows; i++) {
        free(A[i]);
    }
    for (i = 0; i < ncols; i++) {
        free(B[i]);
    }
    for (i = 0; i < nrows; i++) {
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return EXIT_SUCCESS;
}
