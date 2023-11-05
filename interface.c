#include "interface.h"
#include "compress_and_encrypt.h"
#include "database.h"
#include "xor_cipher.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void startInterface(void) {
  unsigned int choice;
  char *secret = "secret";
  struct file files[MAX_FILES];
  unsigned int file_count = 0;

  printWelcomeMessage();
  readFiles(files, &file_count);

  do {
    printMenu();
    printf("database file_count: %d\n", file_count);
    promptForInt("Enter choice>", &choice);
    switch (choice) {
    case 1:
      batchProcessFiles(secret, files, &file_count);
      break;
    case 2:
      printFiles(files, file_count);
      break;
    case 3:
      batchExportFiles(secret, files, &file_count);
      break;
    case 4:
      printHelpMessage();
      break;
    case 5:
      printf("Bye.\n");
      return;
    default:
      printf("Invalid choice.\n");
      break;
    }
  } while (1);
}

void printWelcomeMessage() {
  printf("==================================================\n"
         "           Welcome to the Encryptor!\n"
         "==================================================\n"
         "This program will help you encrypt and decrypt\n"
         "your text files with state-of-the-art encryption\n"
         "algorithms.\n\n"
         "To get started, follow the prompts in the terminal.\n"
         "If you need help at any point, type '5'.\n\n"
         "Let's secure your data!\n"
         "==================================================\n\n");
}

void printHelpMessage() {
  printf("==================================================\n"
         "                       Help\n"
         "==================================================\n"
         "1. Input file(s):\n"
         "   Enter the paths of the .txt files you wish to input, one per "
         "line.\n"
         "   Each file must have a .txt extension and a unique name that is\n"
         "   not already present in the database. Files will be encrypted "
         "using\n"
         "   the XOR cipher algorithm upon input.\n\n"
         "2. Display file(s):\n"
         "   This option will list all the files currently stored in the "
         "database.\n"
         "   The files will be printed to the standard output (stdout).\n\n"
         "3. Export file(s):\n"
         "   Selecting this option will export all files from the database to "
         "your\n"
         "   local directory, decrypting them using the XOR cipher algorithm. "
         "If a file\n"
         "   with the same name already exists in the directory, the "
         "corresponding file\n"
         "   in the database will not be exported. Ensure that any duplicate "
         "files are\n"
         "   removed to allow for successful export.\n\n"
         "4. Help:\n"
         "   Print this message again.\n\n"
         "5. Exit:\n"
         "   Use this option to exit the program.\n\n"
         "==================================================\n\n");
}

void printMenu() {
  printf("\n========== Encryptor Main Menu ==========\n");
  printf("Select an option by entering its number.\n");
  printf("\n1. Input file(s)\n");
  printf("2. Display file(s)\n");
  printf("3. Export file(s)\n");
  printf("4. Help\n");
  printf("5. Exit\n");
  printf("=========================================\n\n");
}

int promptForInt(const char *message, unsigned int *num) {
  printf("%s", message);
  int result = scanf("%i", num);
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
  return result;
}
