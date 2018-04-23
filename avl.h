//
// Created by Bogdan Iordache on 4/22/2018.
//

#ifndef AVL_AVL_H
#define AVL_AVL_H

#include "bstbase.h"
#include <algorithm>

namespace bst {

    template <typename T>
    class AVL;

    template <typename T>
    class NodeAVL : public NodeBase<T> {
      public:
        explicit NodeAVL(NodeAVL* = nullptr, NodeAVL* = nullptr, T = T(), NodeAVL* = nullptr);
        NodeAVL(const NodeAVL&) = delete;
        NodeAVL& operator=(const NodeAVL&) = delete;
        virtual ~NodeAVL() = default;

        NodeBase<T>* insert(const T&);
        NodeBase<T>* erase(const T&);
        int get_height() const;
        NodeBase<T>* find(const T&);
        NodeBase<T>* get_next();
      protected:
        void update();
        NodeAVL* rotate_left();
        NodeAVL*rotate_right();
        NodeAVL* balance();

        NodeAVL* parent_;
        int height_, balance_;
        friend class AVL<T>;
    };

    template <typename T>
    class AVL : public BSTBase<T> {
      public:
        explicit AVL(NodeAVL<T>* = nullptr);
        explicit AVL(const util::Vector<T>&);
        AVL(const AVL&);
        AVL& operator=(const AVL&);
        virtual ~AVL();

        void insert(const T&);
      protected:
    };

    //implementation section
    template <typename T>
    NodeAVL<T>::NodeAVL(NodeAVL *left, NodeAVL *right, T data, NodeAVL* parent) :
            NodeBase<T>(left, right, data),
            parent_(parent) {
        if (left != nullptr) left->parent_ = this;
        if (right != nullptr) right->parent_ = this;
        update();
    }

    template <typename T>
    NodeBase<T>* NodeAVL<T>::insert(const T &value) {
        if (value < this->data_) {
            if (this->left_ == nullptr) {
                this->left_ = new NodeAVL(nullptr, nullptr, value, this);
                dynamic_cast<NodeAVL*>(this->left_)->update();
            }
            else
                this->left_ = this->left_->insert(value);
        }
        if (value > this->data_) {
            if (this->right_ == nullptr) {
                this->right_ = new NodeAVL(nullptr, nullptr, value, this);
                dynamic_cast<NodeAVL*>(this->right_)->update();
            }
            else
                this->right_ = this->right_->insert(value);
        }
        return balance();
    }

    template <typename T>
    NodeBase<T>* NodeAVL<T>::erase(const T &value) {
        NodeAVL<T>* res;
        if (this->data_ == value) {
            if (this->left_ == nullptr || this->right_ == nullptr) {
                res = dynamic_cast<NodeAVL<T>*>(this->left_ == nullptr ? this->right_ : this->left_);
                delete this;
                return res;
            }
            else {
                for (res = dynamic_cast<NodeAVL<T>*>(this->right_);
                     res->left_ != nullptr; res = dynamic_cast<NodeAVL<T>*>(res->left_));
                this->data_ = res->data_;
                res->data_ = value;
                this->right_ = this->right_->erase(value);
                return balance();
            }
        }
        if (value < this->data_)
            this->left_ = this->left_->erase(value);
        else
            this->right_ = this->right_->erase(value);
        return balance();
    }

    template <typename T>
    int NodeAVL<T>::get_height() const {
        return height_;
    }

    template <typename T>
    NodeBase<T>* NodeAVL<T>::find(const T &value) {
        if (this->data_ == value)
            return this;
        if (this->data_ > value)
            return (this->left_ == nullptr ? nullptr : this->left_->find(value));
        else
            return (this->right_ == nullptr ? nullptr : this->right_->find(value));
    }

    template <typename T>
    NodeBase<T>* NodeAVL<T>::get_next() {
        NodeAVL* next = this;
        if (next->right_ == nullptr) {
            while (next->parent_ != nullptr && next->parent_->right_ == next)
                next = next->parent_;
            next = next->parent_;
        }
        else {
            next = dynamic_cast<NodeAVL<T>*>(next->right_);
            while (next->left_ != nullptr)
                next = dynamic_cast<NodeAVL<T>*>(next->left_);
        }
        return next;
    }

