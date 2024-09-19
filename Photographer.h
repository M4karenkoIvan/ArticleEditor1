#pragma once

#include "Employee.h"

class Photographer final : public Employee {
public:
  virtual ~Photographer() = default;

  std::string_view getPosition() const override {
    return "photographer";
  }
};
