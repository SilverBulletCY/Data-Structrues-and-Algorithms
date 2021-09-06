```c++
// Operations on AVLTree
struct node {
	int data;
	int height;
	node* lchild;
	node* rchild;
};

node* newNode(int key);
int getHeight(node* root);
void updateHeight(node* root);
int getBalanceFactor(node* root);
void L_Rotation(node*& root);
void R_Rotation(node*& root);
void insert(node*& root, int key);
node* createAVLTree(vector<int> v, int n);
void inOrder(node* root);
node* search(node* root, node*& pre, int key);
void deleteLeafNode(node* pos, node* pre);
void deleteNode(node* root, int key);


node* newNode(int key) {
	node* Node = new node;
	Node->data = key;
	Node->height = 1;
	Node->lchild = NULL;
	Node->rchild = NULL;
	return Node;
}

int getHeight(node* root) {
	if (root == NULL) return 0;
	else return root->height;
}

int getBalanceFactor(node* root) {
	return getHeight(root->lchild) - getHeight(root->rchild);
}

void updateHeight(node* root) {
	root->height = max(getHeight(root->lchild), getHeight(root->rchild)) + 1;
}

void L_Rotation(node*& root) {
	node* temp = root->rchild;
	root->rchild = temp->lchild;
	temp->lchild = root;
	updateHeight(root);
	updateHeight(temp);
	root = temp;
}

void R_Rotation(node*& root) {
	node* temp = root->lchild;
	root->lchild = temp->rchild;
	temp->rchild = root;
	updateHeight(root);
	updateHeight(temp);
	root = temp;
}

void insert(node*& root, int key) {
	if (root == NULL) {
		root = newNode(key);
		return;
	}
	else if (key < root->data) {
		insert(root->lchild, key);
		if (getBalanceFactor(root) == 2) {
			if (getBalanceFactor(root->lchild) == 1) {
				R_Rotation(root);
			}
			else if (getBalanceFactor(root->lchild) == -1) {
				L_Rotation(root->lchild);
				R_Rotation(root);
			}
		}
	}
	else {
		insert(root->rchild, key);
		if (getBalanceFactor(root) == -2) {
			if (getBalanceFactor(root->rchild) == 1) {
				R_Rotation(root->rchild);
				L_Rotation(root);
			}
			else if (getBalanceFactor(root->lchild) == -1) {
				L_Rotation(root);
			}
		}
	}
}

node* createAVLTree(vector<int> v, int n) {
	node* root = NULL;
	for (int i = 0; i < n; i++) {
		insert(root, v[i]);
	}
	return root;
}

void inOrder(node* root) {
	if (root == NULL) {
		return;
	}
	else {
		inOrder(root->lchild);
		cout << root->data << " ";
		inOrder(root->rchild);
	}
}

node* search(node* root, node*& pre, int key) {
	if (root == NULL) {
		pre = NULL;
		return NULL;
	}
	else if (key == root->data) {
		return root;
	}
	else if (key < root->data) {
		return search(root->lchild, root, key);
	}
	else {
		return search(root->rchild, root, key);
	}
}

void deleteLeafNode(node* pos, node* pre) {
	delete pos;
	if (pre->lchild != NULL) {
		pre->lchild = NULL;
	}
	else {
		pre->rchild = NULL;
	}
}

void deleteNode(node* root, int key) {
	node* pre = NULL;
	node* pos = search(root, pre, key);
	if (pos == NULL) {
		return;
	}
	else {
		if (pos->lchild == NULL && pos->rchild == NULL) {
			deleteLeafNode(pos, pre);
		}
		else if (pos->lchild == NULL) {
			pos->data = pos->rchild->data;
			deleteLeafNode(pos->rchild, pos);
		}
		else if (pos->rchild == NULL) {
			pos->data = pos->lchild->data;
			deleteLeafNode(pos->lchild, pos);
		}
		else {
			node* cur = pos;
			pre = pos;
			pos = pos->lchild;
			while (pos->rchild) {
				pre = pos;
				pos = pos->rchild;
			}
			cur->data = pos->data;
			deleteLeafNode(pos, pre);
		}
	}
}
```

