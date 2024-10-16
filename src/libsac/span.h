#ifndef SPAN_H
#define SPAN_H

#include <cstddef>

template <typename T>
class span {
public:
    span(T* ptr, std::size_t size) : data_(ptr), size_(size) {}
    span(T* begin, T* end) : data_(begin), size_(end - begin) {}

    T& operator[](std::size_t index) const { return data_[index]; }
    T* data() const { return data_; }
    std::size_t size() const { return size_; }
    
    T* begin() const { return data_; }
    T* end() const { return data_ + size_; }

private:
    T* data_;
    std::size_t size_;
};

#endif
