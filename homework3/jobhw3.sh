#!/bin/bash -l

# This examples launches 8 MPI ranks on 2 nodes, with 4 ranks per
# node.

# time allocation
#SBATCH -A edu23.sf2568
# job name
#SBATCH -J myjob_1
# email notification
#SBATCH --mail-type=BEGIN,END
# 10 minutes wall-clock time will be given to this job
#SBATCH -t 24:00:00
# Number of nodes
#SBATCH --nodes=1
# Number of tasks per node
#SBATCH --ntasks-per-node=1
# Number of tasks per core (prevent hyperthreading)
#SBATCH --ntasks-per-core=1
#SBATCH --cpus-per-task=1
# Use Dardel's main partition
#SBATCH -p main

srun ./odd_even_transposition_sort.x 1000000 -5 > slurm_sort_1.out
