#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class Document {
public:
  enum class Type : uint8_t {
    Article,
    MagazineLayout,
    PhotoReport
  };

  virtual Type getType() const = 0;

private:
};
