#include <iostream>
#include <thread>
#include <chrono>
#include "ring_buffer/ring_buffer.hpp"

template<typename T>
void consumer(RingBuffer<T>& ring_buffer, size_t size) {
  while (true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "In consumer " << std::this_thread::get_id() << std::endl;
    auto read = ring_buffer.read(size);
    for (auto &e : read) {
      std::cout << e << " ";
    }
    std::cout << std::endl;
    // ring_buffer.show();
  }
}

template<typename T>
void producer(RingBuffer<float>& ring_buffer, size_t size, T* src) {
  while(true) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ring_buffer.write(src, size);
  }
}

int main() {

  std::cout << "Hello, World!" << std::endl;

  RingBuffer<float> ring_buffer(10);

  auto to_write = new float[5]{1, 2, 3, 4, 5};

//  ring_buffer.write(to_write, 5);
//  ring_buffer.show();
//
//  ring_buffer.write(to_write, 5);
//  ring_buffer.show();
//
//  auto read_objs = ring_buffer.read(10);
//  ring_buffer.show();
//
//  ring_buffer.write(to_write, 5);
//  ring_buffer.show();

  // test multi-threads
  std::thread producer_thread(producer<float>, std::ref(ring_buffer), 5lu, to_write);
  std::thread consumer_thread1(consumer<float>, std::ref(ring_buffer), 10);
  std::thread consumer_thread2(consumer<float>, std::ref(ring_buffer), 10);
  producer_thread.join();
  consumer_thread1.join();
  consumer_thread2.join();

  delete[] to_write;
  return 0;
}
