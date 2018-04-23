#include <iostream>
#include "avl.h"

int main() {
    bst::AVL<int> my_set;
    my_set.insert(5);
    my_set.insert(6);
    my_set.insert(3);
    my_set.insert(2);
    my_set.insert(5);
    my_set.insert(2);
    bst::AVL<int> my_set2(my_set);
    bst::BSTBase<int>* ptr = &my_set2;
    ptr->insert(20);
    ptr->erase(7);
    ptr->erase(3);
    my_set = my_set2 = my_set2;
    my_set.insert(1);

    for (bst::AVL<int>::iterator it = my_set.begin(); it != my_set.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    my_set.print_leaves(std::cout);
    std::cout << my_set << std::endl;

    return 0;
}