#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n = 5;
    vector<int> arr1(n), arr2(n);
    map<int,int> m;
    vector<int> v;

    cout << "Enter arr1 elements:\n";
    for(int i=0;i<n;i++)
        cin >> arr1[i];

    cout << "Enter arr2 elements:\n";
    for(int i=0;i<n;i++)
        cin >> arr2[i];

    for(int i=0;i<n;i++)
    {
        m[arr1[i]]++;
        m[arr2[i]]++;
    }

    for(auto &p : m)
    {
        v.push_back(p.first);
    }

    cout << "Union:\n";
    for(int i=0;i<v.size();i++)
        cout << v[i] << " ";

    return 0;
}
