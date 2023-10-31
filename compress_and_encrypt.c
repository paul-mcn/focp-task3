#include "huffman_coding.h"
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
  FILE *input_fp = fopen(inputFile, "rb");
  if (!input_fp) {
    fprintf(stderr, "Error opening input file.\n");
    return;
  }

  /* Open the output file for writing */
  FILE *output_fp = fopen(outputFile, "wb");
  if (!output_fp) {
    fclose(input_fp);
    fprintf(stderr, "Error opening output file.\n");
    return;
  }

  /** Get the file size by seeking to the end of the file */
  fseek(input_fp, 0, SEEK_END);
  long file_size = ftell(input_fp);
  rewind(input_fp);

  /** Allocate memory for the file content */
  char *file_contents =
      (char *)malloc(file_size + 1); /** +1 for null-terminator */

  if (file_contents == NULL) {
    perror("Error allocating memory");
    fclose(input_fp);
    return;
  }

  /** Read the file content into the allocated memory */
  size_t bytes_read = fread(file_contents, 1, file_size, input_fp);
  file_contents[bytes_read] = '\0'; /** Null-terminate the string */

  /* Write Huffman Coding compression algorithm first  */

  printf("Compressing with huffman...\n");
  /* char arr[] = {'A', 'B', 'C', 'D'}; */
  char arr[] = {'h', 'e', 'l', 'o', ' ', 'w', 'r', 'd'};
  int freq[] = {1, 1, 3, 2, 1, 1, 1, 1};
  int size = sizeof(arr) / sizeof(arr[0]);
  /* int freq[256] = {0}; */
  /* int i; */
  /* for (i = 0; i < bytes_read; i++) { */
  /*   char c = file_contents[i]; */
  /*   printf("%d %c ", c, c); */
  /*   freq[(int)c]++; */
  /* } */
  /* printf("Freq[]="); */
  /* for (i = 0; i < 256; i++) { */
  /*   printf("%d ", freq[i]); */
  /* } */
  /* printf("before_encryption: %s\n", file_contents); */
  /* HuffmanCodes(file_contents, freq, bytes_read); */
  HuffmanCodes(arr, freq, size);

  /* printf("after_encryption: %s\n", file_contents); */

  /* Write XOR Cipher encryption algorithm last */
  xorCipher(file_contents, secret, bytes_read);
  fwrite(file_contents, 1, bytes_read, output_fp);

  /* Close files */
  fclose(input_fp);
  fclose(output_fp);
}
