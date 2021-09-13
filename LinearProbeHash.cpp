// Linear Probe Hashing

#define MAX 13
#define N 12
int hashTable[MAX];

void initHashTable(){
	for (int i = 0; i < MAX; i++) {
		hashTable[i] = 0;
	}
}

void insertHash(int hashTable[], int key) {
	int pos = key % MAX;
	while (hashTable[pos % MAX] != 0) {
		pos = pos + 1;
	}
	hashTable[pos % MAX] = key;
}

int searchHash(int hashTable[], int key) {
	int pos = key % MAX;
	while (hashTable[pos % MAX] != key) {
		pos = pos + 1;
	}
	return pos % MAX;
}