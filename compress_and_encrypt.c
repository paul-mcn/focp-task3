#include "huffman_coding.h"
#include "xor_cipher.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compressAndEncryptFile(char *s, char *secret) {
  /* const char *str = "Hello world"; */
  char str[] = "Hello world";
  char *huffmanCode[MAX_TREE_HT] = {0};

  int freq[MAX_TREE_HT] = {0};
  int n = 0;
  char data[MAX_TREE_HT];

  /** Encoding */
  encode(str, freq, n, data, huffmanCode);

  /** Concatenate the codes */
  char *encodedStr = (char *)malloc(sizeof(char) * 1024);
  encodedStr[0] = '\0';
  int i;
  for (i = 0; str[i]; i++) {
    strcat(encodedStr, huffmanCode[(unsigned char)str[i]]);
  }

  printf("Encoded string: %s\n", encodedStr);

  /** Decoding */
  /** Assuming the Huffman tree is still in memory and the root is accessible */
  int size = sizeof(data) / sizeof(data[0]);
  struct MinHeapNode *root = buildHuffmanTree(data, freq, size);
  /** Reuse the function to get the root */
  char *decodedStr = decode(root, encodedStr);
  printf("Decoded string: %s\n", decodedStr);

  /** Clean up */
  for (i = 0; i < MAX_TREE_HT; i++) {
    if (huffmanCode[i]) {
      free(huffmanCode[i]);
    }
  }
  free(encodedStr);
  free(decodedStr);
}
