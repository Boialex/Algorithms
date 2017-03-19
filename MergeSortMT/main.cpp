#include <iostream>
#include <random>
#include <vector>
#include <iterator>
#include <thread>

bool check(std::vector<int> & a)
{
    for (int i = 0; i < a.size() - 1; ++i) {
        if (a[i] > a[i + 1])
            return false;
    }
    return true;
}

int binarySearch(int x, std::vector<int>::iterator begin, std::vector<int>::iterator end)
{
    if (x <= (*begin))
        return 0;
    int l = 0, r = end - begin;
    while (l < r) {
        int m = (l + r) / 2;
        if ((*(begin + m)) > x)
            r = m;
        else
            l = m;
    }
    return l;
}

void mergeMT(std::vector<int>::iterator begin1T, std::vector<int>::iterator end1T, 
             std::vector<int>::iterator begin2T, std::vector<int>::iterator end2T, 
             std::vector<int>::iterator beginA)
{
    int len1 = end1T - begin1T + 1;
    int len2 = end2T - begin2T + 1;
    if (len1 < len2) {
        std::swap(begin1T, begin2T);
        std::swap(end1T, end2T);
        std::swap(len1, len2);
    }

    if (len1 == 0)
        return;
    else {
        int mid1 = (end1T - begin1T) / 2;
        int mid2 = binarySearch(*(begin1T + mid1), begin2T, end2T);
        int mid3 = mid1 + mid2;
        *(beginA + mid3) = *(begin1T + mid1);
        std::thread * l;
        std::thread * r;
        l = new std::thread(mergeMT, begin1T, begin1T + mid1, begin2T, begin2T + mid2, beginA);
        r = new std::thread(mergeMT, begin1T + mid1 + 1, end1T, begin2T + mid2, end2T, beginA + mid3 + 1);
        l->join();
        r->join();
    }
}

void mergeSortMT(std::vector<int>::iterator beginA, std::vector<int>::iterator endA, std::vector<int>::iterator beginB)
{
    int len = endA - beginA;
    if (len == 1)
        (*beginB) = (*beginA);
    else {
        std::vector<int> T;
        T.resize(len);
        int mid = (endA - beginA) / 2;
        std::thread * l;
        std::thread * r;
        l = new std::thread(mergeSortMT, beginA, beginA + mid, T.begin());
        r = new std::thread(mergeSortMT, beginA + mid, endA, T.begin() + mid);
        l->join();
        r->join();
        mergeMT(T.begin(), T.begin() + mid, T.begin(), T.end(), beginB);
    }
}

int main() {
    int size = 10;
    std::vector<int> a, b;
    a.resize(size);
    b.resize(size, 0);
    for (int i = 0; i < a.size(); ++i) {
        a[i] = rand() % size;
    }
    for (int i = 0; i < b.size(); ++i) {
        std::cout << a[i] << ' ';
    }                                                                
    std::cout << std::endl;

    mergeSortMT(a.begin(), a.end(), b.begin());

    for (int i = 0; i < b.size(); ++i) {
        std::cout << b[i] << ' ';
    }
    std::cout << std::endl;
}
/*
bool check(std::vector<int> & a)
{
    for (int i = 0; i < a.size() - 1; ++i) {
        if (a[i] > a[i + 1])
            return false;
    }
    return true;
}

int binarySearch(int x, std::vector<int> & a, size_t left, size_t right)
{
    if (x <= a[left])
        return left;
    int l = left, r = right;
    while (l < r) {
        int m = (l + r) / 2;
        if (a[m] > x)
            r = m;
        else
            l = m;
    }
    return l;
}

void mergeMT(std::vector<int> & T, size_t left1, size_t right1, size_t left2, size_t right2, std::vector<int> & A, size_t left3)
{
    int len1 = right1 - left1 + 1;
    int len2 = right2 - left2 + 1;
    if (len1 < len2) {
        std::swap(left1, left2);
        std::swap(right1, right2);
        std::swap(len1, len2);
    }

    if (len1 == 0)
        return;
    else {
        int mid1 = (left1 + right1) / 2;
        int mid2 = binarySearch(T[mid1], std::ref(T), left2, right2);
        int mid3 = left3 + (mid1 - left1) + (mid2 - left2);
        A[mid3] = T[mid1];
        std::thread * l;
        std::thread * r;
        l = new std::thread(mergeMT, std::ref(T), left1, mid1, left2, mid2, std::ref(A), left3);
        r = new std::thread(mergeMT, std::ref(T), mid1 + 1, right1, mid2, right2, std::ref(A), mid3 + 1);
        l->join();
        r->join();
    }
}

void mergeSortMT(std::vector<int> & A, size_t left, size_t right, std::vector<int> & B, size_t leftB)
{
    int len = right - left;
    if (len == 1)
        B[leftB] = A[left];
    else {
        std::vector<int> T;
        T.resize(len);
        int mid = (left + right) / 2;
        int newMid = mid - left + 1;
        std::thread * l;
        std::thread * r;
        l = new std::thread(mergeSortMT, std::ref(A), left, mid, std::ref(T), 0);
        r = new std::thread(mergeSortMT, std::ref(A), mid, right, std::ref(T), newMid);
        l->join();
        r->join();
        mergeMT(std::ref(T), 0, newMid, newMid, len, std::ref(B), leftB);
    }
}

int main() {
    int size = 10;
    std::vector<int> a, b;
    a.resize(size);
    b.resize(size, 0);
    for (int i = 0; i < a.size(); ++i) {
        a[i] = rand() % size;
    }
    for (int i = 0; i < b.size(); ++i) {
        std::cout << a[i] << ' ';
    }
    std::cout << std::endl;

    mergeSortMT(std::ref(a), 0, size, std::ref(b), 0);
    
    for (int i = 0; i < b.size(); ++i) {
        std::cout << b[i] << ' ';
    }
    std::cout << std::endl;
}              */