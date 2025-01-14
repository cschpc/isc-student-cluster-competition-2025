# SCC Training 02 - Basics of supercomputing

Date 15.1.2025

## Agenda

|Time|Topic|
|---|---|
|13:00|Training and competition schedule|
|13:30|Parallel computing concepts|
|14:30|Coffee break|
|14:45|Hands-on session|

## Hands-on exercises

See e.g. [CSC User
Documentation](https://docs.csc.fi/computing/running/example-job-scripts-mahti/#mpi-openmp)
for examples of Slurm batch job scripts. For this particular training a set of
nodes has been reserved, the reservation can be used with `--reservation`
option to Slurm:
```
sbatch --reservation=scc my_job_script.sh
```

- [Processes, threads, and affinity](exercises/affinity.md)
- [Heat equation](exercises/heat-equation.md)


## Homework

Carry out the exercises in another supercomputer than Mahti (e.g. Puhti or
Triton).

