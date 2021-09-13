// Chained Hashing

#define MAX 13
#define N 12

struct node{
	int data;
	node* next;
};

struct HashTable {
	node* hashTable[MAX];
};

node* newNode(int key);
HashTable* initHashTable();
void insertHash(HashTable* h, int key);
bool searchHash(HashTable* h, int key);


node* newNode(int key) {
	node* Node = new node;
	Node->data = key;
	Node->next = NULL;
	return Node;
}

HashTable* initHashTable(){
	HashTable* h = new HashTable;
	for (int i = 0; i < MAX; i++) {
		h->hashTable[i] = NULL;
	}
	return h;
}

void insertHash(HashTable* h, int key) {
	int pos = key % MAX;
	node* temp = newNode(key);
	if (h->hashTable[pos] == NULL) {
		h->hashTable[pos] = temp;
	}
	else {
		node* p = h->hashTable[pos];
		while (p->next) {
			p = p->next;
		}
		p->next = temp;
	}

}

bool searchHash(HashTable* h, int key) {
	int pos = key % MAX;
	node* q = h->hashTable[pos];
	while (q) {
		if (q->data == key) {
			return true;
		}
		q = q->next;
	}
	return false;
}