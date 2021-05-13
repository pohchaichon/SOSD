#include "common.h"
#include "benchmark.h"
#include "benchmarks/benchmark_xindex.h"
#include "competitors/xindex.h"

template<template<typename> typename Searcher>
void benchmark_64_xindex(sosd::Benchmark<uint64_t, Searcher>& benchmark, bool pareto) {
  benchmark.template Run<XIndex<uint64_t, 1>>();
  if (pareto) {
    benchmark.template Run<XIndex<uint64_t, 4>>();
    benchmark.template Run<XIndex<uint64_t, 8>>();
    benchmark.template Run<XIndex<uint64_t, 16>>();
    benchmark.template Run<XIndex<uint64_t, 32>>();
    benchmark.template Run<XIndex<uint64_t, 64>>();
    benchmark.template Run<XIndex<uint64_t, 128>>();
    benchmark.template Run<XIndex<uint64_t, 256>>();
    benchmark.template Run<XIndex<uint64_t, 512>>();
    benchmark.template Run<XIndex<uint64_t, 1024>>();
    benchmark.template Run<XIndex<uint64_t, 2048>>();
    if (benchmark.uses_binary_search()) {
      benchmark.template Run<XIndex<uint64_t, 4096>>();
      benchmark.template Run<XIndex<uint64_t, 8192>>();
    }
  }
}


INSTANTIATE_TEMPLATES(benchmark_64_xindex, uint64_t);
