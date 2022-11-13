#include <iostream>
#include <string>
#include <array>
#include <unordered_map>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

//enum algebra {real = 0, complex = 1, quaternions = 2, octonions = 3, sedonions = 4}
const int algebra = 3;
const int algebraDim = 1 << algebra; // 2^n because c++ is fucking cursed

template <std::size_t N>
std::ostream& operator<<(std::ostream& os, std::array<int, N> const& v1)
{
    os << "[ ";
    for (const auto & val : v1)   {  os << val << " "; }
    os << "]";
    return os;
}

template <typename T, size_t N>
array<T,N> operator+(array<T, N> const& a1, array<T, N> const& a2)
{
  array<T,N> a;
  for (int i = 0; i < a1.size(); i++)
    a[i] = a1[i] + a2[i];
  return a;
}

template <typename T, size_t N>
array<T,N> operator-(array<T, N> const& a1, array<T, N> const& a2)
{
  array<T,N> a;
  for (int i = 0; i < a1.size(); i++)
    a[i] = a1[i] - a2[i];
  return a;
}

// A hash function used to hash a pair of any kind
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const
    {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
 
        if (hash1 != hash2) {
            return hash1 ^ hash2;             
        }
         
        // If hash1 == hash2, their XOR is zero.
          return hash1;
    }
};

// returns nonzero index (the value at which is +/- 1) and a True if negative, False if positive
int firstNonzeroIndex(array<int,algebraDim> z)
    {
        for (int k=0; k < algebraDim;  k++) {
            if (z[k] != 0) {
                if (z[k] == 1) { return k + 1;}
                else if (z[k] == -1) { return -k - 1;}
                // -(k+1) = -k-1, shifted to get 1-indexing
                else {
                    cout << "weird nonzero value found";
                    return k;
                }
            }
        }
        cout << "no nonzero value found";
        return 0;
    }
// this global variable has to be defined here because fuck c++
unordered_map<pair<int,int>, int, hash_pair> multTable;