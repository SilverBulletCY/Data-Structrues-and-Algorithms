// Operations on BTree

// 每个结点最多有M棵子树
// 若根结点不是叶子结点，则至少有两棵子树
// 除根之外的所有非终端结点至少有⌈M/2⌉棵子树

# define M 3
# define MAX M - 1
# define MIN ceil(M / 2.0) - 1

struct BTnode {
	int keynum;
	BTnode* parent;
	int key[M + 1];
	BTnode* ptr[M + 1];
};

BTnode* newNode(int key);
BTnode* splitTree(BTnode* root, BTnode* node);
void insertToNode(BTnode* root, BTnode* node, int index, int key);
void search(BTnode* root, int key, BTnode*& pos, int& index);
void insertToTree(BTnode*& root, int key);
BTnode* createBTree(int data[], int n);
void traversal(BTnode* root);


BTnode* newNode(int key) {
	BTnode* node = new BTnode;
	node->keynum = 1;
	node->parent = NULL;
	for (int i = 0; i < M + 1; i++) {
		node->key[i] = 0;
		node->ptr[i] = NULL;
	}
	node->key[0] = key;
	return node;
}

BTnode* splitTree(BTnode* root, BTnode* node) {
	int upIndex = node->keynum / 2;
	int upValue = node->key[upIndex];
	BTnode* parent = node->parent;

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

	BTnode* newLeft = newNode(node->key[0]);
	for (int j = 1; j < upIndex; j++) {
		insertToNode(root, newLeft, j, node->key[j]);
	}
	newLeft->keynum = upIndex;

	BTnode* newRight = newNode(node->key[upIndex + 1]);
	for (int j = upIndex + 2; j < node->keynum; j++) {
		insertToNode(root, newRight, j, node->key[j - upIndex - 1]);
	}
	newRight->keynum = node->keynum - upIndex - 1;

	for (int k = 0; k <= upIndex; k++) {
		newLeft->ptr[k] = node->ptr[k];
	}

	for (int k = upIndex + 1; k <= node->keynum; k++) {
		newRight->ptr[k - upIndex - 1] = node->ptr[k];
	}

	parent->ptr[i] = newLeft;
	parent->ptr[i + 1] = newRight;
	newLeft->parent = parent;
	newRight->parent = parent;

	delete node;

	if (parent->keynum >= M) {
		root = splitTree(root, parent);
		parent->keynum = parent->keynum - 1;
	}
	return root;
}

void insertToNode(BTnode* root, BTnode* node, int index, int key) {
	for (int i = node->keynum; i > index; i--) {
		node->key[i] = node->key[i - 1];
	}
	node->key[index] = key;
	node->keynum = node->keynum + 1;
}

void search(BTnode* root, int key, BTnode*& pos, int& index) {
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

void insertToTree(BTnode*& root, int key) {
	BTnode* pos = NULL;
	int index = 0;
	search(root, key, pos, index);
	if (pos == NULL) {
		pos = newNode(key);
		root = pos;
	}
	else {
		insertToNode(root, pos, index, key);
		if (pos->keynum >= M) {
			root = splitTree(root, pos);
			pos->keynum = pos->keynum - 1;
		}
	}
}

BTnode* createBTree(int data[], int n) {
	BTnode* root = NULL;
	for (int i = 0; i < n; i++) {
		insertToTree(root, data[i]);
	}
	return root;
}

void traversal(BTnode* root) {
	if (root == NULL) return;
	else {
		for (int i = 0; i < root->keynum; i++) {
			traversal(root->ptr[i]);
			cout << root->key[i] << " ";
		}
		traversal(root->ptr[root->keynum]);
	}
}