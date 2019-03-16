
#ifndef ADS_SET_H
#define ADS_SET_H

#include <functional>
#include <algorithm>
#include <iostream>
#include <stdexcept>
using namespace std;
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
        Node* next = nullptr;
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
    
    template<typename InputIt> ADS_set(InputIt first, InputIt last): ADS_set() {
        insert(first, last);
    }
    
    ADS_set(const ADS_set& other);
    
    ~ADS_set() {
        delete [] table;
    }
    
    ADS_set& operator=(const ADS_set& other);
    ADS_set& operator=(std::initializer_list<key_type> ilist);
    
    size_type size() const { return sz; }
    bool empty() const { return max_sz; }
    
    size_type count(const key_type& key) const {
        return !!find_pos(key);
    }
    
    iterator find(const key_type& key) const;
    
    void clear() {
        ADS_set temp;
        swap(temp);
    }
    
    void swap(ADS_set& other) {
        using std::swap;
        swap(sz, other.sz);
        swap(max_sz, other.max_sz);
        swap(max_lf, other.max_lf);
        swap(table, other.table);
    }
    
    void insert(std::initializer_list<key_type> ilist) {
        for(const auto& i: ilist) {
            if(!count(i)) insert_unchecked(i);
        }
    }
    
    std::pair<iterator,bool> insert(const key_type& key);
    template<typename InputIt> void insert(InputIt first, InputIt last) {
        for(auto it = first; it != last; it++) {
            if(find_pos(*it) != nullptr) insert_unchecked(*it);
        }
    }
    
    size_type erase(const key_type& key);
    
    const_iterator begin() const {
        for(size_t i{0}; i < max_sz; ++i){
            if(table[i].next)
                return const_iterator(this, table + i, table[i].next);
        }
        return end();
    }
    
    const_iterator end() const {
        return const_iterator(nullptr);
    }
    
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
private:
    const ADS_set* set{nullptr};
    const Node* ptr{nullptr};
    Node* p = nullptr;
public:
    explicit Iterator(const ADS_set* set = nullptr, const Node* ptr = nullptr, Node* p=nullptr): set(set), ptr(ptr), p(p) {
    }
    reference operator*() const { return p -> key; }
    pointer operator->() const { return &p -> key; }
    
    Iterator& operator++() {
        if(p -> next != nullptr){
            p = p -> next;
            return *this;
        }
        while(++ptr != &set -> table[set -> max_sz]){
            p = ptr -> next;
            if(p){
                return *this;
            }
        }
        p = nullptr;
        ptr = nullptr;
        return *this;
        
        
    }
    
    Iterator operator++(int) {
        Iterator temp = *this;
        operator++();
        return temp;
    }
    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
        return lhs.p == rhs.p;
    }
    friend bool operator!=(const Iterator& lhs, const Iterator& rhs){
        return lhs.p != rhs.p;
    }
};

template <typename Key, size_t N> void swap(ADS_set<Key,N>& lhs, ADS_set<Key,N>& rhs) { lhs.swap(rhs); }

#endif // ADS_SET_H
