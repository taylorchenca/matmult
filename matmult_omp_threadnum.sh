#!/bin/bash
#
#SBATCH --job-name=matmult_omp_threadnum
#SBATCH --partition=cpu
#SBATCH --cpus-per-task=8
#SBATCH --mem-per-cpu=8G
#SBATCH --nodes=1
#SBATCH --output=matmult_omp_threadnum-%j.out
#SBATCH --mail-type=ALL
#SBATCH --mail-user=hchen7@scu.edu #

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
log_file_name=matmult_omp_threadnum-$current_time.log

# For studying thread_num's impact on performance
tile_size=32
for thread_num in 1 2 4 8 12 14 16 20 24 28
do
/WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp 1000 1000 1000 $tile_size $thread_num >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
done