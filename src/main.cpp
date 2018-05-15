#include <bcrypt/bcrypt.h>
#include <string>
#include <cstdio>

int main() {
  using namespace std::string_literals;
  const auto pass = "test"s;
  const auto hash = bcrypt::hash(pass);
  std::printf("%s: %s (%s)\n", pass.data(), hash.data(), bcrypt::verify(pass, hash) ? "ok" : "mismatch");
}
