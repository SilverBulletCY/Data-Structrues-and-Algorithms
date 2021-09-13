// Operations on BPlusTree
// Visualization Ref:https://www.cs.usfca.edu/~galles/visualization/BTree.html

#define M 4
#define MAX M-1
#define MIN ceil(M / 2.0) - 1

struct BPNode {
	int keynum;
	BPNode* parent;
	int key[M + 1];
	BPNode* ptr[M + 1];
};

BPNode* newNode(int key);
BPNode* splitTree(BPNode* root, BPNode* node);
BPNode* findPreListNode(BPNode* root, BPNode* node);
BPNode* findPostListNode(BPNode* root, BPNode* node);
int getIndexFromParent(BPNode* pos, int key, BPNode* parent);
void search(BPNode* root, int key, BPNode*& pos, int& index);
void insertToNode(BPNode*& root, BPNode* node, int index, int key);
void insertToTree(BPNode*& root, BPNode*& listHead, int key);
void print(BPNode* listHead);


BPNode* newNode(int key) {
	BPNode* node = new BPNode;
	node->keynum = 1;
	node->parent = NULL;
	for (int i = 0; i < M + 1; i++) {
		node->key[i] = 0;
		node->ptr[i] = NULL;
	}
	node->key[0] = key;
	return node;
}

BPNode* splitTree(BPNode* root, BPNode* node) {
	int upIndex = node->keynum / 2;
	int upValue = node->key[upIndex];
	BPNode* parent = node->parent;

	int i = 0;
	if (parent == NULL) {
		parent = newNode(upValue);
		root = parent;
	}
	else {
		i = 0;
		while (i < parent->keynum && parent->key[i] < upValue) i++;
		insertToNode(root, parent, i, upValue);
	}

	BPNode* newLeft = newNode(node->key[0]);
	for (int j = 1; j < upIndex; j++) {
		insertToNode(root, newLeft, j, node->key[j]);
	}
	newLeft->keynum = upIndex;

	BPNode* newRight = newNode(node->key[upIndex]);
	for (int j = upIndex + 1; j < node->keynum; j++) {
		insertToNode(root, newRight, j - upIndex, node->key[j]);
	}
	newRight->keynum = node->keynum - upIndex;

	for (int k = 0; k <= upIndex; k++) {
		newLeft->ptr[k] = node->ptr[k];
		if (newLeft->ptr[k] != NULL) {
			newLeft->ptr[k]->parent = newLeft;
		}
	}

	for (int k = upIndex + 1; k <= node->keynum && k != M; k++) {
		newRight->ptr[k - upIndex - 1] = node->ptr[k];
		if (newRight->ptr[k - upIndex - 1] != NULL) {
			newRight->ptr[k - upIndex - 1]->parent = newRight;
		}
	}

	parent->ptr[i] = newLeft;
	parent->ptr[i + 1] = newRight;
	newLeft->parent = parent;
	newRight->parent = parent;

	if (newLeft->ptr[0] == NULL) {
		newLeft->ptr[M] = newRight;

		if ((i != 0) && (i != parent->keynum - 1)) {
			parent->ptr[i - 1]->ptr[M] = newLeft;
			newRight->ptr[M] = parent->ptr[i + 2];			
		}
		else if (i == 0) {
			BPNode* preLeft = findPreListNode(root, newLeft);
			if(preLeft) preLeft->ptr[M] = newLeft;
			newRight->ptr[M] = parent->ptr[i + 2];
		}
		else {
			BPNode* postRight = findPostListNode(root,newRight);
			parent->ptr[i - 1]->ptr[M] = newLeft;
			if (postRight) newRight->ptr[M] = postRight;
		}
	}

	delete node;

	while (parent->keynum >= M) {
		root = splitTree(root, parent);
	}
	return root;
}

BPNode* findPreListNode(BPNode* root, BPNode* node) {
	BPNode* temp = node;
	while (temp->parent != NULL && temp->parent->ptr[0] == temp) {
		temp = temp->parent;
	}
	if (temp == root) {
		return NULL;
	}
	else {
		int index = getIndexFromParent(temp, temp->key[1], temp->parent);
		temp = temp->parent->ptr[index - 1];
		while (temp->ptr[temp->keynum]) {
			temp = temp->ptr[temp->keynum];
		}
	}
	return temp;
}

BPNode* findPostListNode(BPNode* root, BPNode* node) {
	BPNode* temp = node;
	while (temp->parent != NULL && temp->parent->ptr[temp->parent->keynum] == temp) {
		temp = temp->parent;
	}
	if (temp == root) {
		return NULL;
	}
	else {
		int index = getIndexFromParent(temp, temp->key[1], temp->parent);
		temp = temp->parent->ptr[index + 1];
		while (temp->ptr[0]) {
			temp = temp->ptr[0];
		}
	}
	return temp;
}

int getIndexFromParent(BPNode* pos, int key, BPNode* parent) {
	int i = 0;
	while (parent->key[i] < key && parent->key[i] != 0) i++;
	return i;
}

void search(BPNode* root, int key, BPNode*& pos, int& index) {
	if (root == NULL) {
		return;
	}
	else {
		int i = 0;
		while (i < root->keynum && root->key[i] < key) i++;
		index = i;
		pos = root;
		search(root->ptr[i], key, pos, index);
	}
}

void insertToNode(BPNode*& root, BPNode* node, int index, int key) {
	for (int i = node->keynum; i > index; i--) {
		node->key[i] = node->key[i - 1];
	}
	node->key[index] = key;
	node->keynum = node->keynum + 1;
	for (int i = M - 1; i > index + 1; i--) {
		node->ptr[i] = node->ptr[i - 1];
	}
}

void insertToTree(BPNode*& root, BPNode*& listHead, int key) {
	BPNode* pos = NULL;
	int index = 0;
	search(root, key, pos, index);
	if (pos == NULL) {
		pos = newNode(key);
		root = pos;
		listHead = pos;
	}
	else {
		insertToNode(root, pos, index, key);
		if (pos->keynum >= M) {
			root = splitTree(root, pos);
			BPNode* temp = root;
			while (temp->ptr[0]) {
				temp = temp->ptr[0];
			}
			listHead = temp;
		}
	}
}

void print(BPNode* listHead) {
	BPNode* current = listHead;
	while (current) {
		for (int i = 0; i < current->keynum; i++) {
			cout << current->key[i] << " ";
		}
		current = current->ptr[M];
	}
}