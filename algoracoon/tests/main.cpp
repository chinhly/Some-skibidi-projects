#include "lib/algoracoon.hpp"
#include <iostream>
#include <string>

using namespace algoracoon;

int main() {
    LinkedList<int> list;
    list.insertBack(2);
    list.insertFront(1);
    list.insertAt(2, 3);
    std::cout << "LinkedList forward:";
    list.forEachForward([](int x) { std::cout << ' ' << x; });
    std::cout << "\nLinkedList backward:";
    list.forEachBackward([](int x) { std::cout << ' ' << x; });
    std::cout << "\n";

    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    std::cout << "Stack top: " << stack.top() << "\n";

    Queue<std::string> queue;
    queue.enqueue("first");
    queue.enqueue("second");
    std::cout << "Queue front: " << queue.front() << "\n";

    PriorityQueue<int> pq;
    pq.insert(4);
    pq.insert(9);
    pq.insert(1);
    std::cout << "PriorityQueue extract: " << pq.extract() << "\n";

    int arr[] = {5, 2, 9, 1, 5, 6};
    mergeSort(arr, 6);
    std::cout << "Sorted array:";
    for (int x : arr) std::cout << ' ' << x;
    std::cout << "\nIndex of 5: " << binarySearch(arr, 6, 5) << "\n";

    BST<int> bst;
    bst.insert(8);
    bst.insert(3);
    bst.insert(10);
    std::cout << "BST inorder:";
    bst.inorder([](int x) { std::cout << ' ' << x; });
    std::cout << "\n";

    AVL<int> avl;
    avl.insert(30);
    avl.insert(20);
    avl.insert(10);
    std::cout << "AVL inorder:";
    avl.inorder([](int x) { std::cout << ' ' << x; });
    std::cout << "\nAVL height: " << avl.height() << "\n";

    HashTable<std::string, int> table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("alice", 95);
    std::cout << "HashTable alice: " << *table.find("alice") << "\n";
    std::cout << "HashTable contains bob: " << table.contains("bob") << "\n";

    std::cout << "\nNhan phim Enter de thoat...";
    std::cin.get();

    return 0;
}
