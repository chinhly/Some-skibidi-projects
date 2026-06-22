#ifndef ALGORACOON_ALGORITHMS_HPP
#define ALGORACOON_ALGORITHMS_HPP

#include <cstddef>
#include <functional>
#include <utility>

namespace algoracoon {

// Swaps two values; used internally by sorting algorithms.
template <typename T>
void swapValues(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// Sorts an array using bubble sort and the comparator cmp.
template <typename T, typename Comp = std::less<T>>
void bubbleSort(T arr[], std::size_t n, Comp cmp = Comp()) {
    if (n < 2) return;
    for (std::size_t i = 0; i + 1 < n; ++i) {
        bool changed = false;
        for (std::size_t j = 0; j + 1 < n - i; ++j) {
            if (cmp(arr[j + 1], arr[j])) {
                swapValues(arr[j], arr[j + 1]);
                changed = true;
            }
        }
        if (!changed) break;
    }
}

// Sorts an array using selection sort and the comparator cmp.
template <typename T, typename Comp = std::less<T>>
void selectionSort(T arr[], std::size_t n, Comp cmp = Comp()) {
    for (std::size_t i = 0; i < n; ++i) {
        std::size_t best = i;
        for (std::size_t j = i + 1; j < n; ++j) {
            if (cmp(arr[j], arr[best])) best = j;
        }
        if (best != i) swapValues(arr[i], arr[best]);
    }
}

// Sorts an array using insertion sort and the comparator cmp.
template <typename T, typename Comp = std::less<T>>
void insertionSort(T arr[], std::size_t n, Comp cmp = Comp()) {
    for (std::size_t i = 1; i < n; ++i) {
        T key = arr[i];
        std::size_t j = i;
        while (j > 0 && cmp(key, arr[j - 1])) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

// Repairs the heap property for the subtree rooted at index root.
template <typename T, typename Comp>
void heapify(T arr[], std::size_t n, std::size_t root, Comp cmp) {
    while (true) {
        std::size_t best = root;
        std::size_t left = 2 * root + 1;
        std::size_t right = 2 * root + 2;

        if (left < n && cmp(arr[best], arr[left])) best = left;
        if (right < n && cmp(arr[best], arr[right])) best = right;

        if (best == root) break;
        swapValues(arr[root], arr[best]);
        root = best;
    }
}

// Sorts an array using heap sort and the comparator cmp.
template <typename T, typename Comp = std::less<T>>
void heapSort(T arr[], std::size_t n, Comp cmp = Comp()) {
    if (n < 2) return;

    for (std::size_t i = n / 2; i > 0; --i) {
        heapify(arr, n, i - 1, cmp);
    }
    for (std::size_t end = n; end > 1; --end) {
        swapValues(arr[0], arr[end - 1]);
        heapify(arr, end - 1, 0, cmp);
    }
}

// Partitions arr[lo..hi] for quick sort using the comparator cmp.
template <typename T, typename Comp>
int partition(T arr[], int lo, int hi, Comp cmp) {
    T pivot = arr[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; ++j) {
        if (!cmp(pivot, arr[j])) {
            ++i;
            swapValues(arr[i], arr[j]);
        }
    }
    swapValues(arr[i + 1], arr[hi]);
    return i + 1;
}

// Sorts arr[lo..hi] using quick sort and the comparator cmp.
template <typename T, typename Comp = std::less<T>>
void quickSort(T arr[], int lo, int hi, Comp cmp = Comp()) {
    if (lo >= hi) return;
    int p = partition(arr, lo, hi, cmp);
    quickSort(arr, lo, p - 1, cmp);
    quickSort(arr, p + 1, hi, cmp);
}

// Merges two sorted ranges [left, mid) and [mid, right) using the comparator cmp.
template <typename T, typename Comp>
void mergeRanges(T arr[], T buffer[], std::size_t left, std::size_t mid, std::size_t right, Comp cmp) {
    std::size_t i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (cmp(arr[j], arr[i])) buffer[k++] = arr[j++];
        else buffer[k++] = arr[i++];
    }
    while (i < mid) buffer[k++] = arr[i++];
    while (j < right) buffer[k++] = arr[j++];
    for (std::size_t t = left; t < right; ++t) arr[t] = buffer[t];
}

// Recursively sorts arr[left..right) using merge sort and the comparator cmp.
template <typename T, typename Comp>
void mergeSortRange(T arr[], T buffer[], std::size_t left, std::size_t right, Comp cmp) {
    if (right - left < 2) return;
    std::size_t mid = left + (right - left) / 2;
    mergeSortRange(arr, buffer, left, mid, cmp);
    mergeSortRange(arr, buffer, mid, right, cmp);
    mergeRanges(arr, buffer, left, mid, right, cmp);
}

// Sorts an array using merge sort and the comparator cmp.
template <typename T, typename Comp = std::less<T>>
void mergeSort(T arr[], std::size_t n, Comp cmp = Comp()) {
    if (n < 2) return;
    T* buffer = new T[n];
    mergeSortRange(arr, buffer, 0, n, cmp);
    delete[] buffer;
}

// Returns the first index of key in arr, or -1 if key is not found.
template <typename T, typename Key = T>
int linearSearch(const T arr[], std::size_t n, const Key& key) {
    for (std::size_t i = 0; i < n; ++i) {
        if (arr[i] == key) return static_cast<int>(i);
    }
    return -1;
}

// Returns the index of key in a sorted array, or -1 if key is not found.
template <typename T, typename Key = T, typename Comp = std::less<T>>
int binarySearch(const T arr[], std::size_t n, const Key& key, Comp cmp = Comp()) {
    std::size_t left = 0;
    std::size_t right = n;
    while (left < right) {
        std::size_t mid = left + (right - left) / 2;
        if (cmp(arr[mid], key)) left = mid + 1;
        else if (cmp(key, arr[mid])) right = mid;
        else return static_cast<int>(mid);
    }
    return -1;
}

} // namespace algoracoon

#endif // ALGORACOON_ALGORITHMS_HPP
