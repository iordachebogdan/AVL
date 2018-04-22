//
// Created by Bogdan Iordache on 3/5/2018.
//

#ifndef DIRECTEDGRAPHHANDLER_UTIL_VECTOR_H
#define DIRECTEDGRAPHHANDLER_UTIL_VECTOR_H

#include <stdexcept>
#include <algorithm>

namespace util {
    typedef unsigned int size_t;

    //implementation of STL-like vector
    template<typename T>
    class Vector {
    public:
        typedef T* iterator;
        typedef const T* const_iterator;

        //constructors
        Vector();
        explicit Vector(size_t size, T value = T());
        Vector(const Vector& rhs);

        //assignment operator
        Vector& operator = (const Vector& rhs);

        //destructor
        virtual ~Vector();

        //method that returns the number of elements in this vector
        size_t size() const;

        //method that returns the current capacity of this vector
        size_t capacity() const;

        //method that returns true if there are no elements in this vector
        bool empty() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;

        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

        //method that inserts an element at the end of the vector
        void push_back(const T& value);

        //method that erases the element at the back of the vector
        void pop_back();

        //method that erases all elements from this vector
        void clear();

        //overload for operator [] to access the element at the specified index
        T& operator[] (size_t index);
        const T& operator[] (size_t index) const;

    protected:
        size_t size_, capacity_;
        T* data_;

        void expand_capacity();
    };

    template<typename T>
    Vector<T>::Vector() : size_(0), capacity_(0), data_(nullptr) {}

    template<typename T>
    Vector<T>::Vector(size_t size, T value) :
            size_(size),
            capacity_(size) {
        data_ = (size == 0 ? nullptr : new T[size]);
        for (size_t i = 0; i < size; ++i)
            data_[i] = value;
    }

    template<typename T>
    Vector<T>::Vector(const Vector &rhs) {
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        data_ = (capacity_ == 0 ? nullptr : new T[capacity_]);
        for (size_t i = 0; i < size_; ++i)
            data_[i] = rhs.data_[i];
    }

    template <typename T>
    Vector<T>& Vector<T>::operator = (const Vector &rhs) {
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        T* copied_data = (capacity_ == 0 ? nullptr : new T[capacity_]);
        for (size_t i = 0; i < size_; ++i)
            copied_data[i] = rhs.data_[i];
        if (data_ != nullptr)
            delete[] data_;
        data_ = copied_data;
        return (*this);
    }

    template<typename T>
    Vector<T>::~Vector() {
        clear();
    }

    template<typename T>
    size_t Vector<T>::size() const {
        return size_;
    }

    template <typename T>
    size_t Vector<T>::capacity() const {
        return capacity_;
    }

    template<typename T>
    bool Vector<T>::empty() const {
        return size_ == 0;
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::begin() {
        return data_;
    }

    template<typename T>
    typename Vector<T>::const_iterator Vector<T>::begin() const {
        return data_;
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::end() {
        return data_ + size_;
    }

    template<typename T>
    typename Vector<T>::const_iterator Vector<T>::end() const {
        return data_ + size_;
    }

    template<typename T>
    const T& Vector<T>::front() const {
        if (empty())
            throw std::out_of_range("Container is empty!");
        return data_[0];
    }

    template<typename T>
    T& Vector<T>::front() {
        return const_cast<T&>(
                static_cast< const Vector<T>& >(*this).front()
        );
    }

    template<typename T>
    const T& Vector<T>::back() const {
        if (empty())
            throw std::out_of_range("Container is empty!");
        return data_[size_ - 1];
    }

    template<typename T>
    T& Vector<T>::back() {
        return const_cast<T&>(
                static_cast< const Vector<T>& >(*this).back()
        );
    }

    template<typename T>
    const T& Vector<T>::operator[](size_t index) const {
        if (index >= size_)
            throw std::out_of_range("Invalid index!");
        return data_[index];
    }

    template<typename T>
    T& Vector<T>::operator[](size_t index) {
        return const_cast<T&>(
                static_cast< const Vector<T>& >(*this)[index]
        );
    }

    template<typename T>
    void Vector<T>::push_back(const T &value) {
        if (size_ == capacity_)
            expand_capacity();
        data_[size_++] = value;
    }

    template <typename T>
    void Vector<T>::pop_back() {
        if (empty())
            throw std::out_of_range("Container is empty!");
        data_[--size_].~T();
    }

    template <typename T>
    void Vector<T>::clear() {
        if (data_ != nullptr)
            delete[] data_;
        size_ = capacity_ = 0;
        data_ = nullptr;
    }

    template<typename T>
    void Vector<T>::expand_capacity() {
        capacity_ = std::max((size_t)1, 2 * capacity_);
        T* new_data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i)
            new_data[i] = data_[i];
        if (data_ != nullptr)
            delete[] data_;
        data_ = new_data;
    }
}

#endif //DIRECTEDGRAPHHANDLER_UTIL_VECTOR_H