#!/usr/bin/env python3

import sys

# use e.g. as
# ./analyze-seissol.py run-seissol_turkey-nodes-*/out-* | sort -g -k 4 

files = sys.argv[1:]
print('#nodes ranks/node threads flop/s')
for file in files:
    with open(file, 'r') as f:
        for line in f:
            if 'Using MPI with #ranks:' in line:
                line = line.split()
                total_ranks = int(line[9])
            if '#ranks/node:' in line:
                line = line.split()
                ranks_node = int(line[11])
                nodes = total_ranks // ranks_node
            if '#threads/rank:' in line:
                line = line.split()
                threads = int(line[9])
            if 'Time spent in simulation:' in line:
                line = line.split()
                elapsed_time = float(line[9]) 
                print('  {0:3d}     {1:4d}      {2:2d}    {3:f}'.format(nodes, ranks_node, threads, elapsed_time))
                break
                
