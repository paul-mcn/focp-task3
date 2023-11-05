#include "database.h"
#include "compress_and_encrypt.h"
#include "xor_cipher.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isSpace(const char *str) {
  while (*str != '\0') {
    if (*str == ' ') {
      return 1;
    }
    str++;
  }
  return 0;
}

int isTextFile(char *filename) {
  if (filename == NULL) {
    return 0;
  }

  const char *extension = ".txt";
  size_t filename_len = strlen(filename);
  size_t extension_len = strlen(extension);

  /** Check if the filename is shorter than the extension */
  if (filename_len < extension_len) {
    return 0;
  }

  /** Compare the end of the filename with the extension */
  if (strcmp(filename + filename_len - extension_len, extension) == 0) {
    return 1;
  }
  return 0;
}

int isFileInDatabase(const char *substring) {
  FILE *file = fopen("database", "r"); /** Open the file in read mode */
  if (file == NULL) {
    perror("Error opening file");
    return 0;
  }

  int found = 0;
  char buffer[1024]; /** Buffer to hold chunks of file content */

  /** Read file contents chunk by chunk */
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    /** Check if the current chunk contains the substring */
    if (strstr(buffer, substring) != NULL) {
      fclose(file);
      return 1;
    }
  }

  fclose(file);
  return found;
}

int isFileEmpty(const char *filename) {
  FILE *file = fopen(filename, "r"); /** Open the file in read mode */
  if (file == NULL) {
    perror("Error opening file");
    return 1; /** Assuming file is empty if it can't be opened */
  }

  int firstChar = fgetc(file); /** Read the first character */
  fclose(file);                /** Close the file */

  return firstChar == EOF; /** If first character is EOF, file is empty */
}

int shouldFileExist(const char *filename) {
  FILE *fp = fopen(filename, "r"); /** Open the file in read mode */
  if (fp == NULL) {
    return 0; /** Assuming file is empty if it can't be opened */
  }
  fclose(fp);
  return 1;
}

int scanStr(char *s) {
  int result = scanf("%s", s);
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
  return result;
}

int validateFileName(char filename[256]) {
  if (!isTextFile(filename)) {
    printf("Error: \"%s\" does not end in .txt, Please try again\n", filename);
    return 0;
  } else if (isSpace(filename)) {
    printf("Error: \"%s\" contains a space.\n", filename);
    return 0;
  } else if (isFileEmpty(filename)) {
    printf("Error: \"%s\" is empty\n", filename);
    return 0;
  } else if (isFileInDatabase(filename)) {
    printf("Error: \"%s\" is already in database\n", filename);
    return 0;
  }
  return 1;
}

int promptForFileLoop(struct file files[], unsigned int size) {
  if (size == MAX_FILES) {
    printf("You have reached the maximum file limit of 6. Please delete some "
           "files from the database to free up space for new ones.\n");
    return size;
  }
  printf("Enter filenames (Files stored: %d of 6 | Type -1 to go back | Files "
         "seperated by \\n )>\n",
         size);

  int new_files = 0;
  while (size < MAX_FILES) {
    scanStr(files[size].name);
    if (strcmp(files[size].name, "-1") == 0) {
      printf("Exiting at count=%d", size);
      return new_files;
    } else if (validateFileName(files[size].name)) {
      new_files++;
      size++;
      printf("New files: %d\n", new_files);
    }
  }
  return new_files;
}

int removeFilesFromArray(struct file files[], unsigned int size,
                         int files_to_remove[MAX_FILES]) {
  int i, write_idx = 0;
  for (i = 0; i < size; i++) {
    if (!files_to_remove[i]) {
      /* prtinf("\nReplacing %s with %s", files[write_idx]) */
      /* memcpy(&files[i], &files[i + 1], sizeof(files)); */
      files[write_idx] = files[i];
      write_idx++;
    }
  }
  return write_idx;
}

void removeFileFromDatabase(const char *text) {
  const char *db_filename = "database";
  const char *temp_filename = "temp";
  FILE *db_fp = fopen(db_filename, "r");
  if (db_fp == NULL) {
    perror("Error opening source file");
    return;
  }

  FILE *temp_fp = fopen(temp_filename, "w");
  if (temp_fp == NULL) {
    perror("Error opening temporary file");
    fclose(db_fp);
    return;
  }

  char buffer[1024];
  while (fgets(buffer, sizeof(buffer), db_fp) != NULL) {
    /** If the line doesn't contain the text, write it to the temp file */
    if (strstr(buffer, text) == NULL) {
      fputs(buffer, temp_fp);
    }
  }

  fclose(db_fp);
  fclose(temp_fp);

  /** Remove the original file and rename the temp file to the original file */
  remove(db_filename);
  rename(temp_filename, db_filename);
}

