# Visualization of SeisSol output with ParaView

## Output parameters

By default, the simulation parameters and output behavior of a SeisSol case file is determined in the "parameters.par" file of that case.

A full list of all parameters and explanations can be found in the seissol docs: https://seissol.readthedocs.io/en/latest/parameter-file.html.

Some important settings you can tweak in the parameters.par file to modify your output include:

- EndTime (in seconds)
- TimeInterval
- FaultOutPutFlag (1 or 0)
- OutputFile
- Format (no output or hdf5)

## XDMF 

ParaView natively supports visualizing XDMF files. When you open a given `.xdmf` file, it will automatically load the corresponding `.h5` or `.bin` data it depends on.

As the main `.xdmf` files depend on HDF5 or posix files, make sure that when you visualize an xdmf file that you have the corresponding data files present in the same folder. 

In the case of the Turkey data set, you can visualize seismic wave or fault output xdmf files e.g.: 

```
paraview Turkey_hom_ext4_o6_ev1.xdmf
# or
paraview Turkey_hom_ext4_o6_ev1-fault.xdmf
```

That will depend on information present in the folder `Turkey_hom_ext4_o6_ev1_cell`. Make sure to copy also this data to your local computer if you're running paraview locally.

# Paraview usage

You can start paraview from the command line and include the file you want to open as a parameter, or you can import the file through the GUI under `File/open`.

After loading a file into paraview, you can open it into the grid by clicking the eye symbol in the "Pipeline Browser", or by choosing which Cell Arrays you want to include and clicking Apply in the `Properties` section.

In the `Properties` section, under `Coloring`, choose which cell array you are visualizing and rescale the color bars. Change the timesteps forward and rescale the color bars as needed.

Check the documentation for more details: [https://docs.paraview.org/en/latest/](https://docs.paraview.org/en/latest/)  
