#pragma once

#include "Employee.h"
#include "Journalist.h"
#include "Photographer.h"

#include "MagazineLayout.h"
class Designer final : public Employee {
public:
  Designer(std::string_view firstName, std::string_view lastName, std::string_view department)
      : Employee(firstName, lastName, department) {}

  virtual ~Designer() = default;
  Journalist journalist1;
  Photographer photograph1;

  void viewSourceFiles() {
    journalist1.viewArticles();
    photograph1.viewPhotos();
  }

  void setLayout() {
    std::string typeLayout;
    std::cout << "Input path of the layout: ";
    MagazineLayout magazineLayout;
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
    std::string layout(ofn.lpstrFile);
    std::cout << "Input the type of the layout:\n";
    std::cin >> typeLayout;
    magazineLayout.setLayout(layout);
    magazineLayout.setTypeLayout(typeLayout);
    Layouts.push_back(magazineLayout);
  }

  void viewLayouts() {
    std::cout << "Current layouts: " << Layouts.size() << '\n';
    for (const auto& MagazineLayoute : Layouts) {
      std::cout << MagazineLayoute.getLayout() << '\n';
    }
  }

  Position getPosition() const override {
    return Position::Designer;
  }

private:
  std::vector<MagazineLayout> Layouts;
};