void exportFiles(struct file files[], unsigned int *size) {
  int i;
  int files_to_remove[MAX_FILES] = {0};
  for (i = 0; i < *size; i++) {
    /* First, we open the file to check if it exists */
    if (shouldFileExist(files[i].name)) {
      fprintf(
          stderr,
          "Error: \"%s\" already exists, please delete it before exporting\n",
          files[i].name);
    } else {
      /** Create a new file and move contents from the database to the file */
      FILE *output_fp = fopen(files[i].name, "a+");
      if (!output_fp) {
        fprintf(stderr, "Error: Something went wrong creating \"%s\".\n",
                files[i].name);
      }
      fprintf(output_fp, "%s\n", files[i].contents);
      fclose(output_fp);
      removeFileFromDatabase(files[i].name);
      files_to_remove[i] = 1;
    }
  }
  int removed_files = removeFilesFromArray(files, *size, files_to_remove);
  *size = removed_files;
}

void saveFiles(struct file files[], unsigned int *size,
               unsigned int new_files_count, char *secret) {
  /** Ensure we don't read more than 6 files */
  if (*size > MAX_FILES) {
    printf("Error: Can only read up to 6 files.\n");
    return;
  }

  /* Open the database file for writing, if it doesnt exist, create one */
  FILE *output_fp = fopen("database", "a+");
  if (!output_fp) {
    fprintf(stderr, "Error opening output file.\n");
    return;
  }

  const unsigned int max_size = *size + new_files_count;

  printf("\nsize=%d max=%d, new=%d\n", *size, max_size, new_files_count);
  while (*size < max_size) {

    /* Open the input file for reading */
    FILE *input_fp = fopen(files[*size].name, "rb");
    if (!input_fp) {
      fprintf(stderr, "Error opening file. %s\n", files[*size].name);
      return;
    }

    /** Get the file size by seeking to the end of the file */
    fseek(input_fp, 0, SEEK_END);
    long file_size = ftell(input_fp);
    rewind(input_fp);

    /** Allocate memory for the file content. +1 for null-terminator  */
    char *file_contents = (char *)malloc(file_size + 1);

    if (file_contents == NULL) {
      perror("Error allocating memory");
      fclose(input_fp);
      return;
    }

    /** Read the file content into the allocated memory */
    size_t bytes_read = fread(file_contents, 1, file_size, input_fp);

    /** Null-terminate the string */
    file_contents[bytes_read] = '\0';

    strcpy(files[*size].contents, file_contents);
    /* xorCipher(file_contents, secret, bytes_read); */
    fprintf(output_fp, "%s %s", files[*size].name, file_contents);

    free(file_contents);
    fclose(input_fp);

    (*size)++;
  }
  /* Close files */
  fclose(output_fp);
}

void readFiles(struct file files[], unsigned int *counter) {
  FILE *fp = NULL;
  fp = fopen("database", "r");
  if (fp == NULL) {
    printf("Error: could not open database file");
    return;
  }
  unsigned int i = 0;
  char line[512];
  while (fgets(line, sizeof(line), fp) && i < MAX_FILES) {
    printf("line: %s", line);
    char *name = strtok(line, " ");
    char *contents = strtok(NULL, "\n");

    if (name && contents) {
      /** Remove leading/trailing spaces */

      name[strcspn(name, " ")] = 0;          /** Remove spaces from name */
      contents[strcspn(contents, "\n")] = 0; /** Remove newline from contents */

      strncpy(files[i].name, name, sizeof(files[i].name));
      strncpy(files[i].contents, contents, sizeof(files[i].contents));
    }
    i++;
  }
  fclose(fp);
  /** For demonstration: print the contents of the files array */
  int j;
  for (j = 0; j < i; j++) {
    printf("\nFile: %s\nContents: %s\n", files[j].name, files[j].contents);
  }
  *counter = i;
}

void encryptDecryptFiles(const char *secret, struct file files[],
                         unsigned int size) {
  int i;
  for (i = 0; i < size; i++) {
    size_t contents_size = sizeof(files[i].contents) / sizeof(char);
    xorCipher(files[i].contents, secret, contents_size);
  }
}

void printFiles(struct file files[], unsigned int file_count) {
  if (file_count == 0) {
    printf("File list empty.\n");
    return;
  }
  printf("File name | contents\n"
         "---------- ---------- \n");
  int i;
  for (i = 0; i < file_count; i++) {
    printf("%s | %s\n", files[i].name, files[i].contents);
  }
  printf("\nTotal files: %d\n", file_count);
}

void batchProcessFiles(char *secret, struct file files[], unsigned int *size) {
  unsigned int new_file_count = promptForFileLoop(files, *size);
  printf("\nfiles found: %d\n", new_file_count);
  saveFiles(files, size, new_file_count, secret);
}

void batchExportFiles(char *secret, struct file files[], unsigned int *size) {
  printf("filecount: %d", *size);
  printf("Exporting files...\n");
  /* encryptDecryptFiles(secret, files, *size); */
  exportFiles(files, size);
}
