#include <stdio.h>
#include <iostream>
#define Length 500050

using   namespace   std;
int arr[Length];

void quickSort(int* arr, int left, int right)
{
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];

	while (i <= j) {
		while (arr[i] < pivot)
			i++;
		while (arr[j] > pivot)
			j--;
		if (i <= j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
			i++;
			j--;
		}
	}

	if (left < j)
		quickSort(arr, left, j);
	if (i < right)
		quickSort(arr, i, right);
}

int midpoint(int imin, int imax) {
	int key;
	key = imin + (imax - imin) / 2;
	return key;
}

int binarySearch(int key, int min, int max)
{
	int mid;

	while (max >= min)
	{
		mid = midpoint(min, max);
	    if (arr[mid] < key)
			min = mid + 1;
		else
			max = mid - 1;
	}
	return min;
}


int main()
{
	int m, n;
	scanf("%d %d", &n, &m);

	for (int i = 0; i < n; i++) {
		int t;
		scanf("%d", &t);
		arr[i] = t;
	}
	quickSort(arr, 0, n - 1);
	int lo, hi;

	for (int i = 0; i < m; i++)
	{
		scanf("%d %d", &lo, &hi);
		int value, rkey, lkey;
		lkey = binarySearch(lo, 0, n - 1);
		rkey = binarySearch(hi, 0, n - 1);

		value = rkey - lkey;
		if (arr[rkey] == hi) value++;
		if (value < 0) value = 0;
		printf("%d\n", value);
	}
}

