#include <benchmark.h>
#include <snrt.h>
#include <stdint.h>
#include <stdio.h>

#include DATAHEADER
#include "kernel/vload.c"

enum {
  kStreamLen = 32,
  kTotalLen = VLOAD_NUM_STREAMS * kStreamLen,
};

uint16_t *input;
uint16_t *pattern;
uint16_t *output;

int main() {
  const unsigned int cid = snrt_cluster_core_idx();
  unsigned int timer = (unsigned int)-1;
  int errors = 0;

  // Allocate the arrays
  if (cid == 0) {
    input = (uint16_t *)snrt_l1alloc(kTotalLen * sizeof(uint16_t));
    pattern = (uint16_t *)snrt_l1alloc(kStreamLen * sizeof(uint16_t));
    output = (uint16_t *)snrt_l1alloc(kTotalLen * sizeof(uint16_t));
  }

  // Initialize the arrays
  if (cid == 0) {
    const unsigned int source_len = sizeof(vl_in_dram) / sizeof(vl_in_dram[0]);

    snrt_dma_start_1d(pattern, vl_in_dram, source_len * sizeof(uint16_t));
    snrt_dma_wait_all();

    for (unsigned int i = source_len; i < kStreamLen; ++i) {
      pattern[i] = pattern[i % source_len];
    }

    for (unsigned int stream = 0; stream < VLOAD_NUM_STREAMS; ++stream) {
      for (unsigned int i = 0; i < kStreamLen; ++i) {
        input[stream * kStreamLen + i] = pattern[i];
        output[stream * kStreamLen + i] = 0;
      }
    }
  }

  if (cid == 0) {
    const uint16_t *in[VLOAD_NUM_STREAMS];
    uint16_t *out[VLOAD_NUM_STREAMS];

    for (unsigned int stream = 0; stream < VLOAD_NUM_STREAMS; ++stream) {
      in[stream] = input + stream * kStreamLen;
      out[stream] = output + stream * kStreamLen;
    }

    vload_prepare(kStreamLen);

    start_kernel();
    timer = benchmark_get_cycle();
    vload_test(in[0], in[1], in[2], in[3], in[4], in[5], in[6], in[7]);
    timer = benchmark_get_cycle() - timer;
    stop_kernel();

    vload_finish(out[0], out[1], out[2], out[3], out[4], out[5], out[6], out[7]);

    for (unsigned int stream = 0; stream < VLOAD_NUM_STREAMS; ++stream) {
      for (unsigned int i = 0; i < kStreamLen; ++i) {
        const unsigned int idx = stream * kStreamLen + i;
        if (output[idx] != pattern[i]) {
          const uint16_t got = output[idx];
          const uint16_t exp = pattern[i];
          printf("Mismatch at stream %u index %u: got 0x%04x, expected 0x%04x\n",
                 stream, i, got, exp);
          errors = 1;
        }
      }
    }

    printf("\n----- dummy-vload -----\n");
    printf("Execution took %u cycles.\n", timer);
    if (!errors) {
      printf("PASS\n");
    }
  }

  snrt_cluster_hw_barrier();
  return errors ? -1 : 0;
}
