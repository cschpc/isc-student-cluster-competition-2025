# Profiling tools

## Mpitrace 

[MPITrace](https://github.com/IBM/mpitrace) is a simple tool that is aimed at analysis
of distributed-memory parallel applications written with MPI.

MPItrace is easy to install and use, and provides a text based overview of
time spent in MPI calls, number of calls, size of messages etc.

## Installation

```
git clone https://github.com/IBM/mpitrace
cd mpitrace
cd src
export CC=mpicc
./configure
make
```
Outcome is a shared library `libmpitrace.so`

Note: libmpitrace need to be build with the same MPI implementation as the
application (e.g. OpenMPI or IntelMPI). If one wants to use mpitrace with
different MPI implementations, a separate versions of libmpitrace need to be
build.

## Usage

In order to use MPITrace, set environment variable LD_PRELOAD to the absolute
path of the shared library, *e.g.*
```
export LD_PRELOAD=/scratch/project_2012524/jenkovaa/mpitrace/src/libmpitrace.so
```

When application is now run, the library collects information about MPI usage,
and writes them out into set of `mpi_profile.*` files in the directory where
the application is run. By default, three files are produced for the MPI ranks having the minimum, median and maximum communication times. See [documentation](https://github.com/IBM/mpitrace/tree/master/src) for more options to control output. 




