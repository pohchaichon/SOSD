#pragma once
#include "benchmark.h"

template<template<typename> typename Searcher>
void benchmark_64_xindex(sosd::Benchmark<uint64_t, Searcher>& benchmark, bool pareto);
