#!/bin/bash

template=job_seissol_tpv33_template.sh

cores_per_node=128
for cpus_per_task in 1 2 4 8 16
do
  tasks_per_node=$((cores_per_node / cpus_per_task))
  if [ $cpus_per_task != 1 ]; then
    for comm_thread in 0 1
    do
      cp $template job.sh
      sed -i "s/#NTASKS_PER_NODE/$tasks_per_node/" job.sh
      sed -i "s/#CPUS_PER_TASK/$cpus_per_task/" job.sh
      sed -i "s/#COMM_THREAD/$comm_thread/" job.sh
      # uncomment sbatch only after making sure script works
      # sbatch job.sh
      # cp is for debugging, comment out when script works
      cp job.sh debug-${tasks_per_node}-${cpus_per_task}-${comm_thread}.sh
    done
  else 
    comm_thread=0
      cp $template job.sh
      sed -i "s/#NTASKS_PER_NODE/$tasks_per_node/" job.sh
      sed -i "s/#CPUS_PER_TASK/$cpus_per_task/" job.sh
      sed -i "s/#COMM_THREAD/$comm_thread/" job.sh
      # uncomment sbatch only after making sure script works
      # sbatch job.sh
      # cp is for debugging, comment out when script works
      cp job.sh debug-${tasks_per_node}-${cpus_per_task}-${comm_thread}.sh
  fi
done
    
