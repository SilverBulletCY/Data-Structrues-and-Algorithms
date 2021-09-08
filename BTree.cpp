// Operations on BTree
// Visualization Ref:https://www.cs.usfca.edu/~galles/visualization/BTree.html

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
int getIndexFromParent(BTnode* pos, int key, BTnode* parent);
void deleteInNode(BTnode* root, BTnode* node, int index);
void combineTree_L(BTnode* root, BTnode* pos, int index, int i, BTnode* leftBrother, BTnode* parent);
void combineTree_R(BTnode* root, BTnode* pos, int index, int i, BTnode* rightBrother, BTnode* parent);
void updateTree(BTnode* root, BTnode* pos, BTnode* parent);
void deleteLeafNode(BTnode* root, BTnode* pos, int index, int key);
BTnode* findPre(BTnode* root, BTnode* pos, int index);
void deleteInTree(BTnode* root, int key);
void findDeletePos(BTnode* root, int key, BTnode*& pos, int& index);


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
		insertToNode(root, newRight, j - upIndex - 1, node->key[j]);
	}
	newRight->keynum = node->keynum - upIndex - 1;

	for (int k = 0; k <= upIndex; k++) {
		newLeft->ptr[k] = node->ptr[k];
		if (newLeft->ptr[k] != NULL) {
			newLeft->ptr[k]->parent = newLeft;
		}
	}

	for (int k = upIndex + 1; k <= node->keynum; k++) {
		newRight->ptr[k - upIndex - 1] = node->ptr[k];
		if (newRight->ptr[k - upIndex - 1] != NULL) {
			newRight->ptr[k - upIndex - 1]->parent = newRight;
		}
	}

	parent->ptr[i] = newLeft;
	parent->ptr[i + 1] = newRight;
	newLeft->parent = parent;
	newRight->parent = parent;

	delete node;

	if (parent->keynum >= M) {
		root = splitTree(root, parent);
	}
	return root;
}

int getIndexFromParent(BTnode* pos, int key, BTnode* parent) {
	int i = 0;
	while (parent->key[i] < key && parent->key[i] != 0) i++;
	return i;
}

void deleteInNode(BTnode* root, BTnode* node, int index) {
	for (int i = index; i < node->keynum - 1; i++) {
		node->key[i] = node->key[i + 1];
	}
	node->key[node->keynum - 1] = 0;
	for (int i = index; i < node->keynum; i++) {
		node->ptr[i] = node->ptr[i + 1];
	}
	node->keynum = node->keynum - 1;
}

void combineTree_L(BTnode* root, BTnode* pos, int index, int i, BTnode* leftBrother, BTnode* parent) {
	insertToNode(root, leftBrother, leftBrother->keynum, parent->key[i - 1]);
	deleteInNode(root, parent, i - 1);
	for (int i = 0; i < index; i++) {
		insertToNode(root, leftBrother, leftBrother->keynum, pos->key[i]);
	}
	for (int i = index + 1; i < pos->keynum; i++) {
		insertToNode(root, leftBrother, leftBrother->keynum, pos->key[i]);
	}
	delete pos;
}

void combineTree_R(BTnode* root, BTnode* pos, int index, int i, BTnode* rightBrother, BTnode* parent) {
	insertToNode(root, rightBrother, 0, parent->key[i]);
	deleteInNode(root, parent, i);
	for (int i = 0; i < index; i++) {
		insertToNode(root, rightBrother, 0, pos->key[i]);
	}
	for (int i = index + 1; i < pos->keynum; i++) {
		insertToNode(root, rightBrother, 0, pos->key[i]);
	}
	delete pos;
}

void updateTree(BTnode* root, BTnode* pos, BTnode* parent) {
	while (parent->keynum < MIN && parent != root) {
		BTnode* grandparent = parent->parent;
		if (parent->key[0] < grandparent->key[0]) {
			insertToNode(root, parent, parent->key[parent->keynum], grandparent->key[0]);
			deleteInNode(root, grandparent, 0);
		}
		else {
			insertToNode(root, parent, 0, grandparent->key[grandparent->keynum - 1]);
			deleteInNode(root, grandparent, grandparent->keynum - 1);
		}
		parent = grandparent;
	}
	if (root->keynum == 0) {
		BTnode* pre = findPre(root, root, 0);
		root->key[0] = pre->key[pre->keynum - 1];
		deleteLeafNode(root, pre, pre->keynum - 1, pre->key[pre->keynum - 1]);
	}
}

