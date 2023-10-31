
#define MAX_TREE_HT 50

struct MinHNode;
struct MinHeap;

/** Create nodes */
struct MinHNode *newNode(char item, unsigned freq);

/** Create min heap */
struct MinHeap *createMinH(unsigned capacity);

/** Function to swap */
void swapMinHNode(struct MinHNode **a, struct MinHNode **b);

/** Heapify */
void minHeapify(struct MinHeap *minHeap, int idx);

/** Check if size if 1 */
int checkSizeOne(struct MinHeap *minHeap);

/** Extract min */
struct MinHNode *extractMin(struct MinHeap *minHeap);

/** Insertion function */
void insertMinHeap(struct MinHeap *minHeap, struct MinHNode *minHeapNode);

void buildMinHeap(struct MinHeap *minHeap);

int isLeaf(struct MinHNode *root);

struct MinHeap *createAndBuildMinHeap(char item[], int freq[], int size);

struct MinHNode *buildHuffmanTree(char item[], int freq[], int size);

void printHCodes(struct MinHNode *root, int arr[], int top);

/** Wrapper function */
void HuffmanCodes(char item[], int freq[], int size);

/** Print the array */
void printArray(int arr[], int n);
