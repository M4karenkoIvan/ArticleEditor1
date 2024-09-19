#pragma once

#include "Employee.h"

class Designer final : public Employee {
public:
  virtual ~Designer() = default;

  std::string_view getPosition() const override {
    return "designer";
  }
};
