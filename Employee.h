#pragma once

#include <string>
#include <string_view>

class Employee {
public:
  Employee(std::string_view firstName, std::string_view lastName, std::string_view department)
      : firstName(firstName),
        lastName(lastName),
        department(department) {}

  virtual ~Employee() = default;

  std::string_view getFirstName() const {
    return firstName;
  }

  std::string_view getLastName() const {
    return lastName;
  }

  std::string_view getDepartment() const {
    return department;
  }

  void setContactInfo(std::string_view newContactInfo) {
    contactInfo = newContactInfo;
  }

  std::string_view getContactInfo() const {
    return contactInfo;
  }

  virtual std::string_view getPosition() const = 0;

private:
  std::string firstName;
  std::string lastName;
  std::string department;
  std::string contactInfo;
};
