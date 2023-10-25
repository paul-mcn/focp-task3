#include <string.h>

void xorCipher(char *input, char *key) {
  int input_len = strlen(input);
  int key_len = strlen(key);
  int i;

  for (i = 0; i < input_len; i++) {
    input[i] = input[i] ^ key[i % key_len];
  }
}
