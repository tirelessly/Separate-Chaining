
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
    
private:
    struct Node {
        Key key;
        Node* next{nullptr};
        ~Node() {
            delete next;
        }
    };
    
    Node* table{nullptr};
    size_type sz{0}, max_sz{N};
    float max_lf{0.7};
    
    size_type hash_idx(const_reference key) const {
        return hasher{}(key) % max_sz;
    }
    
    Node* insert_unchecked(const_reference key) {
        Node* help = new Node;
        size_type index = hash_idx(key);
        help -> next = table[index].next;
        help -> key = key;
        table[index].next = help;
        ++sz;
        help = nullptr;
        return table[index].next;
    }
    
    Node* find_pos(const_reference key) const {
        size_type index = hash_idx(key);
        for(Node* i = &table[index]; i != nullptr; i = i -> next) {
            if(key_equal{}(key, i -> key))
                return i;
        }
        return nullptr;
    }
    
public:
    ADS_set() {
        table = new Node[max_sz];
    }
    ADS_set(std::initializer_list<key_type> ilist): ADS_set() {
        insert(ilist);
    }
    template<typename InputIt> ADS_set(InputIt first, InputIt last);
    ADS_set(const ADS_set& other);
    ~ADS_set() {
        delete [] table;
    }
    
    ADS_set& operator=(const ADS_set& other);
    ADS_set& operator=(std::initializer_list<key_type> ilist);
    
    size_type size() const;
    bool empty() const;
    
    size_type count(const key_type& key) const {
        return !!find_pos(key);
    }
    
    iterator find(const key_type& key) const;
    
    void clear();
    void swap(ADS_set& other);
    
    void insert(std::initializer_list<key_type> ilist) {
        for(const auto& i: ilist) {
            if(!count(i)) insert_unchecked(i);
        }
    }
    
    std::pair<iterator,bool> insert(const key_type& key);
    template<typename InputIt> void insert(InputIt first, InputIt last);
    
    size_type erase(const key_type& key);
    
    const_iterator begin() const;
    const_iterator end() const;
    
    void dump(std::ostream& o = std::cerr) const {
        o << "max_sz = " << max_sz << '\n';
        o << "sz = " << sz << '\n';
        for(size_type i{0}; i < max_sz; ++i){
            o << i << ": ";
            for(Node* j = table[i].next; j != nullptr; j = j -> next) {
                o << j -> key;
                o << " -> ";

            }
            o << '\n';
        }
    }
    
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
