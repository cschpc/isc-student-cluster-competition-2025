# SCC Training 03 - OpenMX (At Aalto)

Date 5.2.2025
Materials on Siili: https://siili.rahtiapp.fi/cKwDOFX5SQC5NIbmSIRLmg# 

## Agenda

|Time|Topic|
|---|---|
|13:00|Overview of OpenMX|
|14:00|Break|
|14:30|Hands-on training|
|16:00|Training concluded|

## Homework


Benchmarking homework for OpenMX

Both teams are required to report the results of benchmarking the OpenMX software on Mahti. The report should include the following:

1. Compilation / benchmarking with GCC:
	- Compile and run the OpenMX code with GCC and the corresponding libraries.
	- Test with different optimization levels: -O1, -O2, -O3.
	- Record the compilation time and the testrun elapsed time on one CPU core (=wall time) for each optimization level. For the testruns, use Methane.dat example.
	- Verify that the results produced by the benchmarks are correct (compare against build with -O0).

2. Compilation / benchmarking with the Intel compiler suite:
	- The same set as above but with Intel/MKL.

Submission deadline: Return by 25.2 to the advisors.

Hints:

- remember to use --account=project_2012524 for the SLURM submissions
- GCC optimization options: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
- Intel optimization: https://cdrdv2-public.intel.com/671303/quick-reference-guide-intel-compilers-v19-1-final-.pdf
- Notes on https://siili.rahtiapp.fi/s/W3YVX8abh



