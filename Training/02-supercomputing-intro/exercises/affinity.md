# Process and thread affinity

In this exercise you can investigate how processes and threads are bind to CPU cores and
how various Slurm and OpenMP settings can affect that.

1. Download a simple affinity test code with `git clone` under your personal scratch space:
```
git clone https://github.com/cschpc/affinity.git
```
2. Build the code along the instructions in the main `README.md` in the repositoty.
3. Run the code with different combinations of `--ntasks-per-node` and `--cpus-per-task` Slurm settings (in one or two nodes), as well with different settings of `OMP_NUM_THREADS` and `OMP_PLACES` environment variables

**Note**: It is highly recommended that when ever you start to work on a new HPC system
you investigate how process and thread binding work there.
