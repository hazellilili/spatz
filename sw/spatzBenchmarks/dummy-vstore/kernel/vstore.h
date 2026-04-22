#include <stdint.h>

#ifndef VSTORE_H
#define VSTORE_H

enum {
  VSTORE_NUM_STREAMS = 4,
};

void vstore_prepare(const uint64_t *input, unsigned int vl);
void vstore_test(uint64_t *out0, uint64_t *out1, uint64_t *out2, uint64_t *out3);

#endif
