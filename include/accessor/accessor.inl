#ifndef ACCESSOR_INL
#define ACCESSOR_INL

#include "accessor.hpp"
#include "log.h"

template <typename T> Accessor<T>::Accessor(BufferView &bufferView, long byteOffset, long count) : bufferView(bufferView), byteOffset(byteOffset), count(count)
{
    this->elementSize = this->bufferView.byteLength / this->count;
    if(this->bufferView.byteStride == 0)
        this->bufferView.byteStride = this->elementSize;
}

template <typename T> const T *Accessor<T>::getElementAt(int idx) const
{
    return (const T * const)(bufferView.buffer.data() + (byteOffset + bufferView.byteOffset + (bufferView.byteStride * idx)));
}

template <typename T> const T &Accessor<T>::operator[](std::size_t idx) const
{
    return *getElementAt(idx);
}

template <typename T> long Accessor<T>::getCount()
{
    return count;
}

//Iterator

template <typename T> Accessor<T>::Iterator::Iterator(Accessor *accessor, int idx) : accessor(accessor), currentIndex(idx)
{
}

template <typename T> Accessor<T>::Iterator Accessor<T>::Iterator::operator++()
{
    currentIndex++;
    return *this;
}

template <typename T> const T &Accessor<T>::Iterator::operator*() const
{
    return *accessor->getElementAt(currentIndex);
}

template <typename T> bool Accessor<T>::Iterator::operator!=(const Iterator& other) const
{
    return currentIndex != other.currentIndex;
}

template <typename T> Accessor<T>::Iterator Accessor<T>::begin()
{
    return Iterator(this, 0);
}

template <typename T> Accessor<T>::Iterator Accessor<T>::end()
{
    return Iterator(this, this->count);
}

#endif
