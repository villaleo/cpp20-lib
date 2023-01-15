# `Vec` class
`Vec` is a wrapper over `std::vector` with additional functionality. All the methods from the `std::vector` class are
available, but some have different (more appropriate) names.

## New method behavior
* Calling `peek_back` on an empty vector will throw a `NoSuchElement` exception.
* Calling `peek_front` on an empty vector will throw a `NoSuchElement` exception.
* Calling `at` on an empty vector will throw an `IndexOutOfBounds` exception.
* `pop_back` now returns the item that was removed, wrapped in `std::optional`.

## Renamed methods
* `assign` renamed to `reassign`.
* `back` renamed to `peek_back`.
* `capacity` renamed to `cap`.
* `empty` renamed to `is_empty`.
* `erase` renamed to `remove`.
* `front` renamed to `peek_front`.
* `reserve` renamed to `request_cap`.
* `shrink_to_fit` renamed to `shrink`.

### Overloaded methods
* `insert(const_iterator, size_type, const T&)` renamed to `fill`.
* `insert(const_iterator, InputIterator, InputIterator)` renamed to `insert_range`.
* `insert(const_iterator, initializer_list<T>)` renamed to `insert_list`.
* `erase(const_iterator, const_iterator)` renamed to `remove_range`.

## Additions
### Class construction uses a Rust-like syntax
Introduces a cleaner way to instantiate a `Vec<T>` with the `of` and `from` methods.

```c++
// Creates a vector of size 5, where each element is 1.
auto fives = Vec<int>::of(10, 1);
// [1, 1, 1, 1, 1]
std::cout << fives << '\n';

// Creates a vector of characters with values [a, b, c].
auto abc = Vec<char>::from({'a', 'b', 'c'});

// Create a vector from an array
auto arr = std::array<int, 5> { 1, 2, 3, 4, 5};
auto vec = Vec<int>::from(arr.begin(), arr.end());
```

In addition, there is a `Pattern` type that can be used by the `with` method to apply a pattern to a sequence.
Non-integer types cannot invoke `with`. The patterns available are `pattern::Incr<T>`, `pattern::Decr<T>`, and
`pattern::Mult<T>`.

```c++
// Creates a vector [1, 2, 3, 4].
auto arithmetic = Vec<int>::of(4, 1).with(pattern::Incr<int>);

// Creates a vector [1, 5, 25, 125].
auto geometric = Vec<int>::of(4, 1).with(pattern::Mult<int, 5>);
```
Default `Vec<T>()` and list-initialized `Vec<T> { foo, bar }` constructors remain available.
```c++
// Default constructor.
auto vec = Vec<int>();

// Initializer-list constructor.
auto from_list = Vec<char> { 'x', 'y', 'z' };
```

### Exceptions
Two new exception classes were created to handle situations where it made more sense to crash (i.e. to prevent
undefined behavior).
* `NoSuchElement`: thrown when attempting to remove elements from an empty vector. 
* `IndexOutOfBounds`: thrown when attempting to access an element at an invalid location.

## Not implemented
**Method overloads that had move `&&` parameter(s) were not implemented.**
* `get_allocator()` is not a member of `Vec` since many of the methods in `std::allocator` are deprecated.

# Coming soon
* A `map` method to apply a function to a vector
* A `reduce` method to filter elements in a vector
* A `Stream<T>` class, used as a proxy when applying transformations and mappings to a collection 