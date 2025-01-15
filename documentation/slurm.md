# Slurm tips

Slurm sets several environment variables that can be useful when benchmarking
- `SLURM_JOB_NAME` : name given in `job_name`
- `SLURM_JOBID`
- `SLURM_JOB_NODELIST` : list of nodes the job will run
- `SLURM_NNODES` : number of nodes
- `SLURM_NTASKS_PER_NODE` 
- `SLURM_CPUS_PER_TASK`
- `SLURM_GPUS_PER_NODE`

Wildcards for Slurm output:
- `%x` : job name
- `%J` : job id

The below example illustrates how to put standard error and ouput to different
files (with the job name and id), and redirect application output to job
specific file/directory:

```
#!/bin/bash

# Example on how to benchmark NAMD

#SBATCH --job=namd-apoa1-cpu
#SBATCH --error=%x.%J.err
#SBATCH --output=%x.%J.out
# Normal account, time, resource specifications

# Create output directory for this specific benchmark run:
run_dir=${SLURM_JOB_NAME}-N${SLURM_NNODES}-n${SLURM_NTASKS_PER_NODE}-c${SLURM_CPUS_PER_TASK}-job-${SLURM_JOB_ID}
mkdir $run_dir

# Create symbolic links to NAMD input files wihtin the run_dir
input_files="apoa1.namd apoa1.psf par_all22_popc.xplor apoa1.pdb par_all22_prot_lipid.xplor"
cd $run_dir
for f in $input_files
do
  ln -s ../${f} .
done

# Save exact job script 
cp $0 slurm_script_$SLURM_JOBID

# Save all relevant environment variables
env | grep SLURM > slurm_env_${SLURM_JOBID}.txt
env | grep OMP > omp_env_${SLURM_JOBID}.txt

# Run the application
srun my_bin    # Use full path if executable is not in the PATH
```







