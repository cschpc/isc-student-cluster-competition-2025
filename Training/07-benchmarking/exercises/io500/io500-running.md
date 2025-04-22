# Submission and execution

## Submit script

One can run the `io500` executable directly via a batch file, but the reccomended route is using the io500.sh submit script.

The two arguments that need to be specififed are the following:

```bash
io500_mpirun="mpiexec"
io500_mpiargs="-np 2"
```

which are the mpirun executabel and the necessary arguments for mpirun. For example on mahti the mpirun is `srun` and the mpiargs are specified directly in the batch file so these can be skipped. So we set:


io500_mpirun="srun"
io500_mpiargs=""

Additionally since io500.sh is essentially a batch file one need to give the system specific specification in `#SBATCH` form. For example on mahti we set:

```bash
#!/bin/bash -l
#SBATCH -J io500
#SBATCH -o ./slurm_output/output_%j.txt
#SBATCH -e ./slurm_errors/errors_%j.txt
#SBATCH --account=project_2012524

#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --time=12:00:00
```



## ini file

The next important aspect of the submission is the `*.ini` file which is passed to the submit script as the first ($1) argument. 

The `*.ini` file specifices what benchamrks should be run. One can get a list of all the available benchmarks using

    ./io500 --list

At the beginning of the `*.ini` file one specifies the `datadir` and `resultdir`. These are very important as these are the folder locations that the benchmark will use and which are dependent on the filesystem in use. The benchmark

```bash
[global]
# The directory where the IO500 runs
datadir = $DATA_FILE_PATJ
# The data directory is suffixed by a timestamp. Useful for running several IO500 tests concurrently.
timestamp-datadir = TRUE
# The result directory.
resultdir = $RESULT_FILE_PATH
# The result directory is suffixed by a timestamp. Useful for running several IO500 tests concurrently.
timestamp-resultdir = TRUE
# The general API for the tests (to create/delete the datadir, extra options will be passed to IOR/mdtest)
api = POSIX
# Purge the caches, this is useful for testing and needed for single node runs
drop-caches = FALSE
# Cache purging command, invoked before each I/O phase
drop-caches-cmd = sudo -n bash -c "echo 3 > /proc/sys/vm/drop_caches"
# Allocate the I/O buffers on the GPU
io-buffers-on-gpu = FALSE
# The verbosity level between 1 and 10
verbosity = 1
# Use the rules for the Student Cluster Competition
scc = FALSE
# Type of packet that will be created [timestamp|offset|incompressible|random]
dataPacketType = timestamp
```

The `*.ini` file is given by the scc competition and should not be tweaked!

## Running

Done via given slurm script:

`sbatch io500.sh config-scc.ini`

The number of tasks defines the problem size. We need to saturate the benchmarks to get usable results and each benchmark needs to run atleast the length of the defined stonewall time. This means that we need to increase the number of tasks until the results are "VALID"

### 2 Tasks on mahti (Local nvme disk):
```
[RESULT]       ior-easy-write        0.937651 GiB/s : time 31.616 seconds
ERROR INVALID (src/main.c:437) Runtime of phase (13.518965) is below stonewall time. This shouldn't happen!
ERROR INVALID (src/main.c:443) Runtime is smaller than expected minimum runtime
[RESULT]    mdtest-easy-write      159.840698 kIOPS : time 13.519 seconds [INVALID]
[      ]            timestamp        0.000000 kIOPS : time 0.000 seconds
[RESULT]       ior-hard-write        0.461203 GiB/s : time 30.536 seconds
[RESULT]    mdtest-hard-write       59.206262 kIOPS : time 32.056 seconds
[RESULT]                 find     3123.352505 kIOPS : time 1.231 seconds
[RESULT]        ior-easy-read        1.710102 GiB/s : time 17.336 seconds
[RESULT]     mdtest-easy-stat      300.191897 kIOPS : time 7.683 seconds
[RESULT]        ior-hard-read        0.487973 GiB/s : time 28.879 seconds
[RESULT]     mdtest-hard-stat      293.959584 kIOPS : time 7.293 seconds
[RESULT]   mdtest-easy-delete      156.877744 kIOPS : time 14.573 seconds
[RESULT]     mdtest-hard-read       18.275709 kIOPS : time 101.717 seconds
[RESULT]   mdtest-hard-delete       70.476403 kIOPS : time 27.894 seconds
[      ]  ior-rnd4K-easy-read        0.074125 GiB/s : time 30.005 seconds
[SCORE ] Bandwidth 0.775064 GiB/s : IOPS 164.145975 kiops : TOTAL 11.279349 [INVALID]
```

### 8 Tasks on mahti (Local nvme disk):

```
[RESULT]       ior-easy-write        1.484398 GiB/s : time 41.786 seconds
[RESULT]    mdtest-easy-write      194.267561 kIOPS : time 39.762 seconds
[      ]            timestamp        0.000000 kIOPS : time 0.000 seconds
[RESULT]       ior-hard-write        1.230472 GiB/s : time 45.177 seconds
[RESULT]    mdtest-hard-write       33.541007 kIOPS : time 41.231 seconds
[RESULT]                 find     8218.076202 kIOPS : time 1.085 seconds
[RESULT]        ior-easy-read        4.375021 GiB/s : time 14.176 seconds
[RESULT]     mdtest-easy-stat     3344.135450 kIOPS : time 3.259 seconds
[RESULT]        ior-hard-read        7.407705 GiB/s : time 7.508 seconds
[RESULT]     mdtest-hard-stat     4630.935124 kIOPS : time 1.301 seconds
[RESULT]   mdtest-easy-delete      144.171694 kIOPS : time 56.777 seconds
[RESULT]     mdtest-hard-read      347.592176 kIOPS : time 4.890 seconds
[RESULT]   mdtest-hard-delete       36.992190 kIOPS : time 38.166 seconds
[      ]  ior-rnd4K-easy-read       34.708730 GiB/s : time 0.886 seconds
[SCORE ] Bandwidth 2.773776 GiB/s : IOPS 444.984509 kiops : TOTAL 35.132428
```

The benchmark will require allot of hard drive space which is dependent on the number of tasks. For small number of tasks the 1TB available on mahti scratch should be enough. NOTE the whole team cannot run this at the same time as the jobs will run out of space. But for larger systems we can opt to use the Local nvme storage on mahti which has 3.5TB of space available https://docs.csc.fi/computing/disk/#compute-nodes-with-local-ssd-nvme-disks. 

The local disk folder is dynamic and dependent on the node. It is specified with:

`export MY_JOB_TMPDIR=$LOCAL_SCRATCH`

if we define this in our bash script:

```bash
#!/bin/bash -l
#SBATCH -J io500
#SBATCH -o ./slurm_output/output_%j.txt
#SBATCH -e ./slurm_errors/errors_%j.txt
#SBATCH --account=project_2012524

#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --time=12:00:00
export MY_JOB_TMPDIR=$LOCAL_SCRATCH
envsubst < config-scc-fast-io.ini.template > $1
io500_ini="$1"
io500_mpirun="srun"
io500_mpiargs=""
```

and define config-scc-fast-io.ini.template:

```bash
[global]
datadir = ${MY_JOB_TMPDIR}/datafiles
timestamp-datadir = TRUE
resultdir = /scratch/project_2012524/haaaaron/results-fast-io
timestamp-resultdir = TRUE
api = POSIX
drop-caches = FALSE
drop-caches-cmd = sudo -n bash -c "echo 3 > /proc/sys/vm/drop_caches"
io-buffers-on-gpu = FALSE
verbosity = 1
scc = TRUE
dataPacketType = timestamp
```

why do we need to do this?
