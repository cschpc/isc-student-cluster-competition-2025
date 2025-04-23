# Downloading Nvidia's HPC-Benchmarks Container

You can find detailed information about Nvidia’s HPC container here:
https://catalog.ngc.nvidia.com/orgs/nvidia/containers/hpc-benchmarks

## Setup on Mahti

Log into **Mahti** and navigate to your working directory. If you're working together as a team, consider creating a shared folder.

Set an environment variable to specify where the container image will be cached during download. This location needs **over 10 GB** of available space. You can use the fast local disk (`$TMPDIR`) or the scratch disk area of your project:

```
export APPTAINER_CACHEDIR=/scratch/project_2012524/<your-folder-path>
```

Pull the HPC benchmarks container (version 25.04) via Docker, using [apptainer](https://docs.csc.fi/computing/containers/run-existing/) on Mahti:

```
apptainer pull docker://nvcr.io/nvidia/hpc-benchmarks:25.04
```

You can enter and exit the image in the login node using:

```
[mahti-login] apptainer shell <your-image-file.sif>
[Apptainer] exit
```

To enable GPUs, run it in slurm (in a partition with GPU nodes) with a command e.g. `srun apptainer exec --nv /<path_to>/hpc-benchmarks.sif <your commands>`.

## Running HPL

Inside the container, search the `/workspace` directory to locate the wrapper script for HPL (`hpl.sh`).

Prepare the input file `HPL.dat`. You can either:
- Use the examples in the `sample-dat` directory, or 
- Generate one using e.g. this tool: https://www.advancedclustering.com/act_kb/tune-hpl-dat-file/. 

Create a run script and try the following tasks:

- Run with:
    - 1 GPU
    - 1 MPI task
    - 1 CPU core per task
    - Include the `--no-multinode` command at the end
- Try increasing the amount of cores to 32
- Increase the number of GPUs to the maximum per node in Mahti 
    - You are going to run into an issue with MPI/PMIx that results in the job hanging. Try to google the warning/error message that you get to figure out a fix.
    - Remember to also increase the values for **P**, **Q**, and **N** accordingly 
- Run HPL on two full GPU nodes
- Document the results 
- Compare your results to the **theoretical peak performance** of the GPUs. What percentage of the peak performance did you achieve?
- **Extra**: Check Nvidia's [A100 architecture whitepaper](https://images.nvidia.com/aem-dam/en-zz/Solutions/data-center/nvidia-ampere-architecture-whitepaper.pdf) and read about the "groundbreaking" tensor cores on the GPUs. Do you think the tensor cores are applicable on a problem like HPL? Why or why not?

## Running HPCG

Inside the container, search the `/workspace` directory to locate the wrapper script for HPL (`hpcg.sh`).

Prepare the input file `hpcg.dat`. Example files are located in the hpcg directory under `/workspace`. Modify the runtime duration to something sensible for short testing.

Create a run script and execute the following:

- Run with:
    - 1 GPU
    - 1 MPI task
    - 1 CPU core per task
- Try increasing the amount of cores to 32
- Increase the number of GPUs to the maximum per node in Mahti
    - You might run into an issue with MPI/PMIx that results in the job hanging. Try to google the warning/error message that you get to figure out a fix.
- Run HPCG on two full GPU nodes
- Document the results 
- Estimate the **best performance** you might expect from these GPUs and compare it to your results

## General tips

- We have a 4-node reservation called `scc-gpu` on partition `gpumedium`
