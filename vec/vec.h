#ifndef TOOLS_VEC_H
#define TOOLS_VEC_H

#include <exception>
#include <concepts>
#include <limits>
#include <optional>
#include <ostream>
#include <vector>

#include "../concepts/concepts.h"

/// `Vec` is a wrapper over `std::vector` with additional functionality.
/// Most methods from the `std::vector` class are available, but some have different (more appropriate) names.
template <class T>
class Vec;

namespace error {
    class NoSuchElement : public std::exception {
    private:
        const char* message;
    public:
        explicit NoSuchElement(const char* msg) : message(msg) {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return message;
        }
    };

    class IndexOutOfBounds : public std::exception {
    private:
        const char* message;
    public:
        explicit IndexOutOfBounds(const char* msg) : message(msg) {}

        [[nodiscard]]
        const char* what() const noexcept override {
            return message;
        }
    };
}

namespace pattern {
    template <class T>
    using Pattern = T (*) (const T&);

    /// Used for an incremental sequence (e.g. 1, 2, 3, 4, ...).
    template <class T, int by = 1>
    Pattern<T> Incr = [](const T& val) -> T { return val + by; };

    /// Used for a decremental sequence (e.g. 100, 99, 98, 97, ...).
    template <class T, int by = 1>
    Pattern<T> Decr = [](const T& val) -> T { return val - by; };

    /// Used for a geometric sequence (e.g. 1, 2, 4, 8, ...).
    template <class T, int by = 2>
    Pattern<T> Mult = [](const T& val) -> T { return val * by; };
}

template <class T>
class Vec {
protected:
    /// An alias to the wrapped type.
    using Underlying = std::vector<T>;
private:
    std::vector<T> self;
public:
    /// A constant iterator to the wrapped type.
    using ConstIterator = typename Underlying::const_iterator;
    /// A constant reference to an element in the wrapped type.
    using ConstReference = typename Underlying::const_reference;
    /// A constant reverse iterator to the wrapped type.
    using ConstReverseIterator = typename Underlying::const_reverse_iterator;
    /// An iterator to the wrapped type.
    using Iterator = typename Underlying::iterator;
    /// A reference to an element in the wrapped type.
    using Reference = typename Underlying::reference;
    /// A size type to the wrapped type.
    using Size = typename Underlying::size_type;

    /// Constructs a container with as many elements as the range [first,last), with each element
    /// emplace-constructed from its corresponding element in that range, in the same order.
    template <typename SomeIterator>
    requires(vector::IsValidIterator<SomeIterator, T>)
    static auto from(SomeIterator begin, SomeIterator end) -> Vec<T> {
        auto result = Vec<T>();
        result.self = Underlying(begin, end);
        return result;
    }

    /// Constructs a container with a copy of each of the elements in `other`, in the same order.
    static auto from(const Vec<T>& other) -> Vec<T> {
        auto result = Vec<T>();
        result.self = Underlying(other.self);
        return result;
    }

    /// Constructs a container with a copy of each of the elements in `list`, in the same order.
    static auto from(std::initializer_list<T> list) -> Vec<T> {
        return Vec(list);
    }

    /// Constructs a container with `n` elements. Each element is a copy of `default_val` (if provided).
    static auto of(Size n, const T& default_val = T()) -> Vec<T> {
        auto result = Vec<T>();
        result.self = Underlying(n, default_val);
        return result;
    }

    /// Returns a reference to the element at position `i` in the vector.
    /// @throws IndexOutOfBounds if attempting to access an element at an invalid index.
    auto at(Size i) -> Reference {
        if (i >= self.size()) {
            std::string message = "invalid index for vector of size " + std::to_string(self.size()) + ".";
            throw error::IndexOutOfBounds(message.c_str());
        }
        return self.at(i);
    }

    /// Returns a reference to the element at position `i` in the vector.
    /// @throws IndexOutOfBounds if attempting to access an element at an invalid index.
    auto at(Size i) const -> ConstReference {
        if (i >= self.size()) {
            std::string message = "invalid index for vector of size " + std::to_string(self.size()) + ".";
            throw error::IndexOutOfBounds(message.c_str());
        }
        return self.at(i);
    }

