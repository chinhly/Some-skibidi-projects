#ifndef ALGORACOON_HASH_TABLE_HPP
#define ALGORACOON_HASH_TABLE_HPP

#include "AVL.hpp"
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace algoracoon {

template <typename K, typename V, typename Hash = std::hash<K>, typename KeyEqual = std::equal_to<K>, typename KeyLess = std::less<K>>
struct HashTable {
private:
    using Pair = std::pair<K, V>;

    struct PairFirstLess {
        KeyLess less;

        bool operator()(const Pair& a, const Pair& b) const {
            return less(a.first, b.first);
        }
    };

    using Bucket = AVL<Pair, PairFirstLess>;

    Bucket* buckets_;
    std::size_t bucketCount_;
    std::size_t count_;
    Hash hash_;
    KeyEqual equal_;
    PairFirstLess pairLess_;

    std::size_t indexFor(const K& key) const {
        return hash_(key) % bucketCount_;
    }

    Pair* findPair(const K& key) {
        Bucket& bucket = buckets_[indexFor(key)];
        return bucket.findIf([&](const Pair& item) { return equal_(item.first, key); });
    }

    const Pair* findPair(const K& key) const {
        const Bucket& bucket = buckets_[indexFor(key)];
        return bucket.findIf([&](const Pair& item) { return equal_(item.first, key); });
    }

    void allocateBuckets(std::size_t bucketCount) {
        bucketCount_ = bucketCount ? bucketCount : 1;
        buckets_ = new Bucket[bucketCount_];
    }

public:
    // Creates an empty hash table with separate chaining by AVL buckets.
    explicit HashTable(std::size_t bucketCount = 101, Hash hash = Hash(), KeyEqual equal = KeyEqual(), KeyLess less = KeyLess())
        : buckets_(nullptr), bucketCount_(0), count_(0), hash_(hash), equal_(equal), pairLess_{less} {
        bucketCount_ = bucketCount ? bucketCount : 1;
        buckets_ = new Bucket[bucketCount_];
        for (std::size_t i = 0; i < bucketCount_; ++i) buckets_[i] = Bucket(pairLess_);
    }

    // Creates a deep copy of another hash table.
    HashTable(const HashTable& other)
        : buckets_(nullptr), bucketCount_(other.bucketCount_), count_(other.count_), hash_(other.hash_), equal_(other.equal_), pairLess_(other.pairLess_) {
        buckets_ = new Bucket[bucketCount_];
        for (std::size_t i = 0; i < bucketCount_; ++i) buckets_[i] = other.buckets_[i];
    }

    // Moves another hash table into this one in constant time.
    HashTable(HashTable&& other) noexcept
        : buckets_(other.buckets_), bucketCount_(other.bucketCount_), count_(other.count_), hash_(other.hash_), equal_(other.equal_), pairLess_(other.pairLess_) {
        other.buckets_ = nullptr;
        other.bucketCount_ = 0;
        other.count_ = 0;
    }

    // Replaces this hash table with a deep copy of another hash table.
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            Bucket* next = new Bucket[other.bucketCount_];
            for (std::size_t i = 0; i < other.bucketCount_; ++i) next[i] = other.buckets_[i];
            delete[] buckets_;
            buckets_ = next;
            bucketCount_ = other.bucketCount_;
            count_ = other.count_;
            hash_ = other.hash_;
            equal_ = other.equal_;
            pairLess_ = other.pairLess_;
        }
        return *this;
    }

    // Replaces this hash table by moving another hash table into it.
    HashTable& operator=(HashTable&& other) noexcept {
        if (this != &other) {
            delete[] buckets_;
            buckets_ = other.buckets_;
            bucketCount_ = other.bucketCount_;
            count_ = other.count_;
            hash_ = other.hash_;
            equal_ = other.equal_;
            pairLess_ = other.pairLess_;
            other.buckets_ = nullptr;
            other.bucketCount_ = 0;
            other.count_ = 0;
        }
        return *this;
    }

    // Releases all buckets and stored key-value pairs.
    ~HashTable() {
        delete[] buckets_;
    }

    // Inserts or updates the value associated with key.
    void insert(const K& key, const V& value) {
        Pair* existing = findPair(key);
        if (existing) {
            existing->second = value;
            return;
        }
        buckets_[indexFor(key)].insert(Pair(key, value));
        ++count_;
    }

    // Removes key and returns true if a key-value pair was removed.
    bool remove(const K& key) {
        Pair* existing = findPair(key);
        if (!existing) return false;
        Pair victim = *existing;
        bool removed = buckets_[indexFor(key)].remove(victim);
        if (removed) --count_;
        return removed;
    }

    // Returns a pointer to the value for key, or nullptr if key is absent.
    V* find(const K& key) {
        Pair* existing = findPair(key);
        return existing ? &existing->second : nullptr;
    }

    // Returns a const pointer to the value for key, or nullptr if key is absent.
    const V* find(const K& key) const {
        const Pair* existing = findPair(key);
        return existing ? &existing->second : nullptr;
    }

    // Returns true if key is present in the table.
    bool contains(const K& key) const {
        return findPair(key) != nullptr;
    }

    // Returns a reference to the value for key, inserting a default value when key is absent.
    V& operator[](const K& key) {
        Pair* existing = findPair(key);
        if (!existing) {
            insert(key, V());
            existing = findPair(key);
        }
        return existing->second;
    }

    // Returns the number of key-value pairs stored in the table.
    std::size_t size() const {
        return count_;
    }

    // Returns true when the table has no key-value pairs.
    bool empty() const {
        return count_ == 0;
    }

    // Returns the number of AVL buckets in the table.
    std::size_t bucketCount() const {
        return bucketCount_;
    }

    // Removes all key-value pairs while keeping the same number of buckets.
    void clear() {
        for (std::size_t i = 0; i < bucketCount_; ++i) buckets_[i].clear();
        count_ = 0;
    }
};

} // namespace algoracoon

#endif // ALGORACOON_HASH_TABLE_HPP
