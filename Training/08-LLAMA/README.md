# SCC Training 08 - LLAMA/AI (at CSC)

Date 29.4.2025  

## Agenda

|Time|Topic|
|---|---|
|13:00|Discussion on previous topics|
|13:15|AI methods|
|13.45|Installing and running LLAMA|
|14:30|Coffee break|
|14:45|Installing and running LLAMA|
|16:00|Training concluded|

## Exercise instructions:

Find instructions for installing on Mahti at: https://github.com/shanshanwangcsc/isc25_llm/tree/run_on_mahti

Note that there is a reservation `scc-gpu` in Mahti (`sbatch --reservation=scc-gpu ...`) on partition `gpumedium` and `gpusmall`

CSC Docs page on Fine tuning LLM's: https://docs.csc.fi/support/tutorials/ml-llm/

For any specific AI/LLM themed questions you can contact the instructors through email:  
Shanshan Wang: shanshan.wang@csc.fi  
Mats Sjöberg: mats.sjoberg@csc.fi

## Building a singlularity container for the LLM task

The "pytorch" container referred to in the competition page
(https://hpcadvisorycouncil.atlassian.net/wiki/spaces/HPCWORKS/pages/3240656909/Getting+Started+with+Deep+Learning+tasks+-+Fine-tuning+LLaMA+3.1+8B+with+LoRA+In-Person)
does not contain all the necessary Python packages (e.g. `transformers`).
However, it can be used as a basis for more complete image in the singularity
recipe [isc25-llm.def](isc25-llm.def). The container be build with
```
apptainer build isc25-llm.sif isc25-llm.def
```