    /// Returns an `Iterator` pointing to the first element in the vector.
    auto begin() -> Iterator {
        return self.begin();
    }

    /// Returns an `Iterator` pointing to the first element in the vector.
    auto begin() const -> ConstIterator {
        return self.begin();
    }

    /// Returns the capacity of the vector.
    auto cap() const -> Size {
        return self.capacity();
    }

    /// Returns a `ConstIterator` pointing to the first element in the vector.
    auto cbegin() const -> ConstIterator {
        return self.cbegin();
    }

    /// Returns a `ConstIterator` pointing to the past-the-end element in the vector.
    auto cend() const -> ConstIterator {
        return self.cend();
    }

    /// Removes all elements from the vector (which are destroyed), leaving the vector with a size of 0.
    auto clear() -> void {
        self.clear();
    }

    /// Returns a `ConstReverseIterator` pointing to the last element in the vector (i.e., its reverse beginning).
    auto crbegin() const -> ConstReverseIterator {
        return self.crbegin();
    }

    /// Returns a `ConstReverseIterator` pointing to the theoretical element preceding the first element in the
    /// vector (which is considered its reverse end).
    auto crend() const -> ConstReverseIterator {
        return self.crend();
    }

    template <class... Args>
    /// The vector is extended by inserting a new element at position `at`. This new element is constructed in place
    /// using `args` as the arguments for its construction.
    auto emplace(ConstIterator at, Args&&... args) -> Iterator {
        return self.emplace(at, args...);
    }

    /// Inserts a new element at the end of the vector, right after its current last element. This new element is
    /// constructed in place using `args` as the arguments for its construction.
    template <class... Args>
    auto emplace_back(Args&&... args) -> void {
        self.emplace_back(args...);
    }

    /// Returns an `Iterator` referring to the past-the-end element in the vector.
    auto end() -> Iterator {
        return self.end();
    }

    /// Returns an `Iterator` referring to the past-the-end element in the vector.
    auto end() const -> ConstIterator {
        return self.end();
    }

    /// Inserts a sequence of elements of length `n` at position `at`. Each element is a copy of `val`.
    auto fill(ConstIterator at, Size n, const T& val) -> Iterator {
        return self.insert(at, n, val);
    }

    /// Inserts a copy of `val` into position `at`.
    auto insert(ConstIterator at, const T& val) -> Iterator {
        return self.insert(at, val);
    }

    /// Inserts each element in `list` (in order) into the vector at position `at`.
    auto insert_list(ConstIterator at, std::initializer_list<T> list) -> Iterator {
        return self.insert(at, list);
    }

    /// Inserts the contents of the iterator at position `at` given by `begin` and `end`.
    template <class SomeIterator>
    requires(vector::IsValidIterator<SomeIterator, T>)
    auto insert_range(ConstIterator at, SomeIterator begin, SomeIterator end) -> Iterator {
        return self.insert(at, begin, end);
    }

    /// Returns if the vector is empty.
    [[nodiscard]]
    auto is_empty() const -> bool {
        return self.empty();
    }

    /// Returns the maximum number of elements that the vector can hold.
    auto max_size() const -> Size {
        return self.max_size();
    }

    /// Returns the last element in the vector.
    /// @throws NoSuchElement if the vector is empty.
    auto peek_back() -> Reference {
        if (self.empty()) {
            throw error::NoSuchElement("vector is empty.");
        }
        return self.back();
    }

    /// Returns the last element in the vector.
    /// @throws NoSuchElement if the vector is empty.
    auto peek_back() const -> ConstReference {
        if (self.empty()) {
            throw error::NoSuchElement("vector is empty.");
        }
        return self.back();
    }

    /// Returns the first element in the vector.
    /// @throws NoSuchElement if the vector is empty.
    auto peek_front() -> Reference {
        if (self.empty()) {
            throw error::NoSuchElement("vector is empty.");
        }
        return self.front();
    }

    /// Returns the first element in the vector.
    /// @throws NoSuchElement if the vector is empty.
    auto peek_front() const -> ConstReference {
        if (self.empty()) {
            throw error::NoSuchElement("vector is empty.");
        }
        return self.front();
    }

