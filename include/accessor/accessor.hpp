#ifndef ACCESSOR_HPP
#define ACCESSOR_HPP

#include "bufferview.hpp"

template <typename T> class Accessor
{
    private:
    const T *getElementAt(int idx) const;

    class Iterator
    {
        public:
        Iterator operator++();
        const T &operator*() const;
        bool operator!=(const Iterator &other) const;
        Iterator(Accessor *accessor, int idx);

        private:
        Accessor<T> *accessor;
        int currentIndex;
    };

    BufferView bufferView;  
    long byteOffset;
    long count;
    long elementSize;

    public:
    const T &operator[](std::size_t idx) const;
    long getCount();
    Accessor(BufferView &bufferView, long byteOffset, long count);
    Iterator begin();
    Iterator end();
};

#include "accessor.inl"

#endif