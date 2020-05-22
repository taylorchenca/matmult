#!/bin/bash
#
#SBATCH --job-name=matmult_omp_ref
#SBATCH --partition=cpu
#SBATCH --cpus-per-task=28
#SBATCH --mem-per-cpu=8G
#SBATCH --nodes=1
#SBATCH --output=matmult_omp_ref-%j.out
#SBATCH --time=23:59:00
#SBATCH --mail-type=ALL
#SBATCH --mail-user=hchen7@scu.edu #

export OMP_PLACES=cores
export OMP_PROC_BIND=spread
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
log_file_name=matmult_omp_ref-$current_time.log

for x in `seq 50 50 1000`; do 
    for y in `seq 50 50 1000`; do 
        for z in `seq 50 50 300`; do 
            for k in `seq 1 5`; do 
                for thread_num in 1 2 4 8 12 14 16 20 24 28; do
                    /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp_ref -m 1000 -n 1000 -p 1000 -x $x -y $y -z $z -t $thread_num -c  >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
                    /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp_ref -m 5000 -n 5000 -p 5000 -x $x -y $y -z $z -t $thread_num -c >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
                    /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp_ref -m 1000 -n 1000 -p 2000 -x $x -y $y -z $z -t $thread_num -c >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
                    /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp_ref -m 1000 -n 2000 -p 5000 -x $x -y $y -z $z -t $thread_num -c >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
                    /WAVE/projects/COEN-319-Sp20/hchen7/hw2/matmult/matmult_omp_ref -m 9000 -n 2500 -p 3750 -x $x -y $y -z $z -t $thread_num -c >> /WAVE/projects/COEN-319-Sp20/hchen7/hw2/logs/$log_file_name
                done    
            done    
        done
    done
done