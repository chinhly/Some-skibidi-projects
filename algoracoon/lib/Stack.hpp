#ifndef ALGORACOON_STACK_HPP
#define ALGORACOON_STACK_HPP

#include <cstddef>
#include <stdexcept>

namespace algoracoon {

template <typename T>
struct Stack {
private:
    T* data_;
    std::size_t count_;
    std::size_t capacity_;

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity_) return;
        T* next = new T[newCapacity];
        for (std::size_t i = 0; i < count_; ++i) next[i] = data_[i];
        delete[] data_;
        data_ = next;
        capacity_ = newCapacity;
    }

public:
    // Creates an empty stack with an optional initial capacity.
    explicit Stack(std::size_t capacity = 8) : data_(new T[capacity ? capacity : 1]), count_(0), capacity_(capacity ? capacity : 1) {}

    // Creates a deep copy of another stack.
    Stack(const Stack& other) : data_(new T[other.capacity_]), count_(other.count_), capacity_(other.capacity_) {
        for (std::size_t i = 0; i < count_; ++i) data_[i] = other.data_[i];
    }

    // Moves another stack into this one in constant time.
    Stack(Stack&& other) noexcept : data_(other.data_), count_(other.count_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.count_ = 0;
        other.capacity_ = 0;
    }

    // Replaces this stack with a deep copy of another stack.
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            T* next = new T[other.capacity_];
            for (std::size_t i = 0; i < other.count_; ++i) next[i] = other.data_[i];
            delete[] data_;
            data_ = next;
            count_ = other.count_;
            capacity_ = other.capacity_;
        }
        return *this;
    }

    // Replaces this stack by moving another stack into it.
    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            count_ = other.count_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.count_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    // Releases the stack storage.
    ~Stack() {
        delete[] data_;
    }

    // Pushes value onto the top of the stack.
    void push(const T& value) {
        if (count_ == capacity_) reserve(capacity_ * 2);
        data_[count_++] = value;
    }

    // Removes and returns the top value of the stack.
    T pop() {
        if (empty()) throw std::out_of_range("Stack::pop called on empty stack");
        return data_[--count_];
    }

    // Returns a reference to the top value without removing it.
    T& top() {
        if (empty()) throw std::out_of_range("Stack::top called on empty stack");
        return data_[count_ - 1];
    }

    // Returns a const reference to the top value without removing it.
    const T& top() const {
        if (empty()) throw std::out_of_range("Stack::top called on empty stack");
        return data_[count_ - 1];
    }

    // Returns true when the stack has no elements.
    bool empty() const {
        return count_ == 0;
    }

    // Returns the number of elements in the stack.
    std::size_t size() const {
        return count_;
    }

    // Removes every element from the stack without releasing capacity.
    void clear() {
        count_ = 0;
    }
};

} // namespace algoracoon

#endif // ALGORACOON_STACK_HPP
