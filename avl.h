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
        explicit NodeAVL(NodeAVL* = nullptr, NodeAVL* = nullptr, T = T());
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
      protected:
    };

    //implementation section
    template <typename T>
    NodeAVL<T>::NodeAVL(NodeAVL *left, NodeAVL *right, T data) :
            NodeBase<T>(left, right, data),
            parent_(nullptr) {
        left->parent_ = this;
        right->parent_ = this;
        update();
    }

    template <typename T>
    NodeBase<T>* NodeAVL<T>::insert(const T &value) {
        if (value < this->data_) {
            if (this->left_ == nullptr) {
                this->left_ = new NodeAVL(nullptr, nullptr, value);
                static_cast<NodeAVL*>(this->left_)->update();
            }
            else
                this->left_->insert(value);
        }
        if (value > this->data_) {
            if (this->right_ == nullptr) {
                this->right_ = new NodeAVL(nullptr, nullptr, value);
                static_cast<NodeAVL*>(this->right_)->update();
            }
            else
                this->right_->insert(value);
        }
        return balance();
    }

    template <typename T>
    NodeBase<T>* NodeAVL<T>::erase(const T &value) {
        NodeBase<T>* res;
        if (this->data_ == value) {
            if (this->left_ == nullptr || this->right_ == nullptr) {
                res = (this->left_ == nullptr ? this->right_ : this->left_);
                delete this;
                return res;
            }
            else {
                for (res = this->right_; res->left_ != nullptr; res = res->left_);
                this->data_ = res->data_;
                this->right_->erase(value);
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
            next = next->right_;
            while (next->left_ != nullptr)
                next = next->left_;
        }
        return next;
    }

    template <typename T>
    void NodeAVL<T>::update() {
        height_ = 1;
        if (this->left_ != nullptr)
            height_ = std::max(height_, static_cast<NodeAVL*>(this->left_)->height_ + 1);
        if (this->right_ != nullptr)
            height_ = std::max(height_, static_cast<NodeAVL*>(this->right_)->height_ + 1);
        balance_ = static_cast<NodeAVL*>(this->left_)->height_ - static_cast<NodeAVL*>(this->right_)->height_;
    }

    template <typename T>
    NodeAVL<T>* NodeAVL<T>::rotate_left() {
        NodeAVL<T>* res = static_cast<NodeAVL*>(this->left_);
        this->left_ = res->right_;
        res->right_ = this;
        this->update();
        res->update();
        return res;
    }

    template <typename T>
    NodeAVL<T>* NodeAVL<T>::rotate_right() {
        NodeAVL* res = static_cast<NodeAVL*>(this->right_);
        this->right_ = res->left_;
        res->left_ = this;
        this->update();
        res->update();
        return res;
    }

    template <typename T>
    NodeAVL<T>* NodeAVL<T>::balance() {
        update();
        if (balance_ > 1) {
            if (static_cast<NodeAVL*>(this->left_->right_)->get_height()
                    > static_cast<NodeAVL*>(this->left_->left_)->get_height())
                this->left_ = static_cast<NodeAVL*>(this->left_)->rotate_right();
            return rotate_left();
        }
        if (balance_ < -1) {
            if (static_cast<NodeAVL*>(this->right_->left_)->get_height()
                    > static_cast<NodeAVL*>(this->right_->right_)->get_height())
                this->right_ = static_cast<NodeAVL*>(this->right_)->rotate_left();
            return rotate_right();
        }
        return this;
    }
}

#endif //AVL_AVL_H
