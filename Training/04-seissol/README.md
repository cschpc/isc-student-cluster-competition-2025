# SCC Training 04 - SeisSol (At Aalto)

Date 26.2.2025  
Slides: [https://kannu.csc.fi/s/Yxk8jfsxgdGamx7](https://kannu.csc.fi/s/Yxk8jfsxgdGamx7)

## Agenda

|Time|Topic|
|---|---|
|13:00|Homework & discussion about openmx|
|13:30|Brief introduction about package managers|
|13:45|Brief look at SeisSol and how to install it|
|14:30|Coffee break|
|14:45|Demo on Spack|
|15:00|Installing with Spack, Demo on libmpitrace|
|16:30|Training concluded|

## Exercise instructions:

In this session, you will work as a team to install SeisSol on Mahti both manually, and by using Spack.  
You will then run the "TPV33" test case for SeisSol on different node counts.  

1. Start by installing SeisSol:
    - [Manually installing SeisSol](./seissol_manual_installation.md)
2. Download and run the TPV33 test case
    - Go to the ISC SeisSol instructions at https://hpcadvisorycouncil.atlassian.net/wiki/spaces/HPCWORKS/pages/3278569473/Getting+Started+with+SeisSol+for+ISC25+SCC
    - Download the "pre-computed" test cases as instructed 
    - See the SeisSol documentation for instructions on how to run it: https://seissol.readthedocs.io/en/latest/build-run.html#starting-seissol
    - Then, run the test case on a single (full) node and examine the output that it produces
    - Try running it both without any OMP threading and with threading enabled
3. If time permits:
    - Run tpv33 on two and four nodes
    - Test running it "purely" with all cores reserved to MPI and only 1 thread per task. How does the performance change when compared to a threaded run? 

4. [Installing SeisSol with Spack](./seissol_spack_installation.md)

## Homework

### Benchmarking homework for SeisSol

Prepare a report with the following information:

1. Run SeisSol with the "Turkey" test case with the following tests:
    - On a single node on Mahti's CPU partition:
        - Set the environment variable SEISSOL_COMMTHREAD to 0 or 1 in your job script. Does it affect the performance?
        - Run it on different combinations of MPI tasks and OMP threads (e.g. 8x16, 4x32, 32x4,...). What combination gives you the best results?
        - Try setting the environment values "OMP_PLACES" and "OMP_PROC_BIND" to a few different possible values. What values give you the best performance? (See the [OpenMP documentation](https://www.openmp.org/spec-html/5.0/openmpse53.html) for ideas)
    - Run the test on two, and then four nodes:
        - Document the performance results
        - Do the MPI task/OMP thread combinations behave differently? What options give you the best results?
2. Run libmpitrace on the Turkey case on 1, 2 and 4 nodes
    - Which three MPI calls take the most amount of time? 
    - How much time is spent in communication?
    - Libmpitrace instructions in [/documentation/profiling.md](/documentation/profiling.md)
  
**Note!** Running time for Turkey is anything between 15min - >1hour on 1-4 nodes. Reserve enough time in Slurm for your runs.

You can find the Turkey data set from the [ISC assignment](https://hpcadvisorycouncil.atlassian.net/wiki/spaces/HPCWORKS/pages/3278569473/Getting+Started+with+SeisSol+for+ISC25+SCC) 

Submission deadline: Return to the advisors before the next training (Deadline 4.3.)

Additional links:

- SeisSol documentation: https://seissol.readthedocs.io/en/latest/index.html
- Spack documentation: https://spack.io/
- Libmpitrace instructions: [/documentation/profiling.md](/documentation/profiling.md)

