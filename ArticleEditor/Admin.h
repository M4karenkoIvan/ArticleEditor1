#pragma once

#include "Employee.h"

class Admin final : public Employee {
public:
  Admin(std::string_view firstName, std::string_view lastName, std::string_view department)
      : Employee(firstName, lastName, department) {}

  virtual ~Admin() = default;

  Position getPosition() const override {
    return Position::Admin;
  }
};
