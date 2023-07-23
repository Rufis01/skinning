#ifndef BUFFERVIEW_H
#define BUFFERVIEW_H

#include <vector>

struct BufferView
{
    BufferView(int offset, int length, int stride, std::vector<unsigned char> &buffer) :
    byteOffset(offset), byteLength(length), byteStride(stride), buffer(buffer)
    {
    };

    long byteOffset;
    long byteLength;
    long byteStride;

    std::vector<unsigned char> &buffer;
};

#endif