#include <stdio.h>
#include <iostream>


using   namespace   std;

int main()
{
	const int SZ = 1 << 20;
	struct fastio{
		char inbuf[SZ];
		char outbuf[SZ];
		fastio(){
			setvbuf(stdin, inbuf, _IOFBF, SZ);
			setvbuf(stdout, outbuf, _IOFBF, SZ);
		}
	}io;

	int m, n;

	cin >> n;
	cin >> m;


	std::cout << "m = " << m << " " << ";n = " << n << "\n";
	vector<int> array1(n);
	vector<int> array2(m);
	
	for (int i = 0; i < n; i++)
	{
		cin >> array1[i];
		cout << array1[i] << "\t";
	}

	for (int j = 0; j < m; j++)
	{


	}



	system("PAUSE");
}