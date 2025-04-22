# IO500 Installation

## Dependencies

| Dependency       | Version       | Installation Command               |
|------------------|---------------|-------------------------------------|
| GCC              | -     | `module load gcc`             |
| MPI              | -| `module load openmpi`           |
| LZ4              | - | `prepeare.sh` |
| pfind              | - | `prepeare.sh` |
| ior              | - | `prepeare.sh` |
| mdtest             | - | `prepeare.sh` |
| md-workbench              | - | `prepeare.sh` |



## Compiling

First clone the repository:

    git clone https://github.com/IO500/io500.git -b io500-isc24

Load openmpi

    module load openmpi

The code comes bundled with a setup script since it depends on a few libraries that it will attempt to install and build. Notably pfind and lz4

    ./prepare.sh

Run into an error right away:

```
/appl/spack/v017/install-tree////////////////////////////////////gcc-8.5.0/binutils-2.37-ed6z3n/bin/ld: /local_scratch/haaaaron/io500/build/pfind/src/pfind-main.c:44: undefined reference to `MPI_Init'
```

while installing pfind it can't seem to find the necessary mpi libraries, but premapre.sh does find them:

```
checking for function MPI_Init in -lmpi... yes
checking for mpi.h... yes
```

So pfind can't inherit the compiler or the include paths from the module load.

Let's checkout our mpicc:

```
$ which mpicc
/appl/spack/v017/install-tree/gcc-11.2.0/openmpi-4.1.2-h6c3ze/bin/mpicc
```

Completely different path than the error message. Inspecting the compile.sh file in ./build/pfind we notice that it is looking for `$CC` which is set to `gcc` by default. Let's set

    export CC=mpicc

and run again

    ./prepare.sh

```
.
.
.
mpicc -o io500 ./build/main.o -lm -g -O2 -Lcheck/lib64 -Lcheck/lib -Wl,--enable-new-dtags -Wl,-rpath=check/lib64:check/lib -Lcheck/lib64 -Lcheck/lib -Wl,--enable-new-dtags -Wl,-rpath=check/lib64:check/lib -lm  ./build/io500.a ./build/pfind/pfind.a ./build/ior/src/libaiori.a  -lm -g -O2 -Lcheck/lib64 -Lcheck/lib -Wl,--enable-new-dtags -Wl,-rpath=check/lib64:check/lib -Lcheck/lib64 -Lcheck/lib -Wl,--enable-new-dtags -Wl,-rpath=check/lib64:check/lib -lm 
rm build/ini-test.o
io500: OK


OK: All required software packages are now prepared
ior  mdtest  md-workbench  pfind
```

