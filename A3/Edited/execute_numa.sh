#!/bin/bash
#SBATCH --chdir /home/heuser/Assignments/A3/Edited
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 36
#SBATCH --mem 10G


echo STARTING AT `date`

echo Local:
numactl --localalloc ./numa
echo Interleaved:
numactl --interleave=all ./numa
echo remote
numactl --membind=1 --cpunodebind=0 ./numa
#./order

echo FINISHED at `date`
