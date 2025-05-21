# Simple tests for Infiniband and MPI

## Basic Infiniband tools

- `ibstat`
- `iblinkinfo`
- `ib...`

## Perftest

- <https://github.com/linux-rdma/perftest>

Script for running `ib_write_bw` between two nodes. Execute on one node and provide the hostname/ip address of another node:
```
./ib_test.sh other_node
```

## MPI tests

- Very basic MPI functionality: `mpi-hello.c`
- MPI bandwidth between two tasks: `mpi-bandwidth.c`



