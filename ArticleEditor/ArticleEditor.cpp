#include "Admin.h"
#include "Designer.h"
#include "Editor.h"
#include "Employee.h"
#include "Journalist.h"
#include "Photographer.h"

#include <windows.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

using namespace std;

namespace {

constexpr auto KEY_UP = 38;
constexpr auto KEY_DOWN = 0x28;
constexpr auto KEY_RETURN = 0x0D;
int id = 1;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

Employee* createEmployee(Employee::Position pos, std::string_view firstName, std::string_view lastName,
                         std::string_view department = "department") {
  static std::vector<std::unique_ptr<Employee>> employees;

  switch (pos) {
  case Employee::Position::Admin:
    employees.push_back(std::make_unique<Admin>(firstName, lastName, department));
    break;
  case Employee::Position::Designer:
    employees.push_back(std::make_unique<Designer>(firstName, lastName, department));
    break;
  case Employee::Position::Editor:
    employees.push_back(std::make_unique<Editor>(firstName, lastName, department));
    break;
  case Employee::Position::Journalist:
    employees.push_back(std::make_unique<Journalist>(firstName, lastName, department));
    break;
  case Employee::Position::Photographer:
    employees.push_back(std::make_unique<Photographer>(firstName, lastName, department));
    break;
  }

  return employees.back().get();
}

} // namespace

class Passwords {
public:
  std::map<std::string, std::pair<std::string, Employee*>> users;

  Passwords(int i) {
    std::cout << i << '\n';
  }

  Passwords() {
    users.emplace("user1", std::make_pair("password1", createEmployee(Employee::Position::Admin, "name", "lastname")));
  }

  Employee* authorization() {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs = 0;
    DWORD InputsRead = 0;
    INPUT_RECORD irInput;
    system("cls");
    std::string inputUsername, inputPassword;

    while (true) {
      // asking user role
      std::cout << "Input login: ";
      std::cin >> inputUsername;
      std::cout << "Input password: ";
      std::cin >> inputPassword;

      auto it = users.find(inputUsername);
      if (it != users.end() && it->second.first == inputPassword) {
        std::cout << "Autorisation complete!" << std::endl;
        return it->second.second; // returning user role
      } else {
        std::cout << "Login or password is incorrect. Down arrow try again, up arrow exit." << std::endl;
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
        switch (irInput.EventType) {
        case KEY_EVENT: {
          if (irInput.Event.KeyEvent.bKeyDown) {
            if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
              system("cls");
              continue;
            }
            if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
              return nullptr;
              system("cls");
            }
          }
        }
        }
        system("pause");
        system("cls");
      }
    }
  }

  Employee* findByPosition(Employee::Position position) {
    auto it = std::ranges::find_if(users, [=](const auto& i) { return i.second.second->getPosition() == position; });
    return it == users.end() ? nullptr : it->second.second;
  }

  void addUser() {
    int roleNumber;
    std::string username, password;
    std::cout << "Enter the login name of the new user: ";
    std::cin >> username;
    std::cout << "Enter the password name of the new user: ";
    std::cin >> password;
    std::cout << "Enter the role of the new user(1 designer, 2 editor, 3 journalist, 4 photographer: ";
    std::cin >> roleNumber;
    users.emplace(
        username,                                                                                                 //
        std::make_pair(password, createEmployee(static_cast<Employee::Position>(roleNumber), username, password)) //
    );
    std::cout << "Password is written down\n";
  }

  void save(const std::string& filename) {
    std::ofstream out(filename, std::ios::binary | std::ios::trunc);
    if (!out.good()) {
      throw std::runtime_error("Failed to open the file for writing");
    }

    int value = users.size();
    out.write(reinterpret_cast<char*>(&value), sizeof(value));
    for (const auto& [name, user] : users) {
      // name serialization
      value = name.size();
      out.write(reinterpret_cast<char*>(&value), sizeof(value));
      out.write(name.c_str(), value);

      // password serialization
      value = user.first.size();
      out.write(reinterpret_cast<char*>(&value), sizeof(value));
      out.write(user.first.c_str(), value);

      // position serialization
      value = static_cast<int>(user.second->getPosition());
      out.write(reinterpret_cast<char*>(&value), sizeof(value));
    }
  }

  void load(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.good()) {
      throw std::runtime_error("Failed to open the file for reading");
    }

    int userCount;
    in.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));

    for (int i = 0; i < userCount; i++) {
      // name deserialization
      int value;
      in.read(reinterpret_cast<char*>(&value), sizeof(value));
      std::string name(value, '-');
      in.read(name.data(), value);

      // password deserialization
      in.read(reinterpret_cast<char*>(&value), sizeof(value));
      std::string password(value, '-');
      in.read(password.data(), value);

      // position deserialization
      in.read(reinterpret_cast<char*>(&value), sizeof(value));
      const auto position = static_cast<Employee::Position>(value);

      users.emplace(name,                                                              //
                    std::make_pair(password, createEmployee(position, name, password)) //
      );
    }
  }
};

