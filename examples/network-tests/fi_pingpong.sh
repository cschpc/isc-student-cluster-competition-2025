#!/bin/bash
# export FI_LOG_LEVEL=debug
server=`hostname`
client=$1
cmd="fi_pingpong -p verbs"
echo "Running $cmd : server $server client $client"
$cmd &
sleep 1
ssh $client $cmd $server
