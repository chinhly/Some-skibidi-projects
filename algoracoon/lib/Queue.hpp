#ifndef ALGORACOON_QUEUE_HPP
#define ALGORACOON_QUEUE_HPP

#include <cstddef>
#include <stdexcept>

namespace algoracoon {

template <typename T>
struct Queue {
private:
    T* data_;
    std::size_t head_;
    std::size_t count_;
    std::size_t capacity_;

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity_) return;
        T* next = new T[newCapacity];
        for (std::size_t i = 0; i < count_; ++i) next[i] = data_[(head_ + i) % capacity_];
        delete[] data_;
        data_ = next;
        head_ = 0;
        capacity_ = newCapacity;
    }

public:
    // Creates an empty queue with an optional initial capacity.
    explicit Queue(std::size_t capacity = 8) : data_(new T[capacity ? capacity : 1]), head_(0), count_(0), capacity_(capacity ? capacity : 1) {}

    // Creates a deep copy of another queue.
    Queue(const Queue& other) : data_(new T[other.capacity_]), head_(0), count_(other.count_), capacity_(other.capacity_) {
        for (std::size_t i = 0; i < count_; ++i) data_[i] = other.data_[(other.head_ + i) % other.capacity_];
    }

    // Moves another queue into this one in constant time.
    Queue(Queue&& other) noexcept : data_(other.data_), head_(other.head_), count_(other.count_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.head_ = 0;
        other.count_ = 0;
        other.capacity_ = 0;
    }

    // Replaces this queue with a deep copy of another queue.
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            T* next = new T[other.capacity_];
            for (std::size_t i = 0; i < other.count_; ++i) next[i] = other.data_[(other.head_ + i) % other.capacity_];
            delete[] data_;
            data_ = next;
            head_ = 0;
            count_ = other.count_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    // Replaces this queue by moving another queue into it.
    Queue& operator=(Queue&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            head_ = other.head_;
            count_ = other.count_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.head_ = 0;
            other.count_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Releases the queue storage.
    ~Queue() {
        delete[] data_;
    }

    // Adds value to the back of the queue.
    void enqueue(const T& value) {
        if (count_ == capacity_) reserve(capacity_ * 2);
        std::size_t tail = (head_ + count_) % capacity_;
        data_[tail] = value;
        ++count_;
    }

    // Removes and returns the value at the front of the queue.
    T dequeue() {
        if (empty()) throw std::out_of_range("Queue::dequeue called on empty queue");
        T value = data_[head_];
        head_ = (head_ + 1) % capacity_;
        --count_;
        return value;
    }

    // Returns a reference to the value at the front of the queue.
    T& front() {
        if (empty()) throw std::out_of_range("Queue::front called on empty queue");
        return data_[head_];
    }

    // Returns a const reference to the value at the front of the queue.
    const T& front() const {
        if (empty()) throw std::out_of_range("Queue::front called on empty queue");
        return data_[head_];
    }

    // Returns true when the queue has no elements.
    bool empty() const {
        return count_ == 0;
    }

    // Returns the number of elements in the queue.
    std::size_t size() const {
        return count_;
    }

    // Removes every element from the queue without releasing capacity.
    void clear() {
        head_ = 0;
        count_ = 0;
    }
};

} // namespace algoracoon

#endif // ALGORACOON_QUEUE_HPP
