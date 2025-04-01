#ifndef ALIGNBUF_H
#define ALIGNBUF_H

#include <malloc.h>
#include <stdexcept>
template <typename T, std::size_t align_t=64>
struct align_alloc {
    using value_type = T;

    template <typename U>
    struct rebind {
        using other = align_alloc<U, align_t>;
    };

    constexpr align_alloc() noexcept = default;
    constexpr align_alloc(const align_alloc &) noexcept = default;


    template <typename U>
    constexpr align_alloc(const align_alloc<U, align_t> &) noexcept {}

    T* allocate(std::size_t n) {
        if (n == 0) {
            return nullptr;
        }

        void* ptr = _aligned_malloc(n * sizeof(T), align_t);
        if (!ptr) {
            throw std::bad_alloc();
        }
        return static_cast<T*>(ptr);
    }

    void deallocate(T* p, std::size_t n) noexcept {
        _aligned_free(p);
    }
};

template <typename T, typename U, std::size_t align_t>
bool operator==(const align_alloc<T, align_t>&, const align_alloc<U, align_t>&) noexcept {
    return true;
}

template <typename T, typename U, std::size_t align_t>
bool operator!=(const align_alloc<T, align_t>&, const align_alloc<U, align_t>&) noexcept {
    return false;
}

#endif
