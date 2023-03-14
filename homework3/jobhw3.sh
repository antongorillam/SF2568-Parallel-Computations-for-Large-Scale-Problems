#!/bin/bash -l

# This examples launches 8 MPI ranks on 2 nodes, with 4 ranks per
# node.

# time allocation
#SBATCH -A edu22.sf2568
# job name
#SBATCH -J myjob
# email notification
#SBATCH --mail-type=BEGIN,END
# 10 minutes wall-clock time will be given to this job
#SBATCH -t 00:10:00
# Number of nodes
#SBATCH --nodes=2
# Number of tasks per node
#SBATCH --ntasks-per-node=4
# Number of tasks per core (prevent hyperthreading)
#SBATCH --ntasks-per-core=1
#SBATCH --cpus-per-task=1
# Use Dardel's main partition
#SBATCH -p main

srun ./odd_even_transposition_sort.x
