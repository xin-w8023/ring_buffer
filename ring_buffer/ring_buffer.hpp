//
// Created by Wangxin.Colin on 2021/6/18.
//

#ifndef RING_BUFFER_RING_BUFFER_RING_BUFFER_HPP_
#define RING_BUFFER_RING_BUFFER_RING_BUFFER_HPP_
#include <vector>
#include <mutex>

template<typename T, typename container = std::vector<T> >
class RingBuffer {
 public:
  RingBuffer() = default;

  explicit RingBuffer(size_t capacity)
      : capacity_(capacity), available_(capacity) {
    buffer.resize(capacity_);
  }

  bool empty() const { return available_ == capacity_; }
  bool full() const { return full_; }

  void write(T obj);
  void write(T *obj_ptr, size_t size);

  T read();
  void *read(size_t size, T *dst);
  container read(size_t size);

  size_t available() {
    return available_;
  }
  void show();

 private:

  size_t handle_ptr_(size_t ptr);

 private:
  container buffer{};
  size_t capacity_{0};
  size_t read_ptr_{0};
  size_t write_ptr_{0};
  size_t available_{0};
  bool full_{false};

  std::mutex read_mutex_;
  std::mutex read_one_mutex_;
  std::mutex write_mutex_;
  std::mutex write_one_mutex_;

};

#include "ring_buffer_imp.hpp"
#endif //RING_BUFFER_RING_BUFFER_RING_BUFFER_HPP_
