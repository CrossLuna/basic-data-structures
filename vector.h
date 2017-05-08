#pragma once

#include <cstddef>

#include <algorithm>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>

namespace clstd {

template <typename T>
class vector {
public:
    typedef T                              value_type;
    //allocator_type
    typedef std::size_t                    size_type;
    typedef std::ptrdiff_t                 difference_type;
    typedef value_type&                    reference;
    typedef const value_type&              const_reference;
    typedef value_type*                             pointer;
    typedef const value_type*                       const_pointer;
    typedef value_type*                             iterator;
    typedef const value_type*                       const_iterator;
    //reverse_iterator
    //const_reverse_iterator

    
    /* Constructors */
    vector() = default;
    explicit vector(size_type n): 
    contents_(new T[n]),
    size_(n), 
    capacity_(n) {}
    
    vector(size_type n, value_type value):
    contents_(new T[n]),
    size_(n), 
    capacity_(n) {
        std::fill_n(contents_.get(), n, value);
    }

    /* Copy Constructor*/
    vector(const vector& other): 
    contents_(copy_array(other.contents_, other.size_, other.size_)),
    size_(other.size_),
    capacity_(other.capacity_) {}

    /* Assign Constructor */
    vector& operator=(const vector& other) {
        this->contents_ = copy_array(other.contents_, other.size_, other.size_);
        this->size_ = other.size_;
        this->capacity_ = other.capacity_;
        return *this;
    }

    /* Move Contructor */
    vector(vector&& other):
    contents_(std::move(other.contents_)),
    size_(other.size_),
    capacity_(other.capacity_) {}

    /* Move Assign Constructor */
    vector& operator=(vector&& other) {
        std::swap(contents_, other.contents_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    ~vector() = default;

    iterator               begin()  {return contents_.get();}
    const_iterator         begin() const {return contents_.get();}
    iterator               end() {return contents_.get() + size_;}
    const_iterator         end() const {return contents_.get() + size_;}

    //reverse_iterator       rbegin() noexcept;
    //const_reverse_iterator rbegin()  const noexcept;
    //reverse_iterator       rend() noexcept;
    //const_reverse_iterator rend()    const noexcept;

    const_iterator         cbegin()  const {return contents_.get();};
    const_iterator         cend()    const {return contents_.get() + size_;};
    //const_reverse_iterator crbegin() const noexcept;
    //const_reverse_iterator crend()   const noexcept;


    size_type size() const {return size_;}
    size_type max_size() const {return std::numeric_limits<difference_type>::max();}
    size_type capacity() const {return capacity_;}
    bool empty() const {return size_ == 0;}
    void reserve(size_type n);
    void shrink_to_fit() noexcept;

    reference       operator[] (size_type i) {return contents_[i];}
    const_reference operator[] (size_type i) const {return contents_[i];}
    reference       at(size_type n) {if(n >= size_) throw std::out_of_range("Out of range"); return contents_[n];}
    const_reference at(size_type n) const {if(n >= size_) throw std::out_of_range("Out of range"); return contents_[n];}

    reference       front() {return *begin();}
    const_reference front() const {return *cbegin();}
    reference       back() {return *(end()-1);}
    const_reference back() const {return *(cend()-1);}

    value_type*       data() {return contents_.get();}
    const value_type* data() const {return contents_.get();}

    void push_back(const value_type& x);
    template <class... Args> reference emplace_back(Args&&... args);  // C++14; reference in C++17
    void pop_back();

    template <class... Args> iterator emplace(const_iterator position, Args&&... args);  // C++14
    iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, size_type n, const value_type& x);
    template <class InputIterator>
        iterator insert(const_iterator position, InputIterator first, InputIterator last);
    //iterator insert(const_iterator position, initializer_list<value_type> il);

    iterator erase(const_iterator position);
    iterator erase(const_iterator first, const_iterator last);

    void clear() noexcept;

    void resize(size_type sz);
    void resize(size_type sz, value_type x);

    //void swap(vector&)
    //    noexcept(allocator_traits<allocator_type>::propagate_on_container_swap::value ||
    //             allocator_traits<allocator_type>::is_always_equal::value);  // C++17
    //void flip() noexcept;

    //bool __invariants() const;

    
private:
    std::unique_ptr<T[]> copy_array(const std::unique_ptr<T[]>& orig, std::size_t size, std::size_t orig_size) {
        std::unique_ptr<T[]> copy {new T[size]};
        for(std::size_t i = 0; i < orig_size; ++i) 
            copy[i] = orig[i];
        return copy;
    }
    std::unique_ptr<T[]> contents_ {new T[DEFAULT_MAX_CAP]};
    std::size_t size_ {0u};
    std::size_t capacity_ {DEFAULT_MAX_CAP};
    static const std::size_t DEFAULT_MAX_CAP {0u};
};

template <typename T>
void vector<T>::push_back(const vector<T>::value_type& x) {
    if(size_ == capacity_) { // expand
        capacity_ = std::max(size_ << 1, size_t{1});
        contents_ = copy_array(contents_, capacity_, size_);
    }
    contents_[size_] = x;
    ++size_;
}

template <typename T>
void vector<T>::pop_back() {
    --size_;
    if((size_ << 2) <= capacity_) { 
        capacity_ >>= 1;
    }
    contents_ = copy_array(contents_, capacity_, size_);
}

template <typename T>
void vector<T>::shrink_to_fit() noexcept {
    capacity_ = size_;
    contents_ = copy_array(contents_, capacity_, size_);
}

} //namespace clstd