    template <typename T>
    void NodeAVL<T>::update() {
        height_ = 1;
        if (this->left_ != nullptr)
            height_ = std::max(height_, dynamic_cast<NodeAVL*>(this->left_)->height_ + 1);
        if (this->right_ != nullptr)
            height_ = std::max(height_, dynamic_cast<NodeAVL*>(this->right_)->height_ + 1);
        balance_ = (this->left_ == nullptr ? 0 : dynamic_cast<NodeAVL*>(this->left_)->height_)
                   - (this->right_ == nullptr ? 0 : dynamic_cast<NodeAVL*>(this->right_)->height_);
    }

    template <typename T>
    NodeAVL<T>* NodeAVL<T>::rotate_left() {
        NodeAVL<T>* res = dynamic_cast<NodeAVL<T>*>(this->left_);
        this->left_ = res->right_;
        if (this->left_ != nullptr)
            dynamic_cast<NodeAVL<T>*>(this->left_)->parent_ = this;
        res->right_ = this;
        res->parent_ = this->parent_;
        this->parent_ = res;
        this->update();
        res->update();
        return res;
    }

    template <typename T>
    NodeAVL<T>* NodeAVL<T>::rotate_right() {
        NodeAVL* res = dynamic_cast<NodeAVL*>(this->right_);
        this->right_ = res->left_;
        if (this->right_ != nullptr)
            dynamic_cast<NodeAVL<T>*>(this->right_)->parent_ = this;
        res->left_ = this;
        res->parent_ = this->parent_;
        this->parent_ = res;
        this->update();
        res->update();
        return res;
    }

    template <typename T>
    NodeAVL<T>* NodeAVL<T>::balance() {
        update();
        if (balance_ > 1) {
            auto left_right = dynamic_cast<NodeAVL<T>*>(dynamic_cast<NodeAVL<T>*>(this->left_)->right_);
            auto left_left = dynamic_cast<NodeAVL<T>*>(dynamic_cast<NodeAVL<T>*>(this->left_)->left_);
            if ((left_right == nullptr ? 0 : left_right->get_height())
                    > (left_left == nullptr ? 0 : left_left->get_height()))
                this->left_ = dynamic_cast<NodeAVL<T>*>(this->left_)->rotate_right();
            return rotate_left();
        }
        if (balance_ < -1) {
            auto right_left = dynamic_cast<NodeAVL<T>*>(dynamic_cast<NodeAVL<T>*>(this->right_)->left_);
            auto right_right = dynamic_cast<NodeAVL<T>*>(dynamic_cast<NodeAVL<T>*>(this->right_)->right_);
            if ((right_left == nullptr ? 0 : right_left->get_height())
                > (right_right == nullptr ? 0 : right_right->get_height()))
                this->right_ = dynamic_cast<NodeAVL<T>*>(this->right_)->rotate_left();
            return rotate_right();
        }
        return this;
    }

    template <typename T>
    AVL<T>::AVL(NodeAVL<T> *root) : BSTBase<T>(root) {}

    template <typename T>
    AVL<T>::AVL(const util::Vector<T> &values) {
        for (typename util::Vector<T>::const_iterator it = values.begin(); it != values.end(); ++it)
            insert(*it);
    }

    template <typename T>
    AVL<T>::AVL(const AVL<T> &rhs) : AVL<T>(dynamic_cast<NodeAVL<T>*>(rhs.root_)->inorder_traversal()) {}

    template <typename T>
    AVL<T>& AVL<T>::operator=(const AVL<T> &rhs) {
        if (this == &rhs)
            return (*this);
        BSTBase<T>::clear();
        auto values = dynamic_cast<NodeAVL<T>*>(rhs.root_)->inorder_traversal();
        for (typename util::Vector<T>::const_iterator it = values.begin(); it != values.end(); ++it)
            insert(*it);
    }

    template <typename T>
    AVL<T>::~AVL() {
        BSTBase<T>::~BSTBase();
    }

    template <typename T>
    void AVL<T>::insert(const T &value) {
        if (this->root_ == nullptr) {
            this->root_ = new NodeAVL<T>(nullptr, nullptr, value);
            return;
        }
        this->root_ = this->root_->insert(value);
    }

}

#endif //AVL_AVL_H
