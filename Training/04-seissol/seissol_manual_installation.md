# SeisSol (manual) build instructions

**Note!** If you will install SeisSol manually during the training, please do it through a batch job, or have only 1-2 team members do it! If all 12 students compile it simultaneously we would tank the system's I/O.

Some dependencies mentioned in [SeisSol's documentation](https://seissol.readthedocs.io/en/latest/build-dependencies.html) are found as modules on Mahti. These include:

- The GNU compiler suite
- MPI
- HDF5 (parallel version)
- NetCDF-c
- Python (with Numpy)
- CMake

Enable the modules with:

`module load gcc/11.2.0 openmpi/4.1.2 hdf5/1.10.7-mpi netcdf-c/4.8.1 python-data/3.10-24.04 cmake/3.21.4`

When compiling manually, it is helpful to create a separate installation folder, where you install the required (+ some optional) dependencies and eventually SeisSol. Choose where you will initialize the folder, and specify the environment variable `$SEISSOL_PREFIX` to point to it:

`export SEISSOL_PREFIX=/scratch/project_2012524/$USER/seissol_install`

If you're installing things manually, specifying also the following environment variables will make the process much easier:

```
export SEISSOL_BASE=$TMPDIR/seissol_base

mkdir -p $SEISSOL_BASE

export PATH=$SEISSOL_PREFIX/bin:$PATH
export LIBRARY_PATH=$SEISSOL_PREFIX/lib:$SEISSOL_PREFIX/lib64:$LIBRARY_PATH
export LD_LIBRARY_PATH=$SEISSOL_PREFIX/lib:$SEISSOL_PREFIX/lib64:$LD_LIBRARY_PATH
export PKG_CONFIG_PATH=$SEISSOL_PREFIX/lib/pkgconfig:$SEISSOL_PREFIX/lib64/pkgconfig:$PKG_CONFIG_PATH
export CMAKE_PREFIX_PATH=$SEISSOL_PREFIX:$CMAKE_PREFIX_PATH
export CMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX
export CPATH=$SEISSOL_PREFIX/include:$CPATH
export C_INCLUDE_PATH=$SEISSOL_PREFIX/include:$C_INCLUDE_PATH
export CXX_INCLUDE_PATH=$SEISSOL_PREFIX/include:$CXX_INCLUDE_PATH

export CC=mpicc
export CXX=mpicxx
export FC=mpifort
```

**Modify the first line** to match where you want to compile your programs. 
In the example above, I am building all source codes in Mahti's $TMPDIR disk area, and installing all executables, libraries and headers into the "$SEISSOL_PREFIX" folder, under my username.

*Pay special attention to the last three lines.* We want to make sure that the C, C++ and Fortran compilers we will pass by default to CMake will point to the MPI compiler wrappers on our system. 
The other variables are included to make sure that CMake knows where to look for the dependencies that we will start installing in the next steps.

We will start by downloading source codes for the dependencies that aren't available as modules. We will download them into the `$SEISSOL_BASE` folder we specified above. Then, we will install them one by one into a separate `$SEISSOL_PREFIX` folder. This way, we are keeping the source codes separate from the installed headers, library files and executables.

Go to your seissol base folder with `cd $SEISSOL_BASE`, and start installing the software one by one:

## Eigen 

```
wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
tar -xf eigen-3.4.0.tar.gz
cd eigen-3.4.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX
make install
cd ../..
```

## Ninja software installer

This is not a required dependency, but makes it simpler to install others

```
git clone --branch v1.12.0 --depth 1 https://github.com/ninja-build/ninja.git
mkdir -p ninja/build
cd ninja/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX
make -j 10 install
cd ../..
```

## ParMetis

```
wget https://ftp.mcs.anl.gov/pub/pdetools/spack-pkgs/parmetis-4.0.3.tar.gz
tar -xvf parmetis-4.0.3.tar.gz
cd parmetis-4.0.3
sed -i 's/IDXTYPEWIDTH 32/IDXTYPEWIDTH 64/g'  ./metis/include/metis.h
make config cc=mpicc cxx=mpicxx prefix=$SEISSOL_PREFIX
make install
cp build/Linux-x86_64/libmetis/libmetis.a $SEISSOL_PREFIX/lib
cp metis/include/metis.h $SEISSOL_PREFIX/include
cd ..
```

## YAML-CPP

```
wget https://github.com/jbeder/yaml-cpp/archive/refs/tags/0.8.0.tar.gz
tar -xf 0.8.0.tar.gz
mkdir -p yaml-cpp-0.8.0/build
cd yaml-cpp-0.8.0/build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DCMAKE_BUILD_TYPE=Release -GNinja
ninja install
cd ../..
```

## ASAGI

```
git clone --recursive --depth 1 https://github.com/TUM-I5/ASAGI
mkdir -p ASAGI/build
cd ASAGI/build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DCMAKE_BUILD_TYPE=Release -GNinja
ninja install
cd ../..
```

## Lua

```
wget https://www.lua.org/ftp/lua-5.4.6.tar.gz
tar -xf lua-5.4.6.tar.gz
cd lua-5.4.6
make all install INSTALL_TOP=$SEISSOL_PREFIX
cd ..
```

## easi

```
git clone --recursive --depth 1 https://github.com/seissol/easi
mkdir -p easi/build
cd easi/build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DCMAKE_BUILD_TYPE=Release -GNinja -DASAGI=ON -DLUA=ON -DIMPALAJIT=OFF -DEASICUBE=OFF
ninja install
cd ../..
```

# And finally, compiling SeisSol itself

Clone the source:  
`git clone --recursive --branch v1.3.0 --depth 1 https://github.com/SeisSol/SeisSol.git`

Go into the SeisSol folder and create a build directory:  
`cd SeisSol; mkdir -p build; cd build`

Create the makefile through CMake with the options of your choosing:  
`cmake -DNUMA_AWARE_PINNING=ON -DCMAKE_BUILD_TYPE=Release -DASAGI=ON -DPRECISION=double -DORDER=4 -DEQUATIONS=elastic -DGEMM_TOOLS_LIST=Eigen -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX ..`

And compile with make (with a chosen amount of cores):  
`make -j <number of cores>`

Check the [documentation](https://seissol.readthedocs.io/en/latest/build-seissol.html#build-seissol) for any modifications, or ask one of the advisors for help in choosing. 

**Note!** The architecture of Mahti's AMD Rome 7H12 would correspond to the "rome" option with SeiSsol's `-DHOST_ARCH` cmake option, however choosing it leads to cryptic errors in my brief testing. I would advice against using the argument.

When the compilation goes through, the first step is to check if seissol runs. If all worked out, you will see two SeisSol executables. One of them is a so-called "proxy" version that is used for testing, and the other is the one that you should use for normal execution.  
Try a short sequential run with the proxy version to ensure that seissol works by running it with the parameters: 

`SeisSol_proxy_YOUR_CONFIGURATION 1000 10 all`
