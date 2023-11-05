
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 256

struct MinHeapNode;

struct MinHeap;

struct MinHeapNode *newNode(char data, unsigned freq);

struct MinHeap *createMinHeap(unsigned capacity);

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b);

void minHeapify(struct MinHeap *minHeap, int idx);

int isSizeOne(struct MinHeap *minHeap);

struct MinHeapNode *extractMin(struct MinHeap *minHeap);

void insertMinHeap(struct MinHeap *minHeap, struct MinHeapNode *minHeapNode);

void buildMinHeap(struct MinHeap *minHeap);

int isLeaf(struct MinHeapNode *root);

struct MinHeap *createAndBuildMinHeap(char data[], int freq[], int size);

struct MinHeapNode *buildHuffmanTree(char data[], int freq[], int size);

/** Stores the Huffman codes in an array of strings */
void storeCodes(struct MinHeapNode *root, int arr[], int top,
                char **huffmanCode);

void HuffmanCodes(char data[], int freq[], int size, char **huffmanCode);

/** Utility function to calculate frequencies and call HuffmanCodes */
void encode(char *str, int freq[], int n, char data[], char **huffmanCode);

/** Decodes the encoded string using the Huffman tree */
char *decode(struct MinHeapNode *root, char *str);
