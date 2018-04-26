#include <iostream>
#include <fstream>
#include <cstdlib>
#include "avl.h"

class Demo {
  public:
    static void run_demo() {
        std::ifstream fin("data.in");
        std::ofstream fout("data.out");

        bst::AVL<int> my_set;
        fin >> my_set;
        int q; fin >> q;
        while (q--) {
            int op, x; fin >> op >> x;
            switch(op) {
                case 0: my_set.insert(x); break;
                case 1: my_set.erase(x); break;
                default: fout << (my_set.find(x) == my_set.end() ? 0 : 1);
            }
        }
        fout << '\n';
        fout << my_set << '\n';

        bst::BSTBase<int>* my_set2 = new bst::AVL<int>(my_set);
        my_set2->clear();
        *dynamic_cast<bst::AVL<int>*>(my_set2) = my_set;

        for (int i = 1; i <= 3000; ++i)
            my_set2->insert(rand() % 1000);
        for (int i = 1; i <= 1000; ++i)
            my_set2->erase(rand() % 1000);
        fout << *dynamic_cast<bst::AVL<int>*>(my_set2) << std::endl << std::endl;

        bst::AVL<int>& my_set2_ref = *dynamic_cast<bst::AVL<int>*>(my_set2);
        fout << my_set2_ref - my_set << std::endl;
        fout << my_set2_ref + my_set << std::endl;
        fout << my_set2_ref * my_set << std::endl;
        fout << my_set - my_set2_ref << std::endl << std::endl;

        std::cerr << ((my_set2_ref - my_set) == (my_set2_ref - (my_set2_ref * my_set)) ? "OK!" : "Nope..");

        auto it = my_set2_ref.begin();
        for (int i = 0; i < 10; ++i)
            it++;
        fout << *it << '\n';

        my_set2_ref.clear();
        fout << my_set2_ref - my_set << std::endl;
        fout << my_set2_ref + my_set << std::endl;
        fout << my_set2_ref * my_set << std::endl;
        fout << my_set - my_set2_ref << std::endl;

        delete my_set2;
    }
};

int main() {
    Demo::run_demo();
    return 0;
}
