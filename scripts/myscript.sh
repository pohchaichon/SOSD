#! /usr/bin/env bash

threads=1
num_iterations=1;
while getopts t:n arg; do
    case $arg in
        t) threads=${OPTARG};;
        n) num_iterations=${OPTARG};;
    esac
done

function do_benchmark() {
		build/benchmark -r $2 ./data/$1 ./data/$1_equality_lookups_10M --only ALEX --threads $threads
		build/benchmark -r $2 ./data/$1 ./data/$1_equality_lookups_10M --only XIndex --threads $threads
		#build/benchmark -r $2 ./data/$1 ./data/$1_equality_lookups_10M --only RMI --threads $threads
}

mkdir -p ./results

for dataset in $(cat scripts/mydatasets.txt); do
		do_benchmark "$dataset" $num_iterations
done
