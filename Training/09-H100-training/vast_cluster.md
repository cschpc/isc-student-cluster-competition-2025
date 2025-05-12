# Training cluster

We have reserved a training cluster from VAST AI, with the following hardware:

Two nodes of: 

- 2x Nvidia H100 GPUs 
- AMD EPYC 9XXX CPUs
- XXXX GB RAM
- XY TB SSD storage space

## Default software stack

The advisors have installed some minimal requirements on the system as root, including:

- GNU C/Fortran compilers
- The Nvidia CUDA toolkit and H100 GPU drivers
- Apptainer 

## Your task

Choose 1-2 system administrators that will gain root access. Any system-wide installations should happen through them.

Install and run the following applications:

- HPL
- HPCG
- Code Saturne

And if time permits, try to install and run the other applications:

- SeisSol
- OpenMX
- LLaMA

Discuss on how you will divide the work in advance. Make sure to communicate when you will be running your benchmarks, so that your jobs won't compete for the same resources.

Tips:

- Easiest/Most performant way to run HPL and HPCG is through Nvidia's HPC-Benchmarks container 
- With OpenMX and SeisSol you might need to install other dependencies or even Spack. Discuss if these should be system-wide installations or local ones.


