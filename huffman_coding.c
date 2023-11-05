#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

struct MinHeapNode {
  char data;
  unsigned freq;
  struct MinHeapNode *left, *right;
};

struct MinHeap {
  unsigned size;
  unsigned capacity;
  struct MinHeapNode **array;
};

struct MinHeapNode *newNode(char data, unsigned freq) {
  struct MinHeapNode *temp =
      (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
  temp->left = temp->right = NULL;
  temp->data = data;
  temp->freq = freq;
  return temp;
}

struct MinHeap *createMinHeap(unsigned capacity) {
  struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
  minHeap->size = 0;
  minHeap->capacity = capacity;
  minHeap->array = (struct MinHeapNode **)malloc(minHeap->capacity *
                                                 sizeof(struct MinHeapNode *));
  return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b) {
  struct MinHeapNode *t = *a;
  *a = *b;
  *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx) {
  int smallest = idx;
  int left = 2 * idx + 1;
  int right = 2 * idx + 2;

  if (left < minHeap->size &&
      minHeap->array[left]->freq < minHeap->array[smallest]->freq)
    smallest = left;

  if (right < minHeap->size &&
      minHeap->array[right]->freq < minHeap->array[smallest]->freq)
    smallest = right;

  if (smallest != idx) {
    swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
    minHeapify(minHeap, smallest);
  }
}

int isSizeOne(struct MinHeap *minHeap) { return (minHeap->size == 1); }

struct MinHeapNode *extractMin(struct MinHeap *minHeap) {
  struct MinHeapNode *temp = minHeap->array[0];
  minHeap->array[0] = minHeap->array[minHeap->size - 1];
  --minHeap->size;
  minHeapify(minHeap, 0);
  return temp;
}

void insertMinHeap(struct MinHeap *minHeap, struct MinHeapNode *minHeapNode) {
  ++minHeap->size;
  int i = minHeap->size - 1;

  while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
    minHeap->array[i] = minHeap->array[(i - 1) / 2];
    i = (i - 1) / 2;
  }
  minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {
  int n = minHeap->size - 1;
  int i;
  for (i = (n - 1) / 2; i >= 0; --i) {
    minHeapify(minHeap, i);
  }
}

int isLeaf(struct MinHeapNode *root) { return !(root->left) && !(root->right); }

struct MinHeap *createAndBuildMinHeap(char data[], int freq[], int size) {
  struct MinHeap *minHeap = createMinHeap(size);
  int i;
  for (i = 0; i < size; ++i) {
    minHeap->array[i] = newNode(data[i], freq[i]);
  }
  minHeap->size = size;
  buildMinHeap(minHeap);
  return minHeap;
}

struct MinHeapNode *buildHuffmanTree(char data[], int freq[], int size) {
  struct MinHeapNode *left, *right, *top;
  struct MinHeap *minHeap = createAndBuildMinHeap(data, freq, size);

  while (!isSizeOne(minHeap)) {
    left = extractMin(minHeap);
    right = extractMin(minHeap);
    top = newNode('$', left->freq + right->freq);
    top->left = left;
    top->right = right;
    insertMinHeap(minHeap, top);
  }

  return extractMin(minHeap);
}

/** Stores the Huffman codes in an array of strings */
void storeCodes(struct MinHeapNode *root, int arr[], int top,
                char **huffmanCode) {
  if (root->left) {
    arr[top] = 0;
    storeCodes(root->left, arr, top + 1, huffmanCode);
  }

  if (root->right) {
    arr[top] = 1;
    storeCodes(root->right, arr, top + 1, huffmanCode);
  }

  if (isLeaf(root)) {
    char *code = (char *)malloc(sizeof(char) * (top + 1));
    int i;
    for (i = 0; i < top; i++) {
      code[i] = '0' + arr[i];
    }
    code[top] = '\0';
    huffmanCode[(unsigned char)root->data] = code;
  }
}

void HuffmanCodes(char data[], int freq[], int size, char **huffmanCode) {
  struct MinHeapNode *root = buildHuffmanTree(data, freq, size);
  int arr[MAX_TREE_HT], top = 0;
  storeCodes(root, arr, top, huffmanCode);
}

/** Utility function to calculate frequencies and call HuffmanCodes */
void encode(char *str, int freq[], int n, char data[], char **huffmanCode) {

  /** Calculate frequency and populate the data array */
  int i;
  for (i = 0; str[i]; i++) {
    unsigned char index = (unsigned char)str[i];
    if (freq[index] == 0) {
      data[n++] = str[i];
    }
    freq[index]++;
  }

  HuffmanCodes(data, freq, n, huffmanCode);

  printf("Character   Frequency   Huffman code\n");
  for (i = 0; i < n; i++) {
    printf("%-12c%-12d%s\n", data[i], freq[(unsigned char)data[i]],
           huffmanCode[(unsigned char)data[i]]);
  }
}

/** Decodes the encoded string using the Huffman tree */
char *decode(struct MinHeapNode *root, char *str) {
  char *decodedStr = (char *)malloc(sizeof(char) * (strlen(str) + 1));
  struct MinHeapNode *curr = root;
  int i = 0, j = 0;

  while (str[i]) {
    if (str[i] == '0')
      curr = curr->left;
    else
      curr = curr->right;

    if (isLeaf(curr)) {
      decodedStr[j++] = curr->data;
      curr = root;
    }
    i++;
  }
	printf("ds: %s", decodedStr);
  decodedStr[j] = '\0';
  return decodedStr;
}
