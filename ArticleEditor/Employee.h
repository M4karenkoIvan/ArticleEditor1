#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class Employee {
public:
  enum class Position : uint8_t {
    Admin,
    Designer,
    Editor,
    Journalist,
    Photographer
  };

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

  virtual Position getPosition() const = 0;

private:
  std::string firstName;
  std::string lastName;
  std::string department;
  std::string contactInfo;
};

inline std::string_view to_string(Employee::Position position) {
  switch (position) {
  case Employee::Position::Admin:
    return "Admin";
  case Employee::Position::Designer:
    return "Designer";
  case Employee::Position::Editor:
    return "Editor";
  case Employee::Position::Journalist:
    return "Journalist";
  case Employee::Position::Photographer:
    return "Photographer";
  default:
    return "Unknown";
  }
}

inline std::ostream& operator<<(std::ostream& out, Employee::Position position) {
  return out << to_string(position);
}
