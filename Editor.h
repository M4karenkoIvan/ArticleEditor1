#pragma once

#include "Employee.h"

class Editor final : public Employee {
public:
  virtual ~Editor() = default;

  std::string_view getPosition() const override {
    return "editor";
  }
};
