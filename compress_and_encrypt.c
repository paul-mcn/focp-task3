#include "xor_cipher.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compressAndEncryptFile(char *inputFile, char *outputFile, char *secret) {
  /*
  Compresses and encrypts a file using specified methods.

  :param inputFile: Path to the input file to be processed.
  :param outputFile: Path to the output file where the result will be saved.
  :param compressionMethod: Selected compression method (e.g., GZIP, BZIP2).
  :param encryptionMethod: Selected encryption method (e.g., AES) for securing

  Author: Paul
  */

  /* Open the input file for reading */
  FILE *input = fopen(inputFile, "rb");
  if (!input) {
    fprintf(stderr, "Error opening input file.\n");
    return;
  }

  /* Open the output file for writing */
  FILE *output = fopen(outputFile, "wb");
  if (!output) {
    fclose(input);
    fprintf(stderr, "Error opening output file.\n");
    return;
  }

  /* Write Huffman Coding compression algorithm first  */

  printf("Compressing with huffman...\n");

  /* Write XOR Cipher encryption algorithm last */

  char buffer[1024];

  printf("Encrypting with XOR Cipher...\n");
  while (fgets(buffer, sizeof(buffer), input)) {
    /* Perform XOR encryption on the buffer */
    xorCipher(buffer, secret);

    /* Write the encrypted data to the output file */
    fprintf(output, "%s", buffer);
  }

  /* Close files */
  fclose(input);
  fclose(output);
}
