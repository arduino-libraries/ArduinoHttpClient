#include "b64.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int main() {
  const unsigned char input[] = "foobar";
  const char* expected[] = {"", "Zg==", "Zm8=", "Zm9v", "Zm9vYg==", "Zm9vYmE=", "Zm9vYmFy"};
  for (int i = 0; i < 7; ++i) {
    const int length = strlen(expected[i]);
    for (int capacity = 0; capacity < length; ++capacity) {
      unsigned char output[16];
      memset(output, 0x5a, sizeof output);
      assert(b64_encode(input, i, output, capacity) == length);
      for (unsigned char byte : output) assert(byte == 0x5a);
    }
    unsigned char output[16];
    memset(output, 0x5a, sizeof output);
    assert(b64_encode(input, i, output, length) == length);
    assert(memcmp(output, expected[i], length) == 0);
    assert(output[length] == 0x5a);
  }
  unsigned char output[4] = {};
  assert(b64_encode((const unsigned char*)"", -1, output, 4) == -1);
  assert(b64_encode((const unsigned char*)"", INT_MAX, output, 4) == -1);
  puts("base64 vectors and all undersized output buffers passed");
}
