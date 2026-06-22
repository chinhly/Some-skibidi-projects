#ifndef ALGORACOON_LINKED_LIST_HPP
#define ALGORACOON_LINKED_LIST_HPP

#include <cstddef>
#include <stdexcept>

namespace algoracoon {

template <typename T>
struct LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    std::size_t count_;

    void copyFrom(const LinkedList& other) {
        for (Node* cur = other.head_; cur != nullptr; cur = cur->next) {
            insertBack(cur->data);
        }
    }

    template <typename Func>
    void forEachBackwardFrom(Node* node, Func action) const {
        if (!node) return;
        forEachBackwardFrom(node->next, action);
        action(node->data);
    }

public:
    // Creates an empty linked list.
    LinkedList() : head_(nullptr), tail_(nullptr), count_(0) {}

    // Creates a deep copy of another linked list.
    LinkedList(const LinkedList& other) : head_(nullptr), tail_(nullptr), count_(0) {
        copyFrom(other);
    }

    // Moves another linked list into this one in constant time.
    LinkedList(LinkedList&& other) noexcept : head_(other.head_), tail_(other.tail_), count_(other.count_) {
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.count_ = 0;
    }

    // Replaces this list with a deep copy of another linked list.
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    // Replaces this list by moving another linked list into it.
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            tail_ = other.tail_;
            count_ = other.count_;
            other.head_ = nullptr;
            other.tail_ = nullptr;
            other.count_ = 0;
        }
        return *this;
    }

    // Releases all nodes owned by the linked list.
    ~LinkedList() {
        clear();
    }

    // Inserts value at the beginning of the list.
    void insertFront(const T& value) {
        Node* node = new Node(value);
        node->next = head_;
        head_ = node;
        if (tail_ == nullptr) tail_ = head_;
        ++count_;
    }

    // Inserts value at the end of the list.
    void insertBack(const T& value) {
        Node* node = new Node(value);
        if (tail_) tail_->next = node;
        else head_ = node;
        tail_ = node;
        ++count_;
    }

    // Inserts value before the element currently at index.
    void insertAt(std::size_t index, const T& value) {
        if (index > count_) throw std::out_of_range("LinkedList::insertAt index is out of range");
        if (index == 0) {
            insertFront(value);
            return;
        }
        if (index == count_) {
            insertBack(value);
            return;
        }

        Node* prev = head_;
        for (std::size_t i = 1; i < index; ++i) prev = prev->next;
        Node* node = new Node(value);
        node->next = prev->next;
        prev->next = node;
        ++count_;
    }

    // Removes the first element equal to value and returns true if one was removed.
    bool remove(const T& value) {
        Node* prev = nullptr;
        Node* cur = head_;
        while (cur) {
            if (cur->data == value) {
                if (prev) prev->next = cur->next;
                else head_ = cur->next;
                if (cur == tail_) tail_ = prev;
                delete cur;
                --count_;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // Removes and returns the element at index.
    T removeAt(std::size_t index) {
        if (index >= count_) throw std::out_of_range("LinkedList::removeAt index is out of range");
        Node* prev = nullptr;
        Node* cur = head_;
        for (std::size_t i = 0; i < index; ++i) {
            prev = cur;
            cur = cur->next;
        }
        if (prev) prev->next = cur->next;
        else head_ = cur->next;
        if (cur == tail_) tail_ = prev;
        T value = cur->data;
        delete cur;
        --count_;
        return value;
    }

    // Returns the first index of value, or -1 if value is not found.
    int find(const T& value) const {
        Node* cur = head_;
        int index = 0;
        while (cur) {
            if (cur->data == value) return index;
            cur = cur->next;
            ++index;
        }
        return -1;
    }

    // Returns the number of elements in the list.
    std::size_t size() const {
        return count_;
    }

    // Returns true when the list has no elements.
    bool empty() const {
        return count_ == 0;
    }

    // Removes all elements from the list.
    void clear() {
        Node* cur = head_;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        count_ = 0;
    }

    // Returns a reference to the first element.
    T& front() {
        if (empty()) throw std::out_of_range("LinkedList::front called on empty list");
        return head_->data;
    }

    // Returns a const reference to the first element.
    const T& front() const {
        if (empty()) throw std::out_of_range("LinkedList::front called on empty list");
        return head_->data;
    }

    // Returns a reference to the last element.
    T& back() {
        if (empty()) throw std::out_of_range("LinkedList::back called on empty list");
        return tail_->data;
    }

    // Returns a const reference to the last element.
    const T& back() const {
        if (empty()) throw std::out_of_range("LinkedList::back called on empty list");
        return tail_->data;
    }

    // Visits every element from head to tail using action(element).
    template <typename Func>
    void forEachForward(Func action) const {
        for (Node* cur = head_; cur != nullptr; cur = cur->next) {
            action(cur->data);
        }
    }

    // Visits every element from tail to head using action(element).
    template <typename Func>
    void forEachBackward(Func action) const {
        forEachBackwardFrom(head_, action);
    }
};

} // namespace algoracoon

#endif // ALGORACOON_LINKED_LIST_HPP
