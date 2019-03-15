
#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename Key, size_t N = 7>
class ADS_set {
public:
    class Iterator;
    using value_type = Key;
    using key_type = Key;
    using reference = key_type&;
    using const_reference = const key_type&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = Iterator;
    using const_iterator = Iterator;
    using key_compare = std::less<key_type>;   // B+-Tree
    using key_equal = std::equal_to<key_type>; // Hashing
    using hasher = std::hash<key_type>;        // Hashing
    
    ADS_set();
    ADS_set(std::initializer_list<key_type> ilist);
    template<typename InputIt> ADS_set(InputIt first, InputIt last);
    ADS_set(const ADS_set& other);
    ~ADS_set();
    
    ADS_set& operator=(const ADS_set& other);
    ADS_set& operator=(std::initializer_list<key_type> ilist);
    
    size_type size() const;
    bool empty() const;
    
    size_type count(const key_type& key) const;
    iterator find(const key_type& key) const;
    
    void clear();
    void swap(ADS_set& other);
    
    void insert(std::initializer_list<key_type> ilist);
    std::pair<iterator,bool> insert(const key_type& key);
    template<typename InputIt> void insert(InputIt first, InputIt last);
    
    size_type erase(const key_type& key);
    
    const_iterator begin() const;
    const_iterator end() const;
    
    void dump(std::ostream& o = std::cerr) const;
    
    friend bool operator==(const ADS_set& lhs, const ADS_set& rhs);
    friend bool operator!=(const ADS_set& lhs, const ADS_set& rhs);
};

template <typename Key, size_t N>
class ADS_set<Key,N>::Iterator {
public:
    using value_type = Key;
    using difference_type = std::ptrdiff_t;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::forward_iterator_tag;
    
    explicit Iterator(/* implementation-dependent */);
    reference operator*() const;
    pointer operator->() const;
    Iterator& operator++();
    Iterator operator++(int);
    friend bool operator==(const Iterator& lhs, const Iterator& rhs);
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs);
};

template <typename Key, size_t N> void swap(ADS_set<Key,N>& lhs, ADS_set<Key,N>& rhs) { lhs.swap(rhs); }

#endif // ADS_SET_H
