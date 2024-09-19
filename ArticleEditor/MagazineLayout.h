#pragma once

#include "Document.h"

class MagazineLayout final : public Document {
public:
  Type getType() const override {
    return Type::MagazineLayout;
  }

  std::string_view getLayout() const {
    return layout;
  }

  void setLayout(std::string_view newLayout) {
    layout = newLayout;
  }

  std::string_view getTypeLayout() const {
    return typeLayout;
  }

  void setTypeLayout(std::string_view newtypeLayout) {
    typeLayout = newtypeLayout;
  }

  void serialize(std::ofstream& out) const {
    size_t layoutSize = layout.size();
    size_t typeLayoutSize = typeLayout.size();

    // Записываем размеры строк
    out.write(reinterpret_cast<const char*>(&layoutSize), sizeof(layoutSize));
    out.write(layout.c_str(), layoutSize);
    out.write(reinterpret_cast<const char*>(&typeLayoutSize), sizeof(typeLayoutSize));
    out.write(typeLayout.c_str(), typeLayoutSize);
  }

  // Функция для десериализации из бинарного файла
  void deserialize(std::ifstream& in) {
    size_t layoutSize;
    size_t typeLayoutSize;

    // Считываем размеры строк
    in.read(reinterpret_cast<char*>(&layoutSize), sizeof(layoutSize));
    layout.resize(layoutSize);
    in.read(&layout[0], layoutSize);

    in.read(reinterpret_cast<char*>(&typeLayoutSize), sizeof(typeLayoutSize));
    typeLayout.resize(typeLayoutSize);
    in.read(&typeLayout[0], typeLayoutSize);
  }


// Функции для работы с вектором MagazineLayout
void saveLayoutsToFile(const std::vector<MagazineLayout>& layouts, const std::string& filename) {
  std::ofstream out(filename, std::ios::binary);

  size_t size = layouts.size();
  // Сохраняем размер вектора
  out.write(reinterpret_cast<const char*>(&size), sizeof(size));

  // Сериализуем каждый элемент
  for (const auto& layout : layouts) {
    layout.serialize(out);
  }
}

void loadLayoutsFromFile(std::vector<MagazineLayout>& layouts, const std::string& filename) {
  std::ifstream in(filename, std::ios::binary);

  size_t size;
  // Считываем размер вектора
  in.read(reinterpret_cast<char*>(&size), sizeof(size));
  layouts.resize(size);

  // Десериализуем каждый элемент
  for (auto& layout : layouts) {
    layout.deserialize(in);
  }
}

private:
  std::string layout;
  std::string typeLayout;
};