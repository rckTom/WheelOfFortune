#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <array>
#include <iterator>

template<class T, std::size_t S>
class circularBuffer {
 public:
    circularBuffer() : buffer_{{}} {
        this->head_ = this->buffer_.begin();  // after the end
        this->tail_ = this->buffer_.begin();  // first element
    }
    
    T& front() {
      return *this->tail_;
    }

    void pop_front() {
      if (this->size() > 0) {
        this->tail_++;
        if(this->tail_ == this->buffer_.end())
          this->tail_ = this->buffer_.begin();
      }
    }

    bool push_back(T element) {
      if (this->size() == S)
		this->pop_front();
	  
      *this->head_++ = element;
	  if (this->head_ == this->buffer_.end())
	    this->head_ = this->buffer_.begin();

      return false;
    }

    std::size_t size() {
      auto size = std::distance(this->tail_, this->head_);
      if (size < 0)
        size += this->buffer_.size();
      return size;
    }
    
 private:

    typedef std::array<T, S + 1> StorageClass;

    StorageClass buffer_;
    typename StorageClass::iterator head_;
    typename StorageClass::iterator tail_;

};

#endif  // CIRCULAR_BUFFER_H
