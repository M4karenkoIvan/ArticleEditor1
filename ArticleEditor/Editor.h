#pragma once

#include "Employee.h"

class Editor final : public Employee {
public:
  Editor(std::string_view firstName, std::string_view lastName, std::string_view department)
    : Employee(firstName, lastName, department) {}

  virtual ~Editor() = default;

  Position getPosition() const override {
    return Position::Editor;
  }
};
