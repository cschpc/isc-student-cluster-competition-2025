# Running on LUMI

Specifically to utilise flash storage partition
```
Data updated: 2025-04-22 17:05:41
Project             |                    CPU (used/allocated)|               GPU (used/allocated)|           Storage (used/allocated)
--------------------------------------------------------------------------------------------------------------------------------------
project_462000599   |            5/500000   (0.0%) core/hours|     11308/10000 (113.1%) gpu/hours|        3107/5000  (62.1%) TB/hours
project_462000781   |              0/1000   (0.0%) core/hours|      4097/10000  (41.0%) gpu/hours|       1024/10000  (10.2%) TB/hours
project_462000786   |            0/500000   (0.0%) core/hours|       207/10000   (2.1%) gpu/hours|          2/50000   (0.0%) TB/hours
```

Necessary modules 

```
module swap PrgEnv-cray/8.5.0 PrgEnv-gnu
export CC=cc
```

Specify datadir to be in

```
/flash/project_462000786
```

Lumi batch-scripts https://docs.lumi-supercomputer.eu/runjobs/scheduled-jobs/lumig-job/.
