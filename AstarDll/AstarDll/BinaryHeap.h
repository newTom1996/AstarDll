#pragma once
#include <vector>
using std::vector;
template < typename T>
class BinaryHeap
{
public:
    BinaryHeap(int capacity = 100);
    BinaryHeap(vector<T>& item);
    ~BinaryHeap() {}
    bool IsEmpty();
    T& FindMin() const;
    void Insert(const T& x);
    T DeleteMin();
    bool IsContains(const T& x);
    void Clear();
private:
    int currentSize;
    vector<T> array;
    void PercolateDown(int hole);
    void BuildHeap();
};