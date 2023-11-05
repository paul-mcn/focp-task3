#define MAX_SIZE 1024
#define MAX_FILES 6

struct file {
  char name[256];
  char contents[MAX_SIZE];
};

void readFiles(struct file files[], unsigned int *counter);

void batchProcessFiles(char *secret, struct file files[], unsigned int *size);

void batchExportFiles(char *secret, struct file files[], unsigned int *size);

void printFiles(struct file files[], unsigned int size);
