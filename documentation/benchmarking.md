# Benchmarking tips

- Premature optimization is a root of all evil
- First, get a working baseline build and run configuration
- Use version control

## Building

- Try to make your builds reproducible i.e. create  build scripts
- Use version control for build scripts / Makefiles
   - Make sure to commit at latest when you have something that works
- Once you have a working baseline with some compiler, compiler options and
  libraries, try to experiment with different variants

## Running

- When using hybrid MPI + OpenMP parallelization make sure you understand how
processes and threads are bind to CPU cores with the particular Slurm setup
    - Experiment with the [affinity test code](https://github.com/cschpc/affinity)
    - Run the application with the following variables set:
    ```
    export OMP_AFFINITY_FORMAT="Process %P level %L thread %0.3n affinity %A"
    export OMP_DISPLAY_AFFINITY=true
    ```
    - ssh into node and use `top` and `htop` while the application is running
    - Document your findings

- Try to make your benchmarks easy to reproduce and analyse
    - Utilize special Slurm variables `SLURM_NNODES`, `SLURM_NTASKS_PER_NODE`
      *etc.*
    - Have the batch job script to save a copy of itself (`cp $0 ...`)
    - Make scripts (bash, Python, ...) that generate actual Slurm job scripts from
      templates
        - Especially when developing, it is a good practice to separate
          generating and submitting in order to avoid submission of large
          number of incorrect scripts.
    - Make scripts (bash, Python) that analyse results (sometimes a simple
      `grep` / `awk` one-liner is enough)
- Use version control for benchmarking scripts

## Reporting

- In a competition setting proper reporting might be as important as having good
results
- More details, the better
    - Compiler, options, libraries
    - Number of MPI tasks, threads, GPUs, possible binding settings
- Report best performance as well as at least some differences between various
  settings
    - Even better if you can explain why certain setting was the best (just
      "Intel generated better code in this setting than GCC" is a good start)

