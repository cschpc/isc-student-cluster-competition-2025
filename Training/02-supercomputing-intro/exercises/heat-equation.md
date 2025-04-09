# Building and running heat equation solver

In this exercise you should build a simple three dimensional heat equation solver,
and investigate its performance in Mahti.

You should be working within the "scratch" directory in Mahti. The project
directories can be found with the `csc-workspaces` command.

Download ("clone") the code with
```
git clone https://github.com/cschpc/heat-equation.git
```

## MPI+OpenMP version

Go the directory with C++ implementation using both MPI and OpenMP:
```
cd 3d/mpi-openmp
```

Build the code first with the default (GNU) compiler just by executing
`make`. Try to run the code with different combinations of process and
threads (with one and two nodes) and investigate the performance.

Next, try to use AMD compiler by loading the `aocc` module and recompiling
with:
```
ml aocc
make -B
```
Do you see any performance difference between GNU and AMD compilers? You can
try to see also if different compiler options affect the performance by
editing the `Makefile` and using e.g. `-O2` instead of `-O3`.

## (Bonus) CUDA version

Go the directory with the CUDA version for NVIDIA GPUs:
```
cd 3d/cuda
```

In order to build the CUDA version, `cuda` and `openmpi/4.1.2-cuda` (for GPU
aware MPI) modules are needed:
```
ml cuda openmpi/4.1.2-cuda
make
```

Try to run the code with single node using one or all the four GPUs. Note that
there is no reservation for GPU nodes, so one might need to queue for some
time. For 1 GPU job you can use `gputest` or `gpusmall` partitions, for 4 GPU
`gputest` or `gpumedium` (you can try to check with `sinfo` which partition
has idle nodes). See [Mahti
documentation](https://docs.csc.fi/computing/running/example-job-scripts-mahti/#1-2-gpu-job-ie-gpusmall-partition)
for the Slurm settings needed for GPU runs.



