#!/bin/bash
#SBATCH --chdir /home/heuser/Assignments/A3/Originals
#SBATCH --nodes 1
#SBATCH --ntasks 1
#SBATCH --cpus-per-task 36
#SBATCH --mem 10G


echo STARTING AT `date`

./numa
./order

echo FINISHED at `date`
