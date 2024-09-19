#pragma once

#include "Employee.h"
#include "PhotoReport.h"

#include <windows.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

class Photographer final : public Employee {
public:
  Photographer() : Employee("DefaultFirstName", "DefaultLastName", "DefaultDepartment") {}
  Photographer(std::string_view firstName, std::string_view lastName, std::string_view department)
      : Employee(firstName, lastName, department) {}

  virtual ~Photographer() = default;

  void setPhoto() {
    std::string caption;
    std::cout << "Input path of the photo: ";
    PhotoReport photoReport;
    OPENFILENAMEA ofn = {0};
    char Buffer[300];
    std::fill(Buffer, Buffer + 300, '\0');
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetForegroundWindow();
    ofn.lpstrFile = Buffer;
    ofn.nMaxFile = 300;
    ofn.Flags = OFN_EXPLORER;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    if (GetOpenFileNameA(&ofn)) {
      std::cout << ofn.lpstrFile << std::endl;
      }
    std::string photo(ofn.lpstrFile);
    std::cout << "Input the caption:\n";
    std::cin >> caption;
    photoReport.setCaption(caption);
    photoReport.setPhoto(photo);
    photos.push_back(photoReport);
  }

  void viewPhotos() {
    std::cout << "Current photos: " << photos.size() << '\n';
    for (const auto& photoReport : photos) {
      std::cout << photoReport.getPhoto() << '\n';
    }
  }

  Position getPosition() const override {
    return Position::Photographer;
  }

private:
  std::vector<PhotoReport> photos;
};
