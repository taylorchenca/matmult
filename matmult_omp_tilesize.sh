#!/bin/bash
#
#SBATCH --job-name=matmult_omp_tilesize
#SBATCH --partition=cpu
#SBATCH --cpus-per-task=8
#SBATCH --mem-per-cpu=8G
#SBATCH --nodes=1
#SBATCH --output=matmult_omp_tilesize-%j.out
#SBATCH --time=59:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=hchen7@scu.edu #

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
log_file_name=matmult_omp_tilesize-$current_time.log

# For studying tile_size's impact on performance
thread_num=8
for tile_size in 2 4 8 16 32 64 128 256 512
do
/WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp 5000 5000 5000 $tile_size $thread_num >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
done