void deleteLeafNode(BTnode* root, BTnode* pos, int index, int key) {
	int flag;
	BTnode* parent = pos->parent;
	if (pos->keynum > MIN) {
		deleteInNode(root, pos, index);
	}
	else if (pos->keynum == MIN) {
		int i = getIndexFromParent(pos, key, parent);
		BTnode* leftBrother = NULL, * rightBrother = NULL;
		bool parentEnough = (parent->keynum > MIN);
		bool hasLeftBrother = (i > 0);
		if (hasLeftBrother) leftBrother = parent->ptr[i - 1];
		bool hasRightBrother = (i < parent->keynum);
		if (hasRightBrother) rightBrother = parent->ptr[i + 1];
		bool leftBrotherEnough = (hasLeftBrother) && (parent->ptr[i - 1]->keynum > MIN);
		bool rightBrotherEnough = (hasRightBrother) && (parent->ptr[i + 1]->keynum > MIN);
		if (rightBrotherEnough) flag = 1;
		else if (leftBrotherEnough) flag = 2;
		else flag = 3;

		switch (flag) {
		case 1:
			deleteInNode(root, pos, index);
			insertToNode(root, pos, index, parent->key[i]);
			parent->key[i] = rightBrother->key[0];
			deleteInNode(root, rightBrother, 0);
			break;
		case 2:
			deleteInNode(root, pos, index);
			insertToNode(root, pos, index, parent->key[i - 1]);
			parent->key[i - 1] = leftBrother->key[leftBrother->keynum - 1];
			deleteInNode(root, leftBrother, leftBrother->keynum - 1);
			break;
		case 3:
			if (hasLeftBrother) {
				combineTree_L(root, pos, index, i, leftBrother, parent);
			}
			else {
				combineTree_R(root, pos, index, i, rightBrother, parent);
			}
			if (parent->keynum < MIN) {
				updateTree(root, pos, parent);
			}
			break;
		}
	}
}

BTnode* findPre(BTnode* root, BTnode* pos, int index) {
	BTnode* pre = pos;
	pos = pos->ptr[index];
	while (pos != NULL) {
		pre = pos;
		pos = pos->ptr[pos->keynum];
	}
	return pre;
}

void deleteInTree(BTnode* root, int key) {
	BTnode* pos = NULL;
	int index = 0;
	findDeletePos(root, key, pos, index);
	BTnode* left = pos->ptr[index];
	BTnode* right = pos->ptr[index + 1];
	if (left == NULL && right == NULL) {
		deleteLeafNode(root, pos, index, key);
	}
	else {
		BTnode* pre = findPre(root, pos, index);
		int temp = pre->key[pre->keynum - 1];
		deleteLeafNode(root, pre, pre->keynum - 1, temp);
		if (pos->key[index] != key) {
			findDeletePos(root, key, pos, index);
			pos->key[index] = temp;
		}
		else {
			pos->key[index] = temp;
		}
	}
}

void insertToNode(BTnode* root, BTnode* node, int index, int key) {
	for (int i = node->keynum; i > index; i--) {
		node->key[i] = node->key[i - 1];
	}
	node->key[index] = key;
	node->keynum = node->keynum + 1;
	for (int i = M - 1; i > index + 1; i--) {
		node->ptr[i] = node->ptr[i - 1];
	}
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

void findDeletePos(BTnode* root, int key, BTnode*& pos, int& index) {
	int i = 0;
	while (i < root->keynum && root->key[i] < key) i++;
	if (root->key[i] == key) {
		pos = root;
		index = i;
		return;
	}
	else if (root->ptr[i] == NULL) {
		return;
	}
	else {
		findDeletePos(root->ptr[i], key, pos, index);
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