void addUser(std::map<std::string, std::pair<std::string, std::string>>& users);
std::string authorization(std::map<std::string, std::pair<std::string, std::string>>& users);

// authorization
vector<string> menu1 = {" Authorization", "Exit", ""};
// admin
vector<string> menu2 = {"Add new user", "Exit", ""};
// editor-in-chief
vector<string> menu3 = {"View works in progress",
                        "Exit", ""};

// journalist
vector<string> menu4 = {"View articles in progress", "Create article", "Exit", ""};

// journalist/View articles in progress
vector<string> menu4_1 = {"Add Article", "edit article", "delete article", " Exit ", ""};
// lit editor
vector<string> menu5 = {"View source files ", "Add new Layout ", "View current layouts", "Exit", ""};
// lit editor/View articles in the editorial office
vector<string> menu5_1 = {"Edit Article", "Delete Article", "Exit", ""};

// photographer
vector<string> menu6 = {"Upload photo", " Edit photo", "Exit", ""};
void print_menu(int switcher, const vector<string>& menu);

int main() {
  Passwords Passwords1;
  ifstream file("passwords.bin");
  if ((file.good())) {
    Passwords1.load("passwords.bin");
  }
  // setlocale(LC_ALL, "Russian");
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  DWORD NumInputs = 0;
  DWORD InputsRead = 0;
  bool running = true;
  int switcher = 2;
  Employee::Position pos;
  Employee* employee = nullptr;
  bool autorisation = true;
  string login;
  string password;
  INPUT_RECORD irInput;
  GetNumberOfConsoleInputEvents(hInput, &NumInputs);

  bool miganie = true;
  while (running) {
    if (miganie) {
      system("cls");
      print_menu(switcher, menu1);
    }
    ReadConsoleInput(hInput, &irInput, 1, &InputsRead);

    switch (irInput.EventType) {

    case KEY_EVENT:
      if (irInput.Event.KeyEvent.bKeyDown) {
        if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
          if (switcher > 2) {
            switcher--;
          }
          system("cls");
          print_menu(switcher, menu1);
        } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
          if (switcher < menu1.size()) {
            switcher++;
          }
          system("cls");
          print_menu(switcher, menu1);
        } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_RETURN) {
          switch (switcher - 1) {
          case 1:
            system("cls");
            employee = Passwords1.authorization();
            if (employee == nullptr) {
              system("cls");
              break;
            }
            pos = employee->getPosition();
            std::cout << "your position is: " << pos << '\n';
            system("pause");
            if (pos == Employee::Position::Admin) {
              bool ans = 1;
              switcher = 2;
              miganie = true;
              while (ans) {

                if (miganie) {
                  system("cls");
                  print_menu(switcher, menu2);
                }
                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                switch (irInput.EventType) {
                case KEY_EVENT:
                  if (irInput.Event.KeyEvent.bKeyDown) {
                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
                      if (switcher > 2) {
                        switcher--;
                      }
                      system("cls");
                      print_menu(switcher, menu2);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
                      if (switcher < menu2.size()) {
                        switcher++;
                      }
                      system("cls");
                      print_menu(switcher, menu2);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_RETURN) {
                      switch (switcher - 1) {
                      case 1: {
                        system("cls");
                        Passwords1.addUser();
                        system("pause");
                        break;
                      }
                      case 2:
                        ans = 0;
                        system("cls");
                        break;
                      default:
                        ans = 0;
                        system("cls");
                        // system("pause");
                        break;
                      }
                    }
                  }
                }
                miganie = false;
              }

            } else if (pos == Employee::Position::Designer) {

              // menu Designer

              bool ans = 1;
              switcher = 2;
              miganie = true;
              while (ans) {
                if (miganie) {
                  system("cls");
                  print_menu(switcher, menu5);
                }
                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                switch (irInput.EventType) {
                case KEY_EVENT:
                  if (irInput.Event.KeyEvent.bKeyDown) {
                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
                      if (switcher > 2) {
                        switcher--;
                      }
                      system("cls");
                      print_menu(switcher, menu5);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
                      if (switcher < menu5.size()) {
                        switcher++;
                      }
                      system("cls");
                      print_menu(switcher, menu5);

                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_RETURN) {
                      auto* designer = static_cast<Designer*>(employee);

                      switch (switcher - 1) {
                      case 1: {
                        system("cls");
                        auto* journalist =
                            static_cast<Journalist*>(Passwords1.findByPosition(Employee::Position::Journalist));
                        auto* photographer =
                            static_cast<Photographer*>(Passwords1.findByPosition(Employee::Position::Photographer));
                        if (journalist) {
                          journalist->viewArticles();
                        } else {
                          std::cout << "No journalists available.\n";
                        }
                        if (photographer) {
                          photographer->viewPhotos();
                        } else {
                          std::cout << "No photographers available.\n";
                        }
                        break;
                      }
                      case 2: {
                        system("cls");

                        designer->setLayout();
                        break;
                      }
                      case 3: {
                        system("cls");
                        designer->viewLayouts();
                        break;
                      }

                      case 4:
                        ans = 0;
                        system("cls");
                        switcher = 2;

                        break;
                      default:
                        ans = 0;
                        system("cls");
                        // system("pause");
                        break;
                      }
                    }
                  }
                }
                miganie = false;
              }
            } else if (pos == Employee::Position::Editor) {

              // menu mainEditor

              bool ans = 1;
              switcher = 2;
              miganie = true;
              while (ans) {
                if (miganie) {
                  system("cls");
                  print_menu(switcher, menu3);
                }
                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                switch (irInput.EventType) {
                case KEY_EVENT:
                  if (irInput.Event.KeyEvent.bKeyDown) {
                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
                      if (switcher > 2) {
                        switcher--;
                      }
                      system("cls");
                      print_menu(switcher, menu3);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
                      if (switcher < menu3.size()) {
                        switcher++;
                      }
                      system("cls");
                      print_menu(switcher, menu3);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_RETURN) {
                      auto* editor = static_cast<Editor*>(employee);

                      switch (switcher - 1) {
                      case 1: {
                        system("cls");
                        auto* journalist =
                            static_cast<Journalist*>(Passwords1.findByPosition(Employee::Position::Journalist));
                        auto* photographer =
                            static_cast<Photographer*>(Passwords1.findByPosition(Employee::Position::Photographer));
                        auto* designer =
                            static_cast<Designer*>(Passwords1.findByPosition(Employee::Position::Designer));
                        if (journalist) {
                          journalist->viewArticles();
                        } else {
                          std::cout << "No journalists available.\n";
                        }
                        if (photographer) {
                          photographer->viewPhotos();
                        } else {
                          std::cout << "No photographers available.\n";
                        }
                        if (designer) {
                          designer->viewLayouts();
                        } else {
                          std::cout << "No designers available.\n";
                        }
                        break;
                      }
                      
                      case 2:
                        ans = 0;
                        system("cls");
                        switcher = 2;

                        break;
                      default:
                        ans = 0;
                        system("cls");
                        // system("pause");
                        break;
                      }
                    }
                  }
                }
                miganie = false;
              }
            } else if (pos == Employee::Position::Journalist) {
              // menu journalist
              bool ans = 1;
              switcher = 2;
              miganie = true;
              while (ans) {
                if (miganie) {
                  system("cls");
                  print_menu(switcher, menu4);
                }
                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                switch (irInput.EventType) {
                case KEY_EVENT:
                  if (irInput.Event.KeyEvent.bKeyDown) {
                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
                      if (switcher > 2) {
                        switcher--;
                      }
                      system("cls");
                      print_menu(switcher, menu4);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
                      if (switcher < menu4.size()) {
                        switcher++;
                      }
                      system("cls");
                      print_menu(switcher, menu4);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_RETURN) {
                      auto* journalist = static_cast<Journalist*>(employee);
                      switch (switcher - 1) {
                      case 1: {
                        system("cls");
                        journalist->viewArticles();

                        break;
                      }
                      case 2:

                        system("cls");
                        journalist->createArticle();
                        break;
                      case 3:
                        ans = 0;
                        system("cls");
                        break;
                      default:
                        ans = 0;
                        system("cls");
                        // system("pause");
                        break;
                      }
                    }
                  }
                }
                miganie = false;
              }
            } else if (pos == Employee::Position::Photographer) { // menu photograph
              bool ans = 1;
              switcher = 2;
              miganie = true;
              while (ans) {
                if (miganie) {
                  system("cls");
                  print_menu(switcher, menu6);
                }
                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                switch (irInput.EventType) {
                case KEY_EVENT:
                  if (irInput.Event.KeyEvent.bKeyDown) {
                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP) {
                      if (switcher > 2) {
                        switcher--;
                      }
                      system("cls");
                      print_menu(switcher, menu6);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN) {
                      if (switcher < menu6.size()) {
                        switcher++;
                      }
                      system("cls");
                      print_menu(switcher, menu6);
                    } else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_RETURN) {
                      auto* photographer = static_cast<Photographer*>(employee);
                      switch (switcher - 1) {
                      case 1: {
                        system("cls");
                        photographer->setPhoto();
                        break;
                      }
                      case 2: {
                        system("cls");
                        photographer->viewPhotos();
                        break;
                      }
                      case 3:
                        ans = 0;
                        system("cls");
                        break;
                      default:
                        ans = 0;
                        system("cls");
                        // system("pause");
                        break;
                      }
                    }
                  }
                }
                miganie = false;
              }
            }
            break;
          case 2:
            running = 0;
            break;
          default:
            break;
          }
        }
      }
    }
    miganie = false;
  }

  Passwords1.save("passwords.bin");
}

void print_menu(int switcher, const vector<string>& menu) {
  for (int i = 0; i < menu.size(); i++) {
    if (i + 2 == switcher) {
      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
      CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
      GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
      WORD saved_attributes = consoleInfo.wAttributes;
      SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE |
                                            BACKGROUND_INTENSITY);
      cout << ">>" << menu[i] << '\n';
      SetConsoleTextAttribute(hConsole, saved_attributes);
    } else {
      cout << "  " << menu[i] << '\n';
    }
  }
}
