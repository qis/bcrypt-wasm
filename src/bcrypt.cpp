#include <bcrypt/bcrypt.h>
#include <array>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <cassert>
#include <cstdint>

extern "C" {
#include "crypt_blowfish.h"

#ifdef __wasm__
char rc();
#endif

};

namespace bcrypt {
namespace {

constexpr inline auto min = std::numeric_limits<char>::min();
constexpr inline auto max = std::numeric_limits<char>::max();

constexpr inline auto part_info_size = 7;
constexpr inline auto part_salt_size = 22;
constexpr inline auto part_hash_size = 31;

constexpr inline auto salt_size = part_info_size + part_salt_size;
constexpr inline auto hash_size = part_info_size + part_salt_size + part_hash_size;

}  // namespace

std::string hash(const std::string& pass, unsigned long rounds) {
  // Generate random bytes.
#ifdef __wasm__
  std::array<char, salt_size + 1> rand;
  for (auto& c : rand) {
    c = rc();
  }
#else
  static thread_local std::mt19937 re(std::random_device{}());
  std::uniform_int_distribution<int> dist(min, max);
  std::array<char, salt_size + 1> rand;
  for (auto& c : rand) {
    c = static_cast<char>(dist(re));
  }
#endif

  // Generate salt.
  std::array<char, salt_size + 1> salt;
  _crypt_gensalt_blowfish_rn("$2b$", rounds, rand.data(), salt_size + 1, &salt[0], salt_size + 1);

  // Generate hash.
  std::string hash;
  hash.resize(hash_size + 1);
  [[maybe_unused]] const auto rv = _crypt_blowfish_rn(pass.data(), salt.data(), &hash[0], hash_size + 1);
  assert(rv);
  hash.resize(hash_size);
  return hash;
}

bool verify(const std::string& pass, const std::string& hash) noexcept {
  // Generate hash.
  std::array<char, hash_size + 1> test;
  if (!_crypt_blowfish_rn(pass.data(), hash.data(), &test[0], hash_size + 1)) {
    return false;
  }

  // Compare supplied hash to the generated hash.
  return hash == std::string_view(test.data(), hash_size);
}

}  // namespace bcrypt
