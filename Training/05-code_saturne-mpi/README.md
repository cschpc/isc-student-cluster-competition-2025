# SCC Training 05 - Code_Saturne, MPI (At CSC)

Date 5.3.2025  
Slides: https://kannu.csc.fi/s/2x66k4kJnMibQ3G

## Agenda

|Time|Topic|
|---|---|
|13:00|Homework discussion|
|13:30|Discussion about competition strategies|
|14:00|Brief look Code_saturne at and how to install it|
|14:30|Coffee break|
|14:45|Brief introduction to MPI|
|16:30|Training concluded|

## Exercise instructions:

There is a reservation `scc` in Mahti (`sbatch --reservation=scc ...`)

1. Try to build **code_saturne** along the instructions at https://hpcadvisorycouncil.atlassian.net/wiki/spaces/HPCWORKS/pages/3177381916/Getting+Started+with+code_saturne+for+ISC25+SCC
  - Build will fail (with gcc the reason is more obvious than with Intel), how could you fix that? Create a patch and add it to your team's git repository

2. Implement a MPI parallel sum starting from the [skeleton code](mpi/exercises/sum.cpp).
  - Initialize the data with rank 0
  - Use `MPI_Scatter` for distributing data
  - Use `MPI_Reduce` for collecting partial sums


## Homework


