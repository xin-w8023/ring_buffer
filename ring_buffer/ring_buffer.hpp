//
// Created by Wangxin.Colin on 2021/6/18.
//

#ifndef RING_BUFFER_RING_BUFFER_RING_BUFFER_HPP_
#define RING_BUFFER_RING_BUFFER_RING_BUFFER_HPP_
#include <vector>

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
};

template<typename T, typename container>
void RingBuffer<T, container>::write(T obj) {
  if (available_ > 0) {
    buffer[write_ptr_] = obj;
    write_ptr_ = handle_ptr_(write_ptr_);
    --available_;
  } else {
    throw std::out_of_range("Buffer has no more capacity.\n");
  }
  full_ = available_ == 0;
}

template<typename T, typename container>
size_t RingBuffer<T, container>::handle_ptr_(size_t ptr) {
  ptr += 1;
  return ptr == capacity_ ? 0 : ptr;
}

template<typename T, typename container>
void RingBuffer<T, container>::write(T *obj_ptr, size_t size) {
  if (size <= available_) {
    int idx = 0;
    while (idx < size) {
      write(obj_ptr[idx++]);
    }
  } else {
    throw std::out_of_range(
        "\nBuffer has no more capacity."
        "\nRequest size " + std::to_string(size)
            + ", but only available " + std::to_string(available_));
  }

}

template<typename T, typename container>
void RingBuffer<T, container>::show() {

  std::cout << "Information of buffer" << std::endl;
  std::cout << "  Capacity: " << capacity_ << std::endl;
  std::cout << "  Available: " << available_ << std::endl;
  std::cout << "  Occupied: " << capacity_ - available_ << std::endl;
  std::cout << "  Values: ";

  if (empty()) {
    std::cout << std::endl << std::endl;
    return;
  }

  auto ptr = read_ptr_;
  if (full_) {
    std::cout << buffer[ptr] << " ";
    ptr = handle_ptr_(ptr);
  }
  while (ptr != write_ptr_) {
    std::cout << buffer[ptr] << " ";
    ptr = handle_ptr_(ptr);
  }
  std::cout << std::endl << std::endl;
}

template<typename T, typename container>
T RingBuffer<T, container>::read() {

  if (available_ <= capacity_) {
    T ret = buffer[read_ptr_];
    read_ptr_ = handle_ptr_(read_ptr_);
    available_ = available_ == capacity_ ? capacity_ : available_ + 1;
    return ret;
  } else {
    throw std::out_of_range("There are no more objects in buffer.\n");
  }
}

template<typename T, typename container>
void *RingBuffer<T, container>::read(size_t size, T *dst) {
  size_t left = capacity_ - available_;
  if (size <= left) {
    size_t idx = 0;
    while (idx < size) {
      dst[idx++] = read();
    }
  } else {
    throw std::invalid_argument(
        "\nRequest " + std::to_string(size) + " objects, "
            + "but only left " + std::to_string(left) + " objects.");
  }
}

template<typename T, typename container>
container RingBuffer<T, container>::read(size_t size) {
  size_t left = capacity_ - available_;
  if (size <= left) {
    container dst(left);
    size_t idx = 0;
    while (idx < size) {
      dst[idx++] = read();
    }
    return dst;
  } else {
    throw std::invalid_argument(
        "\nRequest " + std::to_string(size) + " objects, "
            + "but only left " + std::to_string(left) + " objects.");
  }
}

#endif //RING_BUFFER_RING_BUFFER_RING_BUFFER_HPP_
