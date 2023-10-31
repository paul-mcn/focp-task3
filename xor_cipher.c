#include <stdio.h>
#include <string.h>

void xorCipher(char *input, const char *key, size_t length) {
  size_t i;
  for (i = 0; i < length; i++) {
    input[i] ^= key[i % strlen(key)];
  }
}
