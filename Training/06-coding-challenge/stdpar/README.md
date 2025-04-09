# Building with NVIDIA HPC toolkit

Load module:
```
ml purge
ml use /appl/opt/nvhpc/modulefiles
ml nvhpc/24.3
ml gcc
```

GPU compilation with:
```
nvc++ -O4 -std=c++20 -stdpar=gpu -gpu=cc80 --gcc-toolchain=/appl/spack/v017/install-tree/gcc-8.5.0/gcc-11.2.0-zshp2k/bin/g++
```

# Running in Mahti

In Mahti, GPUs are requested from Slurm with `--gres=gpu:a100:NUM`, e.g. in order to use two GPUs per node one would have in the job script:
```
...
#SBATCH --ntasks-per-node=2
#SBATCH --gres=gpu:a100:2
...
```

# Analysing performance with Nsight Systems

Collect trace and statistics:
```
srun nsys profile -t nvtx,cuda -o results --stats=true ./myexe
```

The resulting trace can be opened with `nsys-ui`:
```
nsys-ui results.nsys-rep
```

GUI may work smoother if you install NSight Systems on your laptop and copy the "\*rep" file there. The tool can be downloaded from https://developer.nvidia.com/nsight-systems/get-started

# Using stdpar with MPI

By default, stdpar will use only the GPU 0. In order to have different MPI tasks to use
different GPU, one needs to use wrapper script which makes specific GPU available for each MPI task:
```
#!/bin/bash

device=$SLURM_LOCALID
export CUDA_VISIBLE_DEVICES=$device

$*
```
and run the application via that:
```
srun ./gpu_wrap.sh ./myapp
```


