#include <stdint.h>

#ifndef VLOAD_H
#define VLOAD_H

enum {
  VLOAD_NUM_STREAMS = 8,
};

void vload_prepare(unsigned int vl);
void vload_test(const uint16_t *in0, const uint16_t *in1, const uint16_t *in2,
                const uint16_t *in3, const uint16_t *in4, const uint16_t *in5,
                const uint16_t *in6, const uint16_t *in7);
void vload_finish(uint16_t *out0, uint16_t *out1, uint16_t *out2, uint16_t *out3,
                  uint16_t *out4, uint16_t *out5, uint16_t *out6, uint16_t *out7);

#endif
