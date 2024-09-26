#pragma once

#include <array>

namespace pxd {
template <typename T, size_t N> class RingBuffer {
public:
  RingBuffer() = default;
  RingBuffer(const RingBuffer &other) = default;
  RingBuffer &operator=(const RingBuffer &other) = default;
  RingBuffer(RingBuffer &&other) = default;
  RingBuffer &operator=(RingBuffer &&other) = default;
  ~RingBuffer() = default;

  void add(const T &value) {
    size_t inbound_write_index = (write_index + 1) % N;

    if (inbound_write_index == read_index) {
      return;
    }

    buffer[write_index] = value;
    write_index = inbound_write_index;
  }

  auto get() -> T {
    if (read_index == write_index) {
      return {};
    }

    size_t inbound_read_index = (read_index + 1) % N;
    size_t old_read_index = read_index;
    read_index = inbound_read_index;

    return buffer[old_read_index];
  }

private:
  std::array<T, N> buffer;
  size_t write_index = 0;
  size_t read_index = 0;
};
} // namespace pxd