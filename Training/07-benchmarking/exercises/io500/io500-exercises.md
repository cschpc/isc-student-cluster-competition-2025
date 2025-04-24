# Running on Mahti (Exercise 1.a):

Build and install io500 on mahti. Keep increasing the number of tasks until the system reaches full saturation and a good result is achieved. After this experipent how the result could be improved.

For target filesystems test both on /scratch/ and [$LOCAL_STORAGET](https://docs.csc.fi/computing/disk/). NOTE: Not many runs can be active on the scratch at the same time and the results can vary since it is dependent on the network traffic.

For benchmarking an average of three is generally a good idea especially on scratch due to the inconsitencies of the network. 

## Extra (Exercise 1.b, not necessary):

Mahti has a Lustre filesystem installed so one can expriment with striping: https://docs.csc.fi/computing/lustre/

# Running on LUMI (Exercise 2, not necessary)

Same as Exercise 1.a except run on /scratch and /flash. NOTE: that again both of these filesystems are shared and can't have multiple runs on them running at the same time.

Necessary modules 

```
module swap PrgEnv-cray/8.5.0 PrgEnv-gnu
export CC=cc
```

[Lumi batch-scripts](https://docs.lumi-supercomputer.eu/runjobs/scheduled-jobs/lumig-job/).
