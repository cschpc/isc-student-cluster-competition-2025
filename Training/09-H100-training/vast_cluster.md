# Training cluster

We have reserved a training cluster from VAST AI, with the following hardware:

Two nodes of: 

- 1x Nvidia H100 GPUs 
- AMD EPYC 9554 CPUs
- 200 GB RAM (CPU)
- ~240 GB SSD storage space

## Default software stack

The advisors have installed some minimal requirements on the system as root, including:

- GNU C/Fortran compilers
- The Nvidia CUDA toolkit and H100 GPU drivers
- OpenMPI 

## Your task

Choose 1-2 system administrators that will be given root access. Any system-wide installations should happen through them.

Install and run the following applications:

- Code Saturne
- SeisSol
- OpenMX
- LLaMA

Discuss on how you will divide the work in advance. Make sure to communicate when you will be running your benchmarks, so that your jobs won't compete for the same resources.

Tips:

- With OpenMX and SeisSol you might need to install other dependencies or you can use Spack. Discuss if these should be system-wide installations or local ones.
- There is no Slurm installed on the VM. Instead of using `srun`, use `mpirun` or just run the executable as normal 

