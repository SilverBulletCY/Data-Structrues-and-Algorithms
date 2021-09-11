//LoserTree

#define K 5
#define MIN 0
#define MAX 0xffff

int b[K + 1];
int ls[K];

void adjust(int ls[], int i) {
	int t = (i + K) / 2;
	while (t > 0) {
		if (b[i] > b[ls[t]]) {
			swap(ls[t], i);
		}
		t = t / 2;
	}
	ls[0] = i;
}

void createLoserTree(int ls[]) {
	for (int i = 0; i < K; i++) {
		ls[i] = K;
	}
	for (int i = K - 1; i >= 0; i--) {
		adjust(ls, i);
	}
}

void kMerge(int ls[], int b[], int* data[]) {
	for (int i = 0; i < K; i++) {
		b[i] = *data[i];
	}
	b[K] = MIN;

	createLoserTree(ls);
	cout << b[ls[0]] << endl;

	/*while (b[ls[0]] != MAX) {
		int q = ls[0];
		cout << b[q] << " ";

		adjust(ls, q);
	}
	cout << ls[0] << endl;*/
}

int main() {
	int* data[K];

	int data_0[3] = { 10,15,16 };
	int data_1[3] = { 9,18,20 };
	int data_2[3] = { 20,22,40 };
	int data_3[3] = { 6,15,25 };
	int data_4[3] = { 12,37,48 };
	data[0] = data_0;
	data[1] = data_1;
	data[2] = data_2;
	data[3] = data_3;
	data[4] = data_4;

	kMerge(ls, b, data);
	return 0;
}