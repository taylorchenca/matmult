#!/bin/bash
#
#SBATCH --job-name=matmult
#SBATCH --partition=cpu
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=8G
#SBATCH --nodes=1
#SBATCH --output=matmult-%j.out #SBATCH --time=10:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=hchen7@scu.edu #
export OMP_NUM_THREADS=1
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
log_file_name=matmult-$current_time.log
/WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult 1 1 1 1 1500 >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
#for i in {1..1500}
#do
#  /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult $i $i $i >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
#done

