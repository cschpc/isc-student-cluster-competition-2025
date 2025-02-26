# Installing Seissol with Spack

General Spack documentation can be found at: https://spack.readthedocs.io

## Installing and configuring Spack

Clone spack:
```
git clone -c feature.manyFiles=true --depth=2 https://github.com/spack/spack.git
```

Activate shell support (for bash):
```
. spack/share/spack/setup-env.sh
```
(If you want to have Spack always available you can do something like
```
echo ". " `realpath spack/share/spack/setup-env.sh` >> $HOME/.bashrc
```
)

Find compilers and external system packages:
```
spack compiler find
spack external find
```

Finding additional compilers may require loading additional modules:
```
module load .unsupported
module load intel-oneapi-compilers
spack compiler find
```
Note: once a compiler is known to spack, modules are no longer needed for
using it.

Spack can install all dependencies of a software (including MPI), but normally
one wants to use readily available MPI installations. For this, we need to
tell spack to use "external" MPI installation (the above `external find` added
already some system packages as externals). Spack configuration is gathered
from various files, the most convenient way to edit configuration is via
Spack's own command line tools. In order to edit "packages" configuration use:
```
spack config edit packages
```
Next, add the specification for external OpenMPI to the end of `packages.yaml`
file the above command opened prefix can be found e.g. with `module show
openmpi`):
```
  openmpi:
    externals:
    - spec: openmpi@4.1.2 %gcc@11.2.0
      prefix: /appl/spack/v017/install-tree/gcc-11.2.0/openmpi-4.1.2-h6c3ze
    buildable: False
```    

By default, Spack uses very long hashes in the installation paths, so as our
final basic configuration we tell Spack to use only six characters. Let's open
"config" configuration:
```
spack config edit config
```
and add the following specification:
```
config:
  install_tree:
    root: $spack/opt/spack
    projections:
      all: '{architecture}/{compiler.name}-{compiler.version}/{name}-{version}-{hash:6}'
```      

Spack can also generate modules, but in order to make them usable requires quite a bit
of additional configuration, so we will skip it for time being.

## Installing Seissol

Check if Spack has Seissol package:
```
spack list seissol
```

See Seissol versions and variants known by Spack:
```
spack info seissol
```

Before proceeding with the installation, it is useful to check the "spec" i.e.
the actual configuration together with dependencies that would be installed:
```
spack spec seissol 
```

By default, Seissol would be build with "Libxsmm" and "PSpaMM" code generators
for small matrix multiplications. However, "Libxsmm" does not seem to work in 
Mahti, so specify a variant with only "PSpaMM" in `gemm_tools_list`:
```
spack spec seissol gemm_tools_list=PSpaMM
```
(Note: in Levante LIBXSMM seems to work, so you may want to try the default
settings once working there).

If the "spec" looks reasonable (e.g. it is using also the externals we wanted
such as OpenMPI), we are ready to finally install seissol together with the dependencies
for the specified variant:
```
spack install seissol gemm_tools_list=PSpaMM
```    

Once the installation completes, we have the binaries in the installation
directory
`.../spack/opt/spack/linux-rhel8-x86_64_v3/gcc-11.2.0/seissol-1.3.0-.../bin`

## Running the Spack installed version

In principle, we can just use the full path to the binaries, or add manually
the installation directory to PATH. Spack provides also similar mechanism as
`module`, *i.e.* "load" / "unload", so we can also just do
```
spack load seissol
```
which adds the installation directory to the PATH.






