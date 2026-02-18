// key-value pairs
// Each key is unique.
// C++ provides mainly:

// std::map → Ordered (implemented using Red-Black Tree)
// std::unordered_map → Unordered (implemented using Hash Table)
// std::multimap → Allows duplicate keys

// (Ordered Map)
// Keys are stored in sorted order
// Internally uses Red-Black Tree

// Time complexity:
// Insert → O(log n)
// Find → O(log n)
// Delete → O(log n)

#include<iostream>
#include<array>
#include<bits/stdc++.h>
#include<map>
using namespace std;

// int main()
// {
//     map<int, string> m;

//     m[1] = "Akash";
//     m[2] = "Ashok";
//     m[3] = "Sangnure";

//     for(auto &p : m)
//     {
//         cout << p.first << "->" << p.second <<endl;
//     }
// }

// Q. It takes N Numbers, stores frequecy of each number using map, prints frequency

int main()
{
    int arr[5] = {2,1,5,2,4};
    int size = sizeof(arr)/sizeof(arr[0]);
    map<int, int> m;

    for(int i=0;i<size;i++)
    {
        m[arr[i]]++;
    }

    for(auto &p: m)
    {
        cout<< p.first << "->" << p.second << endl;
    }
    return 0;
}