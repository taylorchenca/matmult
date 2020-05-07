#!/bin/bash
#
#SBATCH --job-name=matmult_omp
#SBATCH --partition=cpu
#SBATCH --cpus-per-task=8
#SBATCH --mem-per-cpu=8G
#SBATCH --nodes=1
#SBATCH --output=matmult_omp-%j.out #SBATCH --time=10:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=hchen7@scu.edu #

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
log_file_name=matmult_omp-$current_time.log

for thread_num in 1 2 4 8 12 14 16 20 24 28
do
#  export OMP_NUM_THREADS=$thread_num
  for tile_size in 16 32 64 128 256 512
  do
  /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp 1000 1000 1000 $tile_size $thread_num >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
  /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp 5000 5000 5000 $tile_size $thread_num >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
  /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp 1000 1000 2000 $tile_size $thread_num >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
  /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp 1000 2000 5000 $tile_size $thread_num >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
  /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp 9000 2500 3750 $tile_size $thread_num >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
  done
done