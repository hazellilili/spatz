#include <benchmark.h>
#include <snrt.h>
#include <stdint.h>
#include <stdio.h>

#include DATAHEADER
#include "kernel/vstore.c"

enum {
  kStreamLen = 32,
  kTotalLen = VSTORE_NUM_STREAMS * kStreamLen,
};

uint64_t *input;
uint64_t *pattern;
uint64_t *output;

int main() {
  const unsigned int cid = snrt_cluster_core_idx();
  unsigned int timer = (unsigned int)-1;
  int errors = 0;

  if (cid == 0) {
    input = (uint64_t *)snrt_l1alloc(kTotalLen * sizeof(uint64_t));
    pattern = (uint64_t *)snrt_l1alloc(kStreamLen * sizeof(uint64_t));
    output = (uint64_t *)snrt_l1alloc(kTotalLen * sizeof(uint64_t));
  }

  if (cid == 0) {
    const unsigned int source_len = sizeof(vl_in_dram) / sizeof(vl_in_dram[0]);

    snrt_dma_start_1d(pattern, vl_in_dram, source_len * sizeof(uint64_t));
    snrt_dma_wait_all();

    for (unsigned int i = source_len; i < kStreamLen; ++i) {
      pattern[i] = pattern[i % source_len];
    }

    for (unsigned int stream = 0; stream < VSTORE_NUM_STREAMS; ++stream) {
      for (unsigned int i = 0; i < kStreamLen; ++i) {
        input[stream * kStreamLen + i] = pattern[i];
        output[stream * kStreamLen + i] = 0;
      }
    }
  }

  if (cid == 0) {
    uint64_t *out[VSTORE_NUM_STREAMS];

    for (unsigned int stream = 0; stream < VSTORE_NUM_STREAMS; ++stream) {
      out[stream] = output + stream * kStreamLen;
    }

    vstore_prepare(input, kStreamLen);

    start_kernel();
    timer = benchmark_get_cycle();
    vstore_test(out[0], out[1], out[2], out[3]);
    timer = benchmark_get_cycle() - timer;
    stop_kernel();

    for (unsigned int stream = 0; stream < VSTORE_NUM_STREAMS; ++stream) {
      for (unsigned int i = 0; i < kStreamLen; ++i) {
        const unsigned int idx = stream * kStreamLen + i;
        if (output[idx] != pattern[i]) {
          const uint64_t got = output[idx];
          const uint64_t exp = pattern[i];
          printf("Mismatch at stream %u index %u: got 0x%016llx, expected 0x%016llx\n",
                 stream, i, (unsigned long long)got, (unsigned long long)exp);
          errors = 1;
        }
      }
    }

    printf("\n----- dummy-vstore -----\n");
    printf("Execution took %u cycles.\n", timer);
    if (!errors) {
      printf("PASS\n");
    }
  }

  snrt_cluster_hw_barrier();
  return errors ? -1 : 0;
}
