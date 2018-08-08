#pragma once
#include "core/byte_array/byte_array.hpp"
#include "crypto/stream_hasher.hpp"

namespace fetch {
namespace crypto {

class FNV : public StreamHasher
{
public:
  using byte_array_type = typename StreamHasher::byte_array_type;

  FNV() { digest_.Resize(4); }

  void Reset() override { context_ = 2166136261; }

  bool Update(byte_array_type const &s) override
  {
    for (std::size_t i = 0; i < s.size(); ++i)
    {
      context_ = (context_ * 16777619) ^ s[i];
    }
    return true;
  }

  void Final() override
  {
    digest_[0] = uint8_t(context_);
    digest_[1] = uint8_t(context_ >> 8);
    digest_[2] = uint8_t(context_ >> 16);
    digest_[3] = uint8_t(context_ >> 24);
  }

  byte_array_type digest() override
  {
    assert(digest_.size() == 4);
    return digest_;
  }

  uint32_t uint_digest() { return context_; }

private:
  uint32_t        context_;
  byte_array_type digest_;
};

struct CallableFNV
{
  std::size_t operator()(fetch::byte_array::ConstByteArray const &key) const
  {
    uint32_t hash = 2166136261;
    for (std::size_t i = 0; i < key.size(); ++i)
    {
      hash = (hash * 16777619) ^ key[i];
    }

    return hash;
  }
};
}  // namespace crypto
}  // namespace fetch
