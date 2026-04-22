#include "vstore.h"

void vstore_prepare(const uint64_t *input, unsigned int vl) {
  const uint64_t *in0 = input + 0 * vl;
  const uint64_t *in1 = input + 1 * vl;
  const uint64_t *in2 = input + 2 * vl;
  const uint64_t *in3 = input + 3 * vl;
  unsigned int avl;

  asm volatile("vsetvli %0, %1, e64, m8, ta, ma" : "=r"(avl) : "r"(vl));

  asm volatile("vle64.v v0, (%0)" : : "r"(in0) : "v0", "memory");
  asm volatile("vle64.v v8, (%0)" : : "r"(in1) : "v8", "memory");
  asm volatile("vle64.v v16, (%0)" : : "r"(in2) : "v16", "memory");
  asm volatile("vle64.v v24, (%0)" : : "r"(in3) : "v24", "memory");
}

void vstore_test(uint64_t *out0, uint64_t *out1, uint64_t *out2, uint64_t *out3) {
  asm volatile("vse64.v v0, (%0)" : : "r"(out0) : "memory");
  asm volatile("vse64.v v8, (%0)" : : "r"(out1) : "memory");
  asm volatile("vse64.v v16, (%0)" : : "r"(out2) : "memory");
  asm volatile("vse64.v v24, (%0)" : : "r"(out3) : "memory");
}
