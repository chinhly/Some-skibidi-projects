#ifndef ALGORACOON_PRIORITY_QUEUE_HPP
#define ALGORACOON_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>

namespace algoracoon {

template <typename T, typename Comp = std::less<T>>
struct PriorityQueue {
private:
    T* data_;
    std::size_t count_;
    std::size_t capacity_;
    Comp cmp_;

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity_) return;
        T* next = new T[newCapacity];
        for (std::size_t i = 0; i < count_; ++i) next[i] = data_[i];
        delete[] data_;
        data_ = next;
        capacity_ = newCapacity;
    }

    void siftUp(std::size_t index) {
        while (index > 0) {
            std::size_t parent = (index - 1) / 2;
            if (!cmp_(data_[parent], data_[index])) break;
            T tmp = data_[parent];
            data_[parent] = data_[index];
            data_[index] = tmp;
            index = parent;
        }
    }

    void siftDown(std::size_t index) {
        while (true) {
            std::size_t left = index * 2 + 1;
            std::size_t right = index * 2 + 2;
            std::size_t best = index;
            if (left < count_ && cmp_(data_[best], data_[left])) best = left;
            if (right < count_ && cmp_(data_[best], data_[right])) best = right;
            if (best == index) break;
            T tmp = data_[best];
            data_[best] = data_[index];
            data_[index] = tmp;
            index = best;
        }
    }

public:
    // Creates an empty priority queue with an optional comparator and initial capacity.
    explicit PriorityQueue(Comp cmp = Comp(), std::size_t capacity = 8)
        : data_(new T[capacity ? capacity : 1]), count_(0), capacity_(capacity ? capacity : 1), cmp_(cmp) {}

    // Creates a deep copy of another priority queue.
    PriorityQueue(const PriorityQueue& other)
        : data_(new T[other.capacity_]), count_(other.count_), capacity_(other.capacity_), cmp_(other.cmp_) {
        for (std::size_t i = 0; i < count_; ++i) data_[i] = other.data_[i];
    }

    // Moves another priority queue into this one in constant time.
    PriorityQueue(PriorityQueue&& other) noexcept
        : data_(other.data_), count_(other.count_), capacity_(other.capacity_), cmp_(other.cmp_) {
        other.data_ = nullptr;
        other.count_ = 0;
        other.capacity_ = 0;
    }

    // Replaces this priority queue with a deep copy of another priority queue.
    PriorityQueue& operator=(const PriorityQueue& other) {
        if (this != &other) {
            T* next = new T[other.capacity_];
            for (std::size_t i = 0; i < other.count_; ++i) next[i] = other.data_[i];
            delete[] data_;
            data_ = next;
            count_ = other.count_;
            capacity_ = other.capacity_;
            cmp_ = other.cmp_;
        }
        return *this;
    }

    // Replaces this priority queue by moving another priority queue into it.
    PriorityQueue& operator=(PriorityQueue&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            count_ = other.count_;
            capacity_ = other.capacity_;
            cmp_ = other.cmp_;
            other.data_ = nullptr;
            other.count_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Releases the priority queue storage.
    ~PriorityQueue() {
        delete[] data_;
    }

    // Inserts value according to its priority.
    void insert(const T& value) {
        if (count_ == capacity_) reserve(capacity_ * 2);
        data_[count_] = value;
        siftUp(count_);
        ++count_;
    }

    // Removes and returns the highest-priority value.
    T extract() {
        if (empty()) throw std::out_of_range("PriorityQueue::extract called on empty queue");
        T result = data_[0];
        data_[0] = data_[--count_];
        if (count_ > 0) siftDown(0);
        return result;
    }

    // Returns a reference to the highest-priority value without removing it.
    T& peek() {
        if (empty()) throw std::out_of_range("PriorityQueue::peek called on empty queue");
        return data_[0];
    }

    // Returns a const reference to the highest-priority value without removing it.
    const T& peek() const {
        if (empty()) throw std::out_of_range("PriorityQueue::peek called on empty queue");
        return data_[0];
    }

    // Returns true when the priority queue has no elements.
    bool empty() const {
        return count_ == 0;
    }

    // Returns the number of elements in the priority queue.
    std::size_t size() const {
        return count_;
    }

    // Removes every element from the priority queue without releasing capacity.
    void clear() {
        count_ = 0;
    }
};

} // namespace algoracoon

#endif // ALGORACOON_PRIORITY_QUEUE_HPP
