#!/bin/bash
server=`hostname`
client=$1
cmd=ib_write_bw
echo "Running $cmd : server $server client $client"
$cmd &
sleep 1
ssh $client $cmd $server
