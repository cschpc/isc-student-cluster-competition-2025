# Process and thread affinity

In this exercise you can investigate how processes and threads are bind to CPU cores and
how various Slurm and OpenMP settings can affect that.

You should be working within the "scratch" directory in Mahti. The project
directories can be found with the `csc-workspaces` command.

1. Go to the project scratch and create a personal directory there:
```
cd /scratch/project_2012524
mkdir $USER
cd $USER
```
2. Download a simple affinity test code with `git clone` under your personal scratch space:
```
git clone https://github.com/cschpc/affinity.git
```
3. Build the code along the instructions in the main `README.md` in the repositoty.
4. Run the code with different combinations of `--ntasks-per-node` and `--cpus-per-task` Slurm settings (with one or two nodes), as well with different settings of `OMP_NUM_THREADS` and `OMP_PLACES` environment variables.

**Note**: It is highly recommended that when ever you start to work on a new HPC system
you investigate how process and thread binding work there.
