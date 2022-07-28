#!/bin/bash
for (( i=0; i < 10; i++ ))
do
    ./msl 1>>output_thread_file.txt
done