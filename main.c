#include "compress_and_encrypt.h"
#include "xor_cipher.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void batchProcessFiles(char *inputDirectory, char *outputDirectory,
                       int encryptionStrength);

/* Author: Torian Pillay */
int main(int argc, char **argv) {

  /* char inputFile[] = "input.txt"; */
  /* char outputFile[] = "output.txt"; */
  char secret[] = "secret";

  compressAndEncryptFile(argv[1], argv[2], secret);
  compressAndEncryptFile(argv[2], argv[1], secret);
  return 0;
}

void batchProcessFiles(char *inputDirectory, char *outputDirectory,
                       int encryptionStrength) {
  /*
  Batch processes multiple files within a directory using specified methods.

  :param inputDirectory: Path to the directory containing files to be processed.
  :param outputDirectory: Path to the directory where processed files will be
  saved. :param compressionMethod: Selected compression method (e.g., GZIP,
  BZIP2). :param encryptionMethod: Selected encryption method (e.g., AES) for
  securing the files. :param encryptionStrength: Strength or key length for
  encryption.

  Author: author
  */

  /* Function logic */
}
