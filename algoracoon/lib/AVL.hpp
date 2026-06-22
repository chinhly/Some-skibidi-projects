#ifndef ALGORACOON_AVL_HPP
#define ALGORACOON_AVL_HPP

#include <cstddef>
#include <functional>

namespace algoracoon {

template <typename T, typename Comp = std::less<T>>
struct AVL {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;
        explicit Node(const T& value) : data(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root_;
    std::size_t count_;
    Comp cmp_;

    bool equals(const T& a, const T& b) const {
        return !cmp_(a, b) && !cmp_(b, a);
    }

    int height(Node* node) const {
        return node ? node->height : 0;
    }

    int maxInt(int a, int b) const {
        return a > b ? a : b;
    }

    void updateHeight(Node* node) {
        if (node) node->height = 1 + maxInt(height(node->left), height(node->right));
    }

    int balanceFactor(Node* node) const {
        return node ? height(node->left) - height(node->right) : 0;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* t2 = x->right;
        x->right = y;
        y->left = t2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* t2 = y->left;
        y->left = x;
        x->right = t2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    Node* rebalance(Node* node) {
        if (!node) return nullptr;
        updateHeight(node);
        int balance = balanceFactor(node);

        if (balance > 1) {
            if (balanceFactor(node->left) < 0) node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        if (balance < -1) {
            if (balanceFactor(node->right) > 0) node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        return node;
    }

    Node* clone(Node* node) {
        if (!node) return nullptr;
        Node* next = new Node(node->data);
        next->height = node->height;
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

    Node* insert(Node* node, const T& value, bool& inserted) {
        if (!node) {
            inserted = true;
            ++count_;
            return new Node(value);
        }
        if (equals(value, node->data)) {
            inserted = false;
            return node;
        }
        if (cmp_(value, node->data)) node->left = insert(node->left, value, inserted);
        else node->right = insert(node->right, value, inserted);
        return rebalance(node);
    }

    Node* minNode(Node* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    Node* remove(Node* node, const T& value, bool& removed) {
        if (!node) return nullptr;

        if (cmp_(value, node->data)) {
            node->left = remove(node->left, value, removed);
        } else if (cmp_(node->data, value)) {
            node->right = remove(node->right, value, removed);
        } else {
            removed = true;
            if (!node->left || !node->right) {
                Node* child = node->left ? node->left : node->right;
                delete node;
                --count_;
                return child;
            }
            Node* successor = minNode(node->right);
            node->data = successor->data;
            bool dummy = false;
            node->right = remove(node->right, successor->data, dummy);
        }
        return rebalance(node);
    }

    Node* findNode(Node* node, const T& value) const {
        while (node) {
            if (equals(value, node->data)) return node;
            node = cmp_(value, node->data) ? node->left : node->right;
        }
        return nullptr;
    }

    template <typename Predicate>
    Node* findIf(Node* node, Predicate pred) const {
        if (!node) return nullptr;
        if (pred(node->data)) return node;
        Node* leftAnswer = findIf(node->left, pred);
        if (leftAnswer) return leftAnswer;
        return findIf(node->right, pred);
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
    // Creates an empty AVL tree with an optional comparator.
    explicit AVL(Comp cmp = Comp()) : root_(nullptr), count_(0), cmp_(cmp) {}

    // Creates a deep copy of another AVL tree.
    AVL(const AVL& other) : root_(nullptr), count_(other.count_), cmp_(other.cmp_) {
        root_ = clone(other.root_);
    }

    // Moves another AVL tree into this one in constant time.
    AVL(AVL&& other) noexcept : root_(other.root_), count_(other.count_), cmp_(other.cmp_) {
        other.root_ = nullptr;
        other.count_ = 0;
    }

    // Replaces this AVL tree with a deep copy of another AVL tree.
    AVL& operator=(const AVL& other) {
        if (this != &other) {
            clear();
            cmp_ = other.cmp_;
            count_ = other.count_;
            root_ = clone(other.root_);
        }
        return *this;
    }

    // Replaces this AVL tree by moving another AVL tree into it.
    AVL& operator=(AVL&& other) noexcept {
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

    // Releases every node in the AVL tree.
    ~AVL() {
        clear();
    }

    // Inserts value and returns true if it was not already present.
    bool insert(const T& value) {
        bool inserted = false;
        root_ = insert(root_, value, inserted);
        return inserted;
    }

    // Removes value and returns true if a value was removed.
    bool remove(const T& value) {
        bool removed = false;
        root_ = remove(root_, value, removed);
        return removed;
    }

    // Returns true if value is stored in the AVL tree.
    bool search(const T& value) const {
        return findNode(root_, value) != nullptr;
    }

    // Returns a pointer to value in the tree, or nullptr if value is absent.
    T* find(const T& value) {
        Node* node = findNode(root_, value);
        return node ? &node->data : nullptr;
    }

    // Returns a const pointer to value in the tree, or nullptr if value is absent.
    const T* find(const T& value) const {
        Node* node = findNode(root_, value);
        return node ? &node->data : nullptr;
    }

    // Returns a pointer to the first value satisfying pred(value), or nullptr if none exists.
    template <typename Predicate>
    T* findIf(Predicate pred) {
        Node* node = findIf(root_, pred);
        return node ? &node->data : nullptr;
    }

    // Returns a const pointer to the first value satisfying pred(value), or nullptr if none exists.
    template <typename Predicate>
    const T* findIf(Predicate pred) const {
        Node* node = findIf(root_, pred);
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

    // Returns the number of values stored in the AVL tree.
    std::size_t size() const {
        return count_;
    }

    // Returns true when the AVL tree has no values.
    bool empty() const {
        return count_ == 0;
    }

    // Removes all values from the AVL tree.
    void clear() {
        clear(root_);
        root_ = nullptr;
        count_ = 0;
    }

    // Returns the height of the AVL tree root.
    int height() const {
        return height(root_);
    }
};

} // namespace algoracoon

#endif // ALGORACOON_AVL_HPP
