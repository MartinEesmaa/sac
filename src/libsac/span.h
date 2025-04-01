#ifndef SPAN_H
#define SPAN_H

#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <algorithm>

template <typename T>
class span {
public:
    span(T* ptr, std::size_t size) : data_(ptr), size_(size) {
        if (!ptr && size > 0) {
            throw std::invalid_argument("Span constructed with null pointer and non-zero size.");
        }
    }

    span(T* begin, T* end) : data_(begin), size_(end - begin) {
        if (end < begin) {
            throw std::invalid_argument("Invalid range for span: end is less than begin.");
        }
    }

    template <typename U, typename = typename std::enable_if<std::is_const<U>::value && std::is_convertible<T*, U*>::value>::type>
    span(const span<U>& other) : data_(other.data()), size_(other.size()) {}

    T& operator[](std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    const T& operator[](std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    T* data() { return data_; }
    const T* data() const { return data_; }
    std::size_t size() const { return size_; }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }
    const T* begin() const { return data_; }
    const T* end() const { return data_ + size_; }

    bool operator==(const span& other) const {
        return size_ == other.size_ && std::equal(begin(), end(), other.begin());
    }

    bool operator!=(const span& other) const {
        return !(*this == other);
    }

private:
    T* data_;
    std::size_t size_;
};

#endif
