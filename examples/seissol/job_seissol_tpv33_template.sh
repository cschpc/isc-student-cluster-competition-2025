#!/bin/bash
#SBATCH --job-name=seissol_tpv33
#SBATCH --account=project_2002078
#SBATCH --output=out.%x.%j
#SBATCH --error=out.%x.%j
#SBATCH --partition=medium
#SBATCH --time=00:30:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=#NTASKS_PER_NODE
#SBATCH --cpus-per-task=#CPUS_PER_TASK
#SBATCH --mem=0

ulimit -Ss unlimited

exe=/scratch/project_2002078/jenkovaa/spack-isc25/opt/spack/linux-rhel8-x86_64_v3/gcc-11.2.0/seissol-1.3.0-ywvytdpvmdxxys77kcxewl2ujrk3turc/bin/SeisSol_Release_dhsw_4_elastic

export SEISSOL_COMMTHREAD=#COMM_THREAD
if [ $SLURM_CPUS_PER_TASK == 1 ]
then
  export SEISSOL_COMMTHREAD=0
fi

CPU_HYPERTHREADING=1
NUM_CORES=$(expr $SLURM_CPUS_PER_TASK / $CPU_HYPERTHREADING)
NUM_COMPUTE_CORES=$(expr $NUM_CORES - $SEISSOL_COMMTHREAD)
export OMP_NUM_THREADS="$(expr $NUM_COMPUTE_CORES \* $CPU_HYPERTHREADING)"
export OMP_PLACES="cores($NUM_COMPUTE_CORES)"
export OMP_PROC_BIND=spread

rundir=run-${SLURM_JOB_NAME}-nodes-${SLURM_NNODES}-cpus_per_task-${SLURM_CPUS_PER_TASK}-commthread-${SEISSOL_COMMTHREAD}-${SLURM_JOBID}
mkdir $rundir
cp $0 $rundir

inp_dir=$PWD/precomputed-seissol/tpv33
cd $rundir
ln -s ${inp_dir}/* .

stdout_file=out-${SLURM_JOB_NAME}-nodes-${SLURM_NNODES}-cpus-per-task-${SLURM_CPUS_PER_TASK}-commthread-${SEISSOL_COMMTHREAD}-${SLURM_JOBID}
srun $exe parameters.par > $stdout_file


