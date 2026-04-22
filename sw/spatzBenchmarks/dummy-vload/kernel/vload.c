#include "vload.h"

void vload_prepare(unsigned int vl) {
  unsigned int avl;

  asm volatile("vsetvli %0, %1, e16, m1, ta, ma" : "=r"(avl) : "r"(vl));
}

void vload_test(const uint16_t *in0, const uint16_t *in1, const uint16_t *in2,
                const uint16_t *in3, const uint16_t *in4, const uint16_t *in5,
                const uint16_t *in6, const uint16_t *in7) {
  asm volatile("vle16.v v16, (%0)" : : "r"(in0) : "v16", "memory");
  asm volatile("vle16.v v17, (%0)" : : "r"(in1) : "v17", "memory");
  asm volatile("vle16.v v18, (%0)" : : "r"(in2) : "v18", "memory");
  asm volatile("vle16.v v19, (%0)" : : "r"(in3) : "v19", "memory");
  asm volatile("vle16.v v20, (%0)" : : "r"(in4) : "v20", "memory");
  asm volatile("vle16.v v21, (%0)" : : "r"(in5) : "v21", "memory");
  asm volatile("vle16.v v22, (%0)" : : "r"(in6) : "v22", "memory");
  asm volatile("vle16.v v23, (%0)" : : "r"(in7) : "v23", "memory");
}

void vload_finish(uint16_t *out0, uint16_t *out1, uint16_t *out2, uint16_t *out3,
                  uint16_t *out4, uint16_t *out5, uint16_t *out6, uint16_t *out7) {
  asm volatile("vse16.v v16, (%0)" : : "r"(out0) : "memory");
  asm volatile("vse16.v v17, (%0)" : : "r"(out1) : "memory");
  asm volatile("vse16.v v18, (%0)" : : "r"(out2) : "memory");
  asm volatile("vse16.v v19, (%0)" : : "r"(out3) : "memory");
  asm volatile("vse16.v v20, (%0)" : : "r"(out4) : "memory");
  asm volatile("vse16.v v21, (%0)" : : "r"(out5) : "memory");
  asm volatile("vse16.v v22, (%0)" : : "r"(out6) : "memory");
  asm volatile("vse16.v v23, (%0)" : : "r"(out7) : "memory");
}
