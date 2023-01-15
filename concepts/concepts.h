#ifndef TOOLS_CONCEPTS_H
#define TOOLS_CONCEPTS_H

#include <concepts>
#include <limits>

#include <vector>

namespace vector {
    template <typename Iterator, typename T>
    concept IsInputIterator = (
        std::__is_cpp17_input_iterator<Iterator>::value &&
        not std::__is_cpp17_forward_iterator<Iterator>::value &&
        std::is_constructible<T, typename std::iterator_traits<Iterator>::reference>::value
    );

    template <typename Iterator, typename T>
    concept IsForwardIterator = (
        std::__is_cpp17_forward_iterator<Iterator>::value &&
        std::is_constructible<T, typename std::iterator_traits<Iterator>::reference>::value
    );

    template <typename Iterator, typename T>
    concept IsValidIterator = (
        IsForwardIterator<Iterator, T> || IsInputIterator<Iterator, T>
    );
}

#endif //TOOLS_CONCEPTS_H
