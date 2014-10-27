#include <stdio.h>
#include <iostream>
#define Length 10050

using   namespace   std;
int str[Length];

bool checkColor(char lr, char color) {

	return true;

}

void insert(int n, int k, char ch) {
	for (int i = n; i > k; i--) {
		str[n] = str[n - 1];
		if (i == k) {
			str[i] = ch;
		}
	}
}

bool checkrDel(int k) {
	if (str[k] == str[k + 1] && str[k] == str[k + 2]) {
		return true;
	}
	return false;
	
}

void rDel(int n, int k) {
	if (str[k] == str[k + 1] && str[k] == str[k + 2]) {
		for (int i = k; i <= n - 3; i++) {
			str[k] = str[k + 3];
		}
	}
}

bool checklDel(int k) {
	if (str[k] == str[k - 1] && str[k] == str[k - 2] && k - 2 >= 0) {
		return true;
	}
	return false;
}

void lDel(int n, int k) {
	for (int i = k; i <= n - 3; i++) {
			str[k - 2] = str[k + 1];
		}
}

bool checkcDel(int k) {
	if (str[k] == str[k - 1] && str[k] == str[k + 1]) {
		return true;
	}

	return false;
}

void cDel(int n, int k) {
	if (str[k] == str[k - 1] && str[k] == str[k + 1]) {
		for (int i = k; i <= n - 3; i++) {
			str[k - 1] = str[k + 2];
		}
	}
}

void show(int n) {
	if (str[0] <= 'A' || str[0] >= 'Z') {
		printf("-\n");
	}
	for (int i = 0; i < n; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			printf("%c", str[i]);
		} else {
			break;
		}
	}
	printf("\n");
}

int del(int n, int k) {
	int count = 0;
	while (k >= 0 && n >= 0) {
		if (!(checkrDel(k) && checklDel(k))) {
			break;
		}

		if (checkrDel(k)) {
			rDel(n, k);	
			count++;
		}

		if (checkrDel(k)) {
			lDel(n, k);
			count++;
		}

		del(n - 3, k - 1);
		k--;
	}
}

void handleStr(int n, int k, char ch) {
	if (checkcDel(k)) {
		cDel(n, k);
		del(n - 3, k - 2);
	}

	del(n, k);
}


int main()
{
	for (int i = 0; i < Length; i++) {
		char c;
		scanf("%c", &c);

		if (c >= 'A' && c <= 'Z') {
			str[i] = c;
		}
		else {
			break;
		}

		int n;
		scanf("%d", &n);

		int k;
		char ch;

		for (int i = 0; i < n; i++) {
			scanf("%d %c", &k, &ch);

			handleStr(n, k, ch);
			show(n);
		}
	}
}

