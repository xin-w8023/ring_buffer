#include <iostream>
#include "ring_buffer/ring_buffer.hpp"

int main() {

  std::cout << "Hello, World!" << std::endl;

  RingBuffer<float> ring_buffer(10);

  auto to_write = new float[5]{1, 2, 3, 4, 5};

  ring_buffer.write(to_write, 5);
  ring_buffer.show();

  ring_buffer.write(to_write, 5);
  ring_buffer.show();

  auto read_objs = ring_buffer.read(10);
  ring_buffer.show();

  ring_buffer.write(to_write, 5);
  ring_buffer.show();

  delete[] to_write;
  return 0;
}
