//
// Created by Bogdan Iordache on 4/22/2018.
//

#ifndef AVL_BSTBASE_H
#define AVL_BSTBASE_H

#include "util_vector.h"

namespace bst {

    template <typename T>
    class BSTBase;

    template <typename T>
    class NodeBase {
      public:
        explicit NodeBase(NodeBase* = nullptr, NodeBase* = nullptr, T = T());
        NodeBase(const NodeBase&) = delete;
        NodeBase& operator = (const NodeBase&) = delete;
        virtual ~NodeBase() = default;

        const T& get_data() const;
        virtual NodeBase* insert(const T&) = 0;
        virtual NodeBase* erase(const T&) = 0;
        virtual NodeBase* find(const T&) = 0;
        virtual NodeBase* get_next() = 0;
      protected:
        void set_data(const T&);
        util::Vector<T> inorder_traversal();

        NodeBase *left_, *right_;
        T data_;

        friend class BSTBase<T>;
    };

    template <typename T>
    class BSTBase {
      public:
        class iterator {
          public:
            iterator(NodeBase<T>* = nullptr);
            iterator(const iterator&) = default;
            iterator& operator=(const iterator&) = default;
            ~iterator() = default;
            iterator& operator++();
            iterator operator++(int);
            const T& operator*() const;
            const T* operator->();
            bool operator==(const iterator&);
          protected:
            NodeBase<T>* ptr_;
        };

        BSTBase(NodeBase<T>* = nullptr);
        explicit BSTBase(const util::Vector<T>&);
        BSTBase(const BSTBase&);
        BSTBase& operator = (const BSTBase&);
        virtual ~BSTBase();

        iterator begin();
        iterator end();
        virtual void insert(const T&) = 0;
        void erase(const T&);
        iterator find(const T&);
        void clear();
      protected:
        void dump(NodeBase<T>*);

        NodeBase<T>* root_;
    };

    //implementation section
    template <typename T>
    NodeBase<T>::NodeBase(NodeBase *left, NodeBase *right, T data) :
            left_(left),
            right_(right),
            data_(data) {
    }

    template <typename T>
    const T& NodeBase<T>::get_data() const {
        return data_;
    }

    template <typename T>
    void NodeBase<T>::set_data(const T& data) {
        data_ = data;
    }

    template <typename T>
    util::Vector<T> NodeBase<T>::inorder_traversal() {
        util::Vector<T> left_traversal = (left_ == nullptr ? util::Vector<T>() : left_->inorder_traversal());
        util::Vector<T> right_traversal = (right_ == nullptr ? util::Vector<T>() : right_->inorder_traversal());
        util::Vector<T> traversal;
        for (typename util::Vector<T>::iterator it = left_traversal.begin(); it != left_traversal.end(); ++it)
            traversal.push_back(*it);
        traversal.push_back(data_);
        for (typename util::Vector<T>::iterator it = right_traversal.begin(); it != right_traversal.end(); ++it)
            traversal.push_back(*it);
        return traversal;
    }

    template <typename T>
    BSTBase<T>::iterator::iterator(NodeBase<T>* ptr) : ptr_(ptr) {}

    template <typename T>
    typename BSTBase<T>::iterator& BSTBase<T>::iterator::operator++() {
        *this = ptr_->get_next();
        return *this;
    }

    template <typename T>
    typename BSTBase<T>::iterator BSTBase<T>::iterator::operator++(int) {
        iterator temp = *this;
        ++(*this);
        return temp;
    }

    template <typename T>
    const T& BSTBase<T>::iterator::operator*() const {
        return ptr_->get_data();
    }

    template <typename T>
    const T* BSTBase<T>::iterator::operator->() {
        return &ptr_->get_data();
    }

    template <typename T>
    bool BSTBase<T>::iterator::operator==(const iterator &rhs) {
        return ptr_ == rhs.ptr_;
    }

    template <typename T>
    BSTBase<T>::BSTBase(NodeBase<T> *root) : root_(root) {}

    template <typename T>
    BSTBase<T>::BSTBase(const util::Vector<T> &values) : root_(nullptr) {
        for (typename util::Vector<T>::const_iterator it = values.begin(); it != values.end(); ++it)
            insert(*it);
    }

    template <typename T>
    BSTBase<T>::BSTBase(const BSTBase &rhs) : BSTBase(rhs.root_->inorder_traversal()) {
    }

    template <typename T>
    BSTBase<T>& BSTBase<T>::operator=(const BSTBase &rhs) {
        if (this == &rhs)
            return (*this);
        clear();
        BSTBase(rhs.root_->inorder_traversal());
    }

    template <typename T>
    BSTBase<T>::~BSTBase() {
        clear();
    }

    template <typename T>
    typename BSTBase<T>::iterator BSTBase<T>::begin() {
        if (root_ == nullptr)
            return iterator();
        NodeBase<T>* aux = root_;
        while (aux->left_ != nullptr)
            aux = aux->left_;
        return iterator(aux);
    }

    template <typename T>
    typename BSTBase<T>::iterator BSTBase<T>::end() {
        return iterator();
    }

    template <typename T>
    void BSTBase<T>::erase(const T &value) {
        if (root_ != nullptr && !(find(value) == end()))
            root_->erase(value);
    }

    template <typename T>
    typename BSTBase<T>::iterator BSTBase<T>::find(const T &value) {
        if (root_ == nullptr)
            return iterator();
        else
            return iterator(root_->find(value));
    }

    template <typename T>
    void BSTBase<T>::clear() {
        dump(root_);
        root_ = nullptr;
    }

    template <typename T>
    void BSTBase<T>::dump(NodeBase<T>* ptr) {
        if (ptr == nullptr)
            return;
        dump(ptr->left_);
        dump(ptr->right_);
        delete ptr;
    }

}

#endif //AVL_BSTBASE_H
