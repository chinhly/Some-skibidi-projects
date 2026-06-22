#ifndef ALGORACOON_BST_HPP
#define ALGORACOON_BST_HPP

#include <cstddef>
#include <functional>

namespace algoracoon {

template <typename T, typename Comp = std::less<T>>
struct BST {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        explicit Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root_;
    std::size_t count_;
    Comp cmp_;

    bool equals(const T& a, const T& b) const {
        return !cmp_(a, b) && !cmp_(b, a);
    }

    Node* clone(Node* node) {
        if (!node) return nullptr;
        Node* next = new Node(node->data);
        next->left = clone(node->left);
        next->right = clone(node->right);
        return next;
    }

    void clear(Node* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

    bool insert(Node*& node, const T& value) {
        if (!node) {
            node = new Node(value);
            ++count_;
            return true;
        }
        if (equals(value, node->data)) return false;
        if (cmp_(value, node->data)) return insert(node->left, value);
        return insert(node->right, value);
    }

    Node* findMin(Node* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    bool remove(Node*& node, const T& value) {
        if (!node) return false;
        if (cmp_(value, node->data)) return remove(node->left, value);
        if (cmp_(node->data, value)) return remove(node->right, value);

        if (!node->left || !node->right) {
            Node* old = node;
            node = node->left ? node->left : node->right;
            delete old;
            --count_;
            return true;
        }

        Node* successor = findMin(node->right);
        node->data = successor->data;
        return remove(node->right, successor->data);
    }

    Node* find(Node* node, const T& value) const {
        while (node) {
            if (equals(value, node->data)) return node;
            node = cmp_(value, node->data) ? node->left : node->right;
        }
        return nullptr;
    }

    template <typename Func>
    void preorder(Node* node, Func action) const {
        if (!node) return;
        action(node->data);
        preorder(node->left, action);
        preorder(node->right, action);
    }

    template <typename Func>
    void inorder(Node* node, Func action) const {
        if (!node) return;
        inorder(node->left, action);
        action(node->data);
        inorder(node->right, action);
    }

    template <typename Func>
    void postorder(Node* node, Func action) const {
        if (!node) return;
        postorder(node->left, action);
        postorder(node->right, action);
        action(node->data);
    }

public:
    // Creates an empty binary search tree with an optional comparator.
    explicit BST(Comp cmp = Comp()) : root_(nullptr), count_(0), cmp_(cmp) {}

    // Creates a deep copy of another binary search tree.
    BST(const BST& other) : root_(nullptr), count_(other.count_), cmp_(other.cmp_) {
        root_ = clone(other.root_);
    }

    // Moves another binary search tree into this one in constant time.
    BST(BST&& other) noexcept : root_(other.root_), count_(other.count_), cmp_(other.cmp_) {
        other.root_ = nullptr;
        other.count_ = 0;
    }

    // Replaces this tree with a deep copy of another tree.
    BST& operator=(const BST& other) {
        if (this != &other) {
            clear();
            cmp_ = other.cmp_;
            count_ = other.count_;
            root_ = clone(other.root_);
        }
        return *this;
    }

    // Replaces this tree by moving another tree into it.
    BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            clear();
            root_ = other.root_;
            count_ = other.count_;
            cmp_ = other.cmp_;
            other.root_ = nullptr;
            other.count_ = 0;
        }
        return *this;
    }

    // Releases every node in the tree.
    ~BST() {
        clear();
    }

    // Inserts value and returns true if it was not already present.
    bool insert(const T& value) {
        return insert(root_, value);
    }

    // Removes value and returns true if a value was removed.
    bool remove(const T& value) {
        return remove(root_, value);
    }

    // Returns true if value is stored in the tree.
    bool search(const T& value) const {
        return find(root_, value) != nullptr;
    }

    // Returns a pointer to value in the tree, or nullptr if value is absent.
    T* find(const T& value) {
        Node* node = find(root_, value);
        return node ? &node->data : nullptr;
    }

    // Returns a const pointer to value in the tree, or nullptr if value is absent.
    const T* find(const T& value) const {
        Node* node = find(root_, value);
        return node ? &node->data : nullptr;
    }

    // Visits values in preorder: root, left subtree, right subtree.
    template <typename Func>
    void preorder(Func action) const {
        preorder(root_, action);
    }

    // Visits values in inorder: left subtree, root, right subtree.
    template <typename Func>
    void inorder(Func action) const {
        inorder(root_, action);
    }

    // Visits values in postorder: left subtree, right subtree, root.
    template <typename Func>
    void postorder(Func action) const {
        postorder(root_, action);
    }

    // Returns the number of values stored in the tree.
    std::size_t size() const {
        return count_;
    }

    // Returns true when the tree has no values.
    bool empty() const {
        return count_ == 0;
    }

    // Removes all values from the tree.
    void clear() {
        clear(root_);
        root_ = nullptr;
        count_ = 0;
    }
};

} // namespace algoracoon

#endif // ALGORACOON_BST_HPP
