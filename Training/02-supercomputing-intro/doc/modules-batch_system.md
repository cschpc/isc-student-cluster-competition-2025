---
title:  Working in supercomputers
event:  Student cluster competition
lang:   en
---

# Outline

- Module system
- Running applications via batch job system

<br>
Some information provided here is specific to LUMI, Mahti, and Puhti, some more general

# Modules {.section}

# Module environment

- Supercomputers have a large number of users with different needs for
  development environments and applications
- _Environment modules_ provide a convenient way to dynamically change the
  user's environment
- In this way, different compiler suites and application versions can be used
  more easily
    * Changing compiler module loads automatically also correct versions of libraries 
    * Loading a module for application sets up the correct environment with single 
      command


# Common module commands

<div class="column">
`module load mod`
  : Load module **mod** in shell environment

`module unload mod`
  : Remove module **mod** from environment

`module list`
  : List loaded modules

`module avail`
  : List all available modules
</div>

<div class="column">
`module spider mod`
  : Search for module **mod**

`module show mod`
  : Get information about module **mod**

`module switch mod1 mod2`
  : Switch loaded **mod1** to **mod2**
</div>


# Batch queue system {.section}

# Batch queue system

- On a cluster, instead of running a program instantly, you submit your
  program/simulation (aka job) to a queue and the system will then execute it
  once the resources are available
    - The queue enables effective and fair resource usage
    - CSC uses SLURM as the queue system

- When running a job on a supercomputer you need to:
    - Describe how you want to run the job and what resources you need
    - Add a command that launches your program
    - Submit your job to a queue
- This is done with a batch job script


# Example SLURM batch job script

```bash
#!/bin/bash
#SBATCH --job-name=example
#SBATCH --account=<project_id>
#SBATCH –-partition=debug
#SBATCH –-time=00:10:00
#SBATCH –-nodes=2
#SBATCH –-ntasks-per-nodes=128

# srun launches "nodes * ntasks-per-nodes" copies of myprog
srun myprog
```

- More examples:
    - <https://docs.lumi-supercomputer.eu/runjobs/scheduled-jobs/batch-job/>
    - <https://docs.csc.fi/computing/running/example-job-scripts-puhti/>
    - <https://docs.csc.fi/computing/running/example-job-scripts-mahti/>


# Running batch jobs under SLURM

- Submit your batch job script to the queue using `sbatch`
  ```bash
  sbatch my_job.sh
  ```
- You can follow the status of your jobs with `squeue`:
  ```bash
  squeue -u my_username
  ```
- If something goes wrong, you can cancel your job with `scancel`:
  ```bash
  scancel jobid
  ```
  (here the jobid is the numeric ID of the job)
- Show job resource usage (for completed jobs) with `sacct`:
  ```bash
  sacct jobid
  ```

# Useful Slurm environment variables


Following variables are available inside Slurm scripts:

<small>

- `SLURM_JOB_NAME` : name given in `job_name`
- `SLURM_JOBID`
- `SLURM_JOB_NODELIST` : list of nodes the job will run

</small>

Following variables are available inside program launched by `srun`

<small>

- `SLURM_PROCID` : global id of process 
- `SLURM_LOCALID` : node local id of process 

</small>

Following variables override the corresponding variables in the batch script

<small>

- `SBATCH_ACCOUNT` : `--account`
- `SBATCH_PARTITION` : `--partition`

</small>
