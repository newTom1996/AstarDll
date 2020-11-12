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
    /*����Ҫ�����ֵ�뵱ǰ���е����һ��Ԫ�صĸ�Ԫ�����Ƚϣ�����Ԫ�ش���x,�򽫸�Ԫ���Ƶ���ǰ��Ԫ�ص�λ��
     * ֱ����Ԫ��С�����x,��x���뵱ǰ��Ԫ�ص�λ�á�
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
    //��array�����һ������ֵ����һ��Ԫ�أ�arrayԪ�ظ�����һ��Ȼ��ʼ����
    array[1] = array[currentSize--];
    PercolateDown(1);
    return result;
}

template<typename T>
void BinaryHeap<T>::PercolateDown(int hole) {
    T x = array[hole];
    int child;
    /*�ҳ���ǰ��Ԫ�ص���Ԫ������С��һ��child��Ȼ��͸�Ԫ�رȽϣ����childС�븸Ԫ�أ�
     ��child��ֵ������Ԫ�أ�ֱ����Ԫ�ص�ֵС�������Ԫ�ص�ֵ����ʱ��x��ֵ���������Ԫ����ֹѭ�� */

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