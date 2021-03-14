#include <iostream>
#include <vector>
#include <math.h>
#include <limits.h>

using namespace std;

long long int inversions;

void merge_inversions(vector<long long int> &inputs, long long int p, long long int q, long long int r)
{
    long long int n1 = q - p;
    long long int n2 = r - q;

    vector<long long int> left, right;
    for (long long int i = 0; i < n1; i++)
    {
        left.push_back(inputs[p + i]);
    }
    for (long long int i = 0; i < n2; i++)
    {
        right.push_back(inputs[q + i]);
    }

    left.push_back(INT_MAX);
    right.push_back(INT_MAX);
    long long int i = 0;
    long long int j = 0;
    for (long long int k = p; k < r; k++)
    {
        if (left[i] <= right[j])
        {
            inputs[k] = left[i];
            i++;
        }
        else
        {
            inputs[k] = right[j];
            j++;
            inversions += n1 - i;
        }
    }
}

void count_inversions(vector<long long int> &inputs, long long int p, long long int r)
{
    if (p >= r - 1)
        return;
    long long int q = (p + r) / 2;
    count_inversions(inputs, p, q);
    count_inversions(inputs, q, r);
    merge_inversions(inputs, p, q, r);
}

int main()
{
    long long int n;
    cin >> n;

    vector<long long int> inputs;

    long long int temp;
    for (long long int idx = 0; idx < n; idx++)
    {
        cin >> temp;

        inputs.push_back(temp);
    }
    count_inversions(inputs, 0, (long long int)inputs.size());
    cout << inversions << endl;
    return 0;
}