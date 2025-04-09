#!/bin/bash

device=$SLURM_LOCALID
export CUDA_VISIBLE_DEVICES=$device

$*
