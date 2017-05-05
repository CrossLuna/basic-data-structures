#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <limits>
#include <algorithm>

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
    contents_(copy_array(other.contents_, other.size_)),
    size_(other.size_),
    capacity_(other.capacity_) {}

    /* Assign Constructor */
    //vector& operator=(const vector& other) {
    //    this.contents_ = 
    //}


    ~vector() = default;

    iterator               begin()  {return contents_.get();}
    const_iterator         begin() const {return contents_.get();}
    iterator               end() {return contents_.get() + size_;}
    const_iterator         end() const {return contents_.get() + size_;}

    //reverse_iterator       rbegin() noexcept;
    //const_reverse_iterator rbegin()  const noexcept;
    //reverse_iterator       rend() noexcept;
    //const_reverse_iterator rend()    const noexcept;

    const_iterator         cbegin()  const noexcept;
    const_iterator         cend()    const noexcept;
    //const_reverse_iterator crbegin() const noexcept;
    //const_reverse_iterator crend()   const noexcept;


    size_type size() const {return size_;}
    size_type capacity() const {return capacity_;}
    bool empty() const {return size_ == 0;}
    size_type max_size() const {return std::numeric_limits<difference_type>::max();}

    reference       operator[] (size_type i) {return contents_[i];}
    const_reference operator[] (size_type i) const {return contents_[i];}
    reference       at(size_type n);
    const_reference at(size_type n) const;

    reference       front();
    const_reference front() const;
    reference       back();
    const_reference back() const;


    value_type*       data() {return contents_.get();}
    const value_type* data() const {return contents_.get();}

    
private:
    std::unique_ptr<T[]> copy_array(const std::unique_ptr<T[]>& orig, std::size_t size) {
        std::unique_ptr<T[]> copy {new T[size]};
        for(std::size_t i = 0; i < size; ++i) 
            copy[i] = orig[i];
        return copy;
    }
    std::unique_ptr<T[]> contents_ {new T[DEFAULT_MAX_CAP]};
    std::size_t size_ {0u};
    std::size_t capacity_ {DEFAULT_MAX_CAP};
    static const std::size_t DEFAULT_MAX_CAP {0u};
};
} //namespace clstd