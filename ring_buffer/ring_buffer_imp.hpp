//
/// COPYRIGHT NOTICE
/// Copyright (c) 2021, individual.
/// All rights reserved.
///
/// @file ring_buffer_imp.hpp
/// @brief
/// 
/// @detail
///
/// @version
/// @author xin wang
/// @email colin8023.wang@gmail.com
/// @date 2021/6/19
//

template<typename T, typename container>
void RingBuffer<T, container>::write(T obj) {
  std::lock_guard<std::mutex> guard(write_one_mutex_);
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
  std::lock_guard<std::mutex> guard(write_mutex_);
  if (size <= available_) {
    int idx = 0;
    while (idx < size) {
      write(obj_ptr[idx++]);
    }
  } else {
    std::cout << "\nBuffer has no more capacity."
                 "\nRequest size " + std::to_string(size)
        + ", but only available " + std::to_string(available_) << std::endl;
    return;
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

  std::lock_guard<std::mutex> guard(read_one_mutex_);
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
  std::lock_guard<std::mutex> guard(read_mutex_);
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
  std::lock_guard<std::mutex> guard(read_mutex_);
  size_t left = capacity_ - available_;
  if (size <= left) {
    container dst(size);
    size_t idx = 0;
    while (idx < size) {
      dst[idx++] = read();
    }
    return dst;
  } else {

    std::cout <<
              "\nRequest " + std::to_string(size) + " objects, "
                  + "but only left " + std::to_string(left) + " objects.\n";
    return {};
    throw std::invalid_argument(
        "\nRequest " + std::to_string(size) + " objects, "
            + "but only left " + std::to_string(left) + " objects.");
  }
}
