# Parallel Dense Matrix Multiplication
An implementation of parallel dense matrix multiplication in C and OpenMP. 
## Parallelization Strategy 
I used tiling to improve the algorithm’s performance. My approach to tiling is based on the code in our OpenMP lecture. My program accepts a value for the tile size and breaks the input matrices into sub-matrices according to the tile size.   

Upon the tiling algorithm, I used OpenMP for parallelization. I used the for statement and collapse statement to distribute work to worker threads. A certain number of worker threads (specified in the command line) is created, and each worker thread can work on an iteration of the triple for loop when it is idle.  

In addition, I also made sure to make the update to the result matrix atomic to prevent contamination to the result.  

## Performance 
For a detailed analysis on performance, refer to report.pdf.  

## How to compile and run
Run  
make  
to compile and generate executable.   

The executable takes 5 parameter values $1, $2, $3, $4, $5  
$1 = Input A nrows  
$2 = Input A ncols  
$2 = Input B ncols2  
$4 = tile size  
$5 = thread number  

In each execution, the program prints out the following comma separated values, they are   
Input A nrows, Input A ncols, INput B ncols2, combined data size, number of threads, tile size, execution time in microsecond.    
Example:  
1000, 1000, 1000, 2000000, 8, 256, 1638002  

Run  
sbatch matmult_omp.sh  
to send job to the HPC. The shell script executes the matrix multiplication algorithm with different input sizes and different number of threads.   

