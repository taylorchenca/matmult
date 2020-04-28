#!/bin/bash
#
#SBATCH --job-name=matmult
#SBATCH --partition=cpu
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=1G
#SBATCH --nodes=1
#SBATCH --output=matmult-%j.out #SBATCH --time=10:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=taylor.chen3000@gmail.com #
export OMP_NUM_THREADS=1
export OMP_PLACES=cores
export OMP_PROC_BIND=spread
for i in {1..4000}
do
  /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult $i $i $i >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/logs/matmult20200427.log
done