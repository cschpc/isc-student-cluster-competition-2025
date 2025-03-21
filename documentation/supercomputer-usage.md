# Brief guideline to different supercomputers

## Mahti

[Comprehensive user guide](https://docs.csc.fi/computing/)

Connecting:
```
ssh yourcscusername@mahti.csc.fi
```

Finding project directories:
```
csc-workspaces
```

Simple batch job script:
```
#!/bin/bash
#SBATCH --job-name=example
#SBATCH --account=project_2012524
#SBATCH --partition=medium
#SBATCH --time=00:10:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=8

# Set the number of threads based on --cpus-per-task
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun myexe
```

## Puhti

[Comprehensive user guide](https://docs.csc.fi/computing/)

Connecting:
```
ssh yourcscusername@puhti.csc.fi
```

Finding project directories:
```
csc-workspaces
```

Simple batch job script:
```
#!/bin/bash
#SBATCH --job-name=example
#SBATCH --account=project_2012524
#SBATCH --partition=large
#SBATCH --time=00:10:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=10
#SBATCH --cpus-per-task=4

# Set the number of threads based on --cpus-per-task
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun myexe
```

## LUMI

[Comprehensive user guide](https://docs.lumi-supercomputer.eu/)

Connecting:
```
ssh yourcscusername@lumi.csc.fi
```

Finding project directories:
```
lumi-workspaces
```

```
#!/bin/bash
#SBATCH --job-name=example
#SBATCH --account=project_2012524
#SBATCH --partition=standard
#SBATCH --time=00:10:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=8

# Set the number of threads based on --cpus-per-task
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

srun myexe
```

## Levante

[Levante user guide](https://docs.dkrz.de/doc/levante/)

- Non-hierarchical "TCL" modules
  - No `spider` command, `avail` shows all available modules
- Available compilers:
    - Intel (recommended)
    - GNU
    - NVIDIA HPC SDK (especially for GPU codes)
- Available MPI libraries:
    - OpenMPI
    - Intel

- Slurm configuration
   - Simultaneous multithreading is enabled by default, disable by explicit
     `--hint=nomultithread` "#SBATCH" or "srun" option. (most HPC workloads
     perform worse with SMT).

- 

## Bridges-2

## Triton
