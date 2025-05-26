# Installing SeisSol and its dependencies on the competition cluster

During installation you will work both on your local laptop and on the cluster. It's a good idea to keep two separate terminals open.

## Downloading dependencies and SeisSol source on your local machine

Download the following packages locally:

```
wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
wget https://ftp.mcs.anl.gov/pub/pdetools/spack-pkgs/parmetis-4.0.3.tar.gz
wget https://github.com/jbeder/yaml-cpp/archive/refs/tags/0.8.0.tar.gz
curl -LO https://curl.se/download/curl-8.4.0.tar.gz
git clone https://github.com/Unidata/netcdf-c.git
git clone --recursive --depth 1 https://github.com/TUM-I5/ASAGI
wget https://www.lua.org/ftp/lua-5.4.6.tar.gz
git clone --recursive --depth 1 https://github.com/seissol/easi
git clone --recursive --depth 1 https://github.com/SeisSol/SeisSol.git
cd SeisSol
git checkout e5d2c2e
cd ..
```

And also the following python packages (gemmforge 2.0.8 is newer and supports H100s, but compilation of SeisSol fails with it):

```
pip download numpy   --platform manylinux2014_x86_64   --python-version 39   --abi cp39   --only-binary=:all:   -d .
pip download scipy   --platform manylinux2014_x86_64   --python-version 39   --abi cp39   --only-binary=:all:   -d .

git clone https://github.com/SeisSol/gemmforge.git
cd gemmforge
git fetch --tags
git checkout v0.0.207
python3 -m pip wheel . --no-deps -w ..
cd ..

git clone https://github.com/SeisSol/chainforge.git
cd chainforge
pip wheel . -w ..
cd ..
```

## Prepare your setup:

Activate the module system and load the following modules:

```
init-modules
module load gcc
module load openmpi
module load hdf5 openblas cmake
```

Create a Python virtual environment and activate it (in this example we create the venv at $HOME):

```
cd $HOME
python3 -m venv venv
source venv/bin/activate
```

Specify where you will stash your library source files, and where you will keep your installations e.g.:

```
export SEISSOL_PREFIX=$HOME/$seissol_install
export SEISSOL_BASE=$HOME/seissol_base
mkdir -p $SEISSOL_BASE
mkdir -p $SEISSOL_PREFIX
```

Export the following environment variables:

```
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


Go back to your local (online) machine and send everything to the seissol_base folder (this will take a few minutes):

```
rsync --no-i-r --info=progress2 --info=name0 -a /path/to/all/packages/* scc-remote:/path/to/seissol_base
```

## Installing Python packages

Make sure that you're in your Python virtual environment.

Install the dependencies with:

```
pip install --no-index --find-links $SEISSOL_BASE numpy
pip install --no-index --find-links $SEISSOL_BASE scipy
pip install --no-index --find-links $SEISSOL_BASE gemmforge
pip install --no-index --find-links $SEISSOL_BASE chainforge
```

## Installing other dependencies

Then let's go to where all the dependencies are downloaded and let's install them one by one into the `$SEISSOL_PREFIX` directory:

### Curl

```
cd $SEISSOL_BASE
tar -xf curl-8.4.0.tar.gz 
cd curl-8.4.0/
./configure \
  --build="$(uname -m)-pc-linux-gnu" \
  --prefix=$SEISSOL_PREFIX \
  --with-ssl \
  --with-zlib \
  --disable-static \
  --enable-threaded-resolver
make -j8
make install
cd ..
```

### NetCDF-C

```
cd $SEISSOL_BASE/netcdf-c
mkdir build; cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DNETCDF_ENABLE_PARALLEL_TESTS=ON -DNETCDF_ENABLE_TESTS=OFF
make -j8
make install
```

### Eigen

```
cd $SEISSOL_BASE
tar -xf eigen-3.4.0.tar.gz
cd eigen-3.4.0
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX
make -j8
make install
cd ../..
```

### ParMetis

```
cd $SEISSOL_BASE
tar -xvf parmetis-4.0.3.tar.gz
cd parmetis-4.0.3
sed -i 's/IDXTYPEWIDTH 32/IDXTYPEWIDTH 64/g'  ./metis/include/metis.h
make config cc=mpicc cxx=mpicxx prefix=$SEISSOL_PREFIX
make -j8
make install
cp build/Linux-x86_64/libmetis/libmetis.a $SEISSOL_PREFIX/lib
cp metis/include/metis.h $SEISSOL_PREFIX/include
cd ..
```

### YAML-CPP

```
cd $SEISSOL_BASE
tar -xf 0.8.0.tar.gz
mkdir -p yaml-cpp-0.8.0/build
cd yaml-cpp-0.8.0/build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DCMAKE_BUILD_TYPE=Release
make -j8
make install
cd ../..
```

### ASAGI

```
cd $SEISSOL_BASE
mkdir -p ASAGI/build
cd ASAGI/build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DCMAKE_BUILD_TYPE=Release
make -j8
make install
cd ../..
```

### Lua

```
cd $SEISSOL_BASE
tar -xf lua-5.4.6.tar.gz
cd lua-5.4.6
make all install INSTALL_TOP=$SEISSOL_PREFIX
cd ..
```

### easi

*Note! I disable ASAGI here and during Seissol installation because of compilation time errors*

```
cd $SEISSOL_BASE
mkdir -p easi/build
cd easi/build
cmake .. -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DCMAKE_BUILD_TYPE=Release -DASAGI=OFF -DLUA=ON -DIMPALAJIT=OFF -DEASICUBE=OFF
make -j8
make install
cd ../..
```

## Compiling SeisSol

**NOTE! This instruction is based on SeisSol's master branch, so it might not be exactly reproducible in the future**

Go into the SeisSol folder and create a build directory:

```
cd $SEISSOL_BASE/SeisSol; mkdir -p build; cd build
```

Create the makefile through CMake:

### For GPUs:

Change a line in the `submodules/Device.cuda.cmake` file:

```
sed -i '\|target_link_libraries(device PUBLIC CUDA::cudart CUDA::cuda_driver CUDA::nvToolsExt)|{\
s|.*|target_link_libraries(device PUBLIC CUDA::cudart CUDA::cuda_driver)|;\
a\
if(TARGET CUDA::nvToolsExt)\
  target_link_libraries(device PUBLIC CUDA::nvToolsExt)\
endif()\
}' submodules/Device/cuda.cmake
```

And then run cmake:

```
cmake -DNUMA_AWARE_PINNING=ON -DCMAKE_BUILD_TYPE=Release -DASAGI=OFF -DPRECISION=double -DHOST_ARCH= -DORDER=4 -DEQUATIONS=elastic -DGEMM_TOOLS_LIST=Eigen -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX -DDEVICE_BACKEND=cuda -DDEVICE_ARCH=sm_80 -DNUMA_AWARE_PINNING=ON ..
```

And then `make` and `make install`

### For CPUs:

```
cmake -DNUMA_AWARE_PINNING=ON -DCMAKE_BUILD_TYPE=Release -DASAGI=OFF -DPRECISION=double -DORDER=4 -DEQUATIONS=elastic -DGEMM_TOOLS_LIST=Eigen -DCMAKE_INSTALL_PREFIX=$SEISSOL_PREFIX ..
```

(Try changing the -DGEMM_TOOLS_LIST argument if you want to try something other than Eigen)

And after it passes, run `make` and `make install`