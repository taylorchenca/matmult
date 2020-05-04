#include <iostream>
#include <chrono>
#include <vector>

void matmult_without_saving(int nrows, int ncols, int ncols2)
{
    int a[nrows][ncols], b[ncols][ncols2], c[nrows][ncols2];
    int sum = 0;
    for (int i = 0; i < nrows; i++) {
        for (int j = 0; j < ncols; j++) {
            a[i][j] = rand();
        }
    }

    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < ncols2; j++) {
            b[i][j] = rand();
        }
    }
    using namespace std::chrono;
    using microsecond = duration<float, microseconds::period>;
    auto start = high_resolution_clock::now();

    for (int i = 0; i < nrows; ++i) {
        for (int j = 0; j < ncols; ++j) {
            for (int k = 0; k < ncols2; ++k) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }

    auto end = high_resolution_clock::now();

    std::cout << nrows << ", "
    << ncols << ", "
    << ncols2 << ", "
    << nrows * ncols + ncols * ncols2 << ", " << duration_cast<microsecond>(end - start).count() << std::endl;
}


int main(int argc, char * argv[]) {
    int nrows, ncols, ncols2;
    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);
    ncols2 = atoi(argv[3]);
    matmult_without_saving(nrows, ncols, ncols2);

    return 0;
}
