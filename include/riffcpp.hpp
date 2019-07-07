#ifndef RIFFCPP_H
#define RIFFCPP_H

#include <array>
#include <cstdint>
#include <istream>
#include <iterator>
#include <memory>
#include <stddef.h>
#include <vector>

namespace riffcpp {
  /// Represents a FourCC
  /**
    This is a sequence of four bytes used to identify the various types of RIFF
    chunks
  */
  using FourCC = std::array<char, 4>;

  /// The `RIFF` FourCC, used to identify toplevel chunks
  constexpr FourCC riff_id = {'R', 'I', 'F', 'F'};

  /// The `LIST` FourCC, used to identify chunks that contain other chunks
  constexpr FourCC list_id = {'L', 'I', 'S', 'T'};

  /// Represents a RIFF chunk
  /**
    Every chunk has a four byte identifier (FourCC) and some contents.
    Depending on the value of the identifier, the chunk may contain other chunks
    as its contents, and in those cases a second FourCC is used to distinguish
    the chunk type.
  */
  class Chunk {
  public:
    class iterator;

  private:
    class impl;

    std::unique_ptr<impl> pimpl;
    friend class iterator;

    Chunk(std::unique_ptr<impl> &&impl);

  public:
    // Loads a chunk from a file
    /**
      Opens the specified file in read-only mode and
      reads it as a chunk
     */
    Chunk(const char *filename);

    // Loads the specified buffer as a chunk
    Chunk(const void *buffer, std::size_t length);

    Chunk(const riffcpp::Chunk &ch);

    Chunk &operator=(const Chunk &lhs);

    ~Chunk();

    /// The chunk's identifier
    FourCC id();

    /// If the chunk contains other chunks, this is its type FourCC
    FourCC type();

    /// Returns the size of the chunk's contents in bytes
    std::uint32_t size();

    /// Provides a way to iterate over subchunks
    class iterator {
      class impl;
      std::unique_ptr<riffcpp::Chunk::iterator::impl> pimpl;

      iterator(std::unique_ptr<riffcpp::Chunk::iterator::impl> &&impl);

      friend class Chunk;

    public:
      iterator(const iterator &it);
      using value_type = riffcpp::Chunk;
      using reference = value_type &;
      using pointer = value_type *;
      using difference_type = std::ptrdiff_t;
      using iterator_category = std::input_iterator_tag;

      /// Returns whether two iterators point to the same chunk
      bool operator==(const iterator &a) const;
      /// Returns whether two iterators do not point to the same chunk
      bool operator!=(const iterator &a) const;

      /// Returns the chunk pointed by the iterator
      Chunk operator*() const;

      /// Moves the iterator ahead, to point to the following iterator
      iterator &operator++();

      /// Moves the iterator ahead, to point to the following iterator and
      /// returns an iterator to the current position
      iterator operator++(int);

      ~iterator();
    };

    /// If this chunk contains other chunks, returns an iterator to the first
    /// chunk contained
    /**
      `no_chunk_id` is used for chunks which have no chunk id but still contain
      subchunks, like `seqt` from DirectMusic
    */
    iterator begin(bool no_chunk_id = false);

    ///  If this chunk contains other chunks, returns an iterator pointing past
    ///  the
    /// last chunk contained
    iterator end();

    /// Returns the raw contents of the chunk
    std::vector<char> data();

    using value_type = Chunk;
    using reference = value_type &;
    using const_reference = const Chunk &;
    using pointer = value_type *;
    using const_pointer = const pointer;
    using const_iterator = const iterator;
  };
} // namespace riffcpp
#endif // RIFFCPP_H