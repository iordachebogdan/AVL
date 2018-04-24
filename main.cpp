#include <iostream>
#include <cstdlib>
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
    my_set2.erase(6);

    std::cout << my_set;
    std::cout << std::endl;

    my_set.print_leaves(std::cout);
    std::cout << my_set2 << std::endl;

    std::cout << my_set2 - my_set << std::endl;
    std::cout << my_set2 + my_set << std::endl;
    std::cout << my_set2 * my_set << std::endl;
    std::cout << my_set - my_set2 << std::endl;

    bst::BSTBase<int>* set = new bst::AVL<int>;
    for (int i = 1; i <= 100000; ++i)
        set->insert(rand() % 10000);
    for (int i = 1; i <= 50000; ++i)
        set->erase(rand() % 10000);
    std::cout << *dynamic_cast<bst::AVL<int>*>(set) << std::endl;

    return 0;
}