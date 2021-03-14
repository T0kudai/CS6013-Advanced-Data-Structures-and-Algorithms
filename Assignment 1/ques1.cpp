#include <iostream>

using namespace std;

bool bits(unsigned long long int x, unsigned long long int y) {
	unsigned long long int x_bits = 0;
	unsigned long long int n = x;
	while(x){
		x_bits++;
		n = n>>1;
	}
	cout<<x_bits;
	if(x_bits * y > 64){
		return false;
	}
	return true;
}

unsigned long long int power(unsigned long long int x, unsigned long long int y)
{
	unsigned long long int result=1;
	if (y == 0)
		return 1;
	for(; y != 0 ; --y){
		result *= x;
	}
	return result;
}

unsigned long long int nthRootSearch(unsigned long long int low, unsigned long long int high,
						 unsigned long long int N, unsigned long long int K)
{

	if (low <= high)
	{

		unsigned long long int mid = (low + high) / 2;


		if ((power(mid, K) <= N) && bits(mid,K) && (power(mid + 1, K) > N) && bits(mid+1, K))
		{
			return mid;
		}

		else if (bits(mid,K) && power(mid, K) < N)
		{
			return nthRootSearch(mid + 1,
								 high, N, K);
		}
		else
		{
			return nthRootSearch(low,
								 mid - 1,
								 N, K);
		}
	}
	return low;
}

int main()
{

	unsigned long long int N = 10000000000000000;
	unsigned long long int k;

	cin>>k;

	cout << (nthRootSearch(0, N, N, k))
		 << endl;
	return 0;
}
