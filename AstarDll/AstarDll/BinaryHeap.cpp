#include "BinaryHeap.h"
#include <iostream>
template<typename T>
BinaryHeap<T>::BinaryHeap(int capacity)
    : array(capacity + 10), currentSize(capacity) {
}

template<typename T>
BinaryHeap<T>::BinaryHeap(vector<T>& item)
    : array(item.size() + 10), currentSize(item.size())
{
    for (int i = 0; i != item.size(); i++)
        array[i + 1] = item[i];
    BuildHeap();
}

template<typename T>
bool BinaryHeap<T>::IsEmpty() {
    return array.size() == 0 ? true : false;
}

template<typename T>
const bool operator <(const T& a, const T& b)
{
    return a.GetCost() < b.GetCost();
}

template<typename T>
void BinaryHeap<T>::Insert(const T& x) {
    if (currentSize == array.size() - 1) {
        array.resize(array.size() * 2);
    }
    int hole = ++currentSize;
    /*将需要插入的值与当前树中的最后一个元素的父元素做比较，若父元素大于x,则将父元素移到当前子元素的位置
     * 直到父元素小与等于x,则x插入当前子元素的位置。
    */
    for (; hole > 1 && x < array[hole / 2]; hole /= 2) {
        array[hole] = array[hole / 2];
    }
    array[hole] = x;
}

template<typename T>
T BinaryHeap<T>::DeleteMin() {
    if (IsEmpty()) {
        std::cout << "The heap is empty!" << std::endl;
    }
    T result(array[1]);
    //将array中最后一个数赋值给第一个元素，array元素个数减一；然后开始下滤
    array[1] = array[currentSize--];
    PercolateDown(1);
    return result;
}

template<typename T>
void BinaryHeap<T>::PercolateDown(int hole) {
    T x = array[hole];
    int child;
    /*找出当前父元素的子元素中最小的一个child，然后和父元素比较，如果child小与父元素，
     则将child的值赋给父元素，直到父元素的值小与等于子元素的值，此时将x的值赋给这个父元素终止循环 */

    for (; hole * 2 <= currentSize; hole = child) {
        child = 2 * hole;
        if (child != currentSize && array[child + 1] < array[child])
            child++;
        if (array[child] < x)
            array[hole] = array[child];
        else
            break;
    }
    array[hole] = x;
}

template<typename T>
void BinaryHeap<T>::BuildHeap() {
    for (int i = currentSize / 2; i > 0; i--)
        PercolateDown(i);
}

template<typename T>
bool BinaryHeap<T>::IsContains(const T& a) {
    for (int i = 1; i <= currentSize; i++) {
        if (a.x == array[i].x && a.y == array[i].y) {
            return true;
        }
    }
    return false;
}