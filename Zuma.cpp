#include <stdio.h>
#include <iostream>
#pragma warning(disable:4996)
#define Length 10050

using   namespace   std;
char str[Length];
int count;

bool checkColor(char lr, char color) {

	return true;

}

void insert(int n, int k, char ch) {
	for (int i = n; i >= k; i--) {
		if (i == k) {
			str[k] = ch;
		} else {
			str[i] = str[i - 1];
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
			str[i] = str[i + 3];
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
	for (int i = k; i <= n - 1; i++) {
			str[i - 2] = str[i + 1];
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
			str[i - 1] = str[i + 2];
		}
	}
}

void show(int n) {
	if (str[0] < 'A' || str[0] > 'Z') {
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

void del(int n, int k) {
	int count = 0;
	while (k >= 0 && n >= 0) {
		if (checkrDel(k) && checklDel(k)) {
			printf("dont del:\n");
			break;
		}

		if (checkrDel(k)) {
			printf("right del:\n");
			rDel(n, k);	
			n = n - 3;
			k = k - 1;
			show(n);
			count++;
			printf("count:%d \n", count);
		}

		if (checkrDel(k)) {
			printf("left del:\n");
			lDel(n, k);
			n = n - 3;
			k = k - 1;
			show(n);
			count++;
			printf("count:%d \n", count);
		}
		

		del(n, k);
		printf("n:%d, k:%d", n, k);
	}
}

void handleStr(int n, int k) {
	if (checkcDel(k)) {
		cDel(n, k);
		del(n - 3, k - 2);
	}

	del(n, k);
}


int main()
{
	int count;
	for (int i = 0; i < Length; i++) {
		char c;
		scanf("%c", &c);

		if (c >= 'A' && c <= 'Z') {
			str[i] = c;
		}
		else {
		    count = i;
			break;
		}
	}
	printf("have %d chars\n", count);
	show(count);
	printf("\n=====\n");
	printf("insert:\n\n");

	int k;
	char ch;
	scanf("%d %c", &k, &ch);
	printf("insert %c in %d\n", ch, k);
	insert(count, k, ch);
	count++;
	show(count);

	printf("\n==========\n");
	printf("zuma begin:\n");
	printf("del: \n");
	//handleStr(count, k);
	//rDel(count, k);
	//lDel(count, k);
	del(count, k);
	show(count);

	//int n;
	//scanf("%d", &n);

	//int k;
	//char ch;
	//printf("%d %d %d,", sizeof(str), sizeof(str[0]), sizeof(str) / sizeof(*str));
	//scanf("%d %c", &k, &ch);
	//show(n);

	//for (int i = 0; i < n; i++) {
	//	scanf("%d %c", &k, &ch);
	//	insert(n, k, ch);
	//	show(n);

	//	handleStr(n, k, ch);
	//	show(n);
	//}
	



	system("PAUSE");
}