    /// Removes and returns the last item in the vector. Returns `std::nullopt` if vector is empty.
    auto pop_back() -> std::optional<T> {
        if (is_empty()) {
            return std::nullopt;
        }
        auto result = peek_back();
        self.pop_back();
        return result;
    }

    // Adds a new element at the end of the vector, after its current last element.
    // The content of val is copied to the new element.
    auto push_back(const T& val) -> void {
        self.push_back(val);
    }

    // Returns a direct pointer to the memory array used internally by the vector to store its owned elements.
    auto raw_ptr_begin() -> T* {
        return self.data();
    }

    // Returns a direct pointer to the memory array used internally by the vector to store its owned elements.
    auto raw_ptr_begin() const -> const T* {
        return self.data();
    }

    /// Assigns the contents from the iterator, given by `begin` and `end`, to the vector.
    /// The old contents of the vector are replaced and the size is modified accordingly.
    auto reassign(Iterator begin, Iterator end) -> void {
        self.assign(begin, end);
    }

    /// Assigns the contents of the `other` vector to the current vector. The old contents of the vector are replaced
    /// and the size is modified accordingly.
    auto reassign(const Vec<T>& other) -> void {
        self.assign(other.self);
    }

    /// Assigns the contents from initializer list `list` to the vector. The old contents of the vector are replaced
    /// and the size is modified accordingly.
    auto reassign(std::initializer_list<T> list) -> void {
        self.assign(list);
    }

    /// Removes the element at position `at` from the vector.
    auto remove(ConstIterator at) -> Iterator {
        return self.erase(at);
    }

    /// Removes the range [`begin`, `end`) from the vector.
    auto remove_range(ConstIterator begin, ConstIterator end) -> Iterator {
        return self.erase(begin, end);
    }

    /// Requests that the vector capacity be at least enough to contain `n` elements.
    auto request_cap(Size n) -> void {
        self.reserve(n);
    }

    /// Resizes the vector so that it contains `n` elements.
    auto resize(Size n) -> void {
        self.resize(n);
    }

    /// Resizes the vector so that it contains `n` elements. Each new element is a copy of `val`.
    auto resize(Size n, const T& val) -> void {
        self.resize(n, val);
    }

    /// Returns the size of the vector.
    auto size() const -> Size {
        return self.size();
    }

    /// Requests the vector to reduce its capacity to fit its size.
    auto shrink() -> void {
        self.shrink_to_fit();
    }

    /// Exchanges the content of the vector by the content of the `other` vector of the same type.
    /// Sizes may differ.
    auto swap(Vec& other) -> void {
        self.swap(other);
    }

    /// Applies a `Pattern` to the vector, modifying each element to satisfy the pattern.
    /// @note This method is only available to vectors of a numeric type (e.g. int, char).
    /// @see Pattern
    template <class U>
    requires (std::numeric_limits<U>::is_integer)
    auto with(const pattern::Pattern<U>& pat) -> Vec<T> {
        for (Size i = 1; i < self.size(); i++) {
            self[i] = pat(self[i - 1]);
        }
        return *this;
    }

    /// Send the contents of the vector as a string to std::ostream.
    friend auto operator<<(std::ostream& os, const Vec<T>& vec) -> std::ostream& {
        os << "[";
        for (auto iter = vec.cbegin(); iter != vec.cend(); iter++) {
            os << *iter;
            if (iter + 1 != vec.cend()) {
                os << ", ";
            }
        }
        os << "]";
        return os;
    }

    /// Access the `i`th element of the vector.
    /// @throws IndexOutOfBounds if attempting to access an element at an invalid index.
    auto operator[](Size i) -> Reference {
        if (i >= self.size()) {
            std::string message = "index too large for vector of size " + std::to_string(self.size()) + ".";
            throw error::IndexOutOfBounds(message.c_str());
        }
        return self.at(i);
    }

    /// Construct a default, empty vector.
    Vec() { self = Underlying(); }

    /// Constructs a vector with a copy of each of the elements in `list`, in the same order.
    Vec(std::initializer_list<T> list) { self = Underlying(list); }
};

#endif //TOOLS_VEC